#include "MinerOwnedStates.h"
#include "fsm/State.h"
#include "Miner.h"
#include "Locations.h"
#include "messaging/Telegram.h"
#include "MessageDispatcher.h"
#include "MessageTypes.h"
#include "Time/CrudeTimer.h"
#include "EntityNames.h"

#include <iostream>
using std::cout;


#ifdef TEXTOUTPUT
#include <fstream>
extern std::ofstream os;
#define cout os
#endif


//------------------------------------------------------------------------methods for EnterMineAndDigForNugget
EnterMineAndDigForNugget* EnterMineAndDigForNugget::Instance()
{
  static EnterMineAndDigForNugget instance;

  return &instance;
}


void EnterMineAndDigForNugget::Enter(Miner* pMiner)
{
  //if the miner is not already located at the goldmine, he must
  //change location to the gold mine
  if (pMiner->Location() != goldmine)
  {
    pMiner->AddMessage("Walkin' to the goldmine");

    pMiner->ChangeLocation(goldmine);
  }
}


void EnterMineAndDigForNugget::Execute(Miner* pMiner)
{  
  //Now the miner is at the goldmine he digs for gold until he
  //is carrying in excess of MaxNuggets. If he gets thirsty during
  //his digging he packs up work for a while and changes state to
  //gp to the saloon for a whiskey.
  pMiner->AddToGoldCarried(1);

  pMiner->IncreaseFatigue();

    pMiner->AddMessage("Pickin' up a nugget");

  //if enough gold mined, go and put it in the bank
  if (pMiner->PocketsFull())
  {
    pMiner->GetFSM()->ChangeState(VisitBankAndDepositGold::Instance());
  }

  if (pMiner->Thirsty())
  {
    pMiner->GetFSM()->ChangeState(QuenchThirst::Instance());
  }
}


void EnterMineAndDigForNugget::Exit(Miner* pMiner)
{
    pMiner->AddMessage("Ah'm leavin' the goldmine with mah pockets full o' sweet gold");
}


bool EnterMineAndDigForNugget::OnMessage(Miner* pMiner, const Telegram& msg)
{
  //send msg to global message handler
  return false;
}

//------------------------------------------------------------------------methods for VisitBankAndDepositGold

VisitBankAndDepositGold* VisitBankAndDepositGold::Instance()
{
  static VisitBankAndDepositGold instance;

  return &instance;
}

void VisitBankAndDepositGold::Enter(Miner* pMiner)
{  
  //on entry the miner makes sure he is located at the bank
  if (pMiner->Location() != bank)
  {
      pMiner->AddMessage("Goin' to the bank. Yes siree");

        pMiner->ChangeLocation(bank);
  }
}


void VisitBankAndDepositGold::Execute(Miner* pMiner)
{
  //deposit the gold
  pMiner->AddToWealth(pMiner->GoldCarried());
    
  pMiner->SetGoldCarried(0);

    pMiner->AddMessage("Depositing gold. Total savings now: " + std::to_string(pMiner->Wealth()));

  //wealthy enough to have a well earned rest?
  if (pMiner->Wealth() >= ComfortLevel)
  {
      pMiner->AddMessage("WooHoo! Rich enough for now. Back home to mah li'lle lady");

      pMiner->GetFSM()->ChangeState(GoHomeAndSleepTilRested::Instance());
  }

  //otherwise get more gold
  else 
  {
    pMiner->GetFSM()->ChangeState(EnterMineAndDigForNugget::Instance());
  }
}


void VisitBankAndDepositGold::Exit(Miner* pMiner)
{
    pMiner->AddMessage("Leavin' the bank");
}


bool VisitBankAndDepositGold::OnMessage(Miner* pMiner, const Telegram& msg)
{
  //send msg to global message handler
  return false;
}
//------------------------------------------------------------------------methods for GoHomeAndSleepTilRested

GoHomeAndSleepTilRested* GoHomeAndSleepTilRested::Instance()
{
  static GoHomeAndSleepTilRested instance;

  return &instance;
}

void GoHomeAndSleepTilRested::Enter(Miner* pMiner)
{
  if (pMiner->Location() != shack)
  {
      pMiner->AddMessage("Walkin' home");

    pMiner->ChangeLocation(shack); 

    //let the wife know I'm home
    Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
                              pMiner->ID(),        //ID of sender
                              ent_Elsa,            //ID of recipient
                              Msg_HiHoneyImHome,   //the message
                              NO_ADDITIONAL_INFO);    
  }
}

void GoHomeAndSleepTilRested::Execute(Miner* pMiner)
{ 
  //if miner is not fatigued start to dig for nuggets again.
  if (!pMiner->Fatigued())
  {
      pMiner->AddMessage("All mah fatigue has drained away. Time to find more gold!");

     pMiner->GetFSM()->ChangeState(EnterMineAndDigForNugget::Instance());
  }

  else 
  {
    //sleep
    pMiner->DecreaseFatigue();

      pMiner->AddMessage("ZZZZ... ");
  } 
}

void GoHomeAndSleepTilRested::Exit(Miner* pMiner)
{ 
}


bool GoHomeAndSleepTilRested::OnMessage(Miner* pMiner, const Telegram& msg)
{
   switch(msg.Msg)
   {
   case Msg_StewReady:

       pMiner->AddMessage("\nMessage handled by " + GetNameOfEntity(pMiner->ID())
     + " at time: " + std::to_string(Clock->GetCurrentTime()), true);

     pMiner->AddMessage("Okay Hun, ahm a comin'!");

     pMiner->GetFSM()->ChangeState(EatStew::Instance());
      
     return true;

   }//end switch

   return false; //send message to global message handler
}

//------------------------------------------------------------------------QuenchThirst

QuenchThirst* QuenchThirst::Instance()
{
  static QuenchThirst instance;

  return &instance;
}

void QuenchThirst::Enter(Miner* pMiner)
{
  if (pMiner->Location() != saloon)
  {    
    pMiner->ChangeLocation(saloon);

      pMiner->AddMessage("Boy, ah sure is thusty! Walking to the saloon");
  }
}

void QuenchThirst::Execute(Miner* pMiner)
{
  pMiner->BuyAndDrinkAWhiskey();

    pMiner->AddMessage("That's mighty fine sippin' liquer");

  pMiner->GetFSM()->ChangeState(EnterMineAndDigForNugget::Instance());
}


void QuenchThirst::Exit(Miner* pMiner)
{
    pMiner->AddMessage("Leaving the saloon, feelin' good");
}


bool QuenchThirst::OnMessage(Miner* pMiner, const Telegram& msg)
{
    switch(msg.Msg)
    {
        case Msg_Fight:

            pMiner->AddMessage("\nMessage handled by " + GetNameOfEntity(pMiner->ID())
                 + " at time: " + std::to_string(Clock->GetCurrentTime()), true);

            pMiner->GetFSM()->ChangeState(FightMode::Instance());

            return true;

    }//end switch

    return false; //send message to global message handler
}

//------------------------------------------------------------------------EatStew

EatStew* EatStew::Instance()
{
  static EatStew instance;

  return &instance;
}


void EatStew::Enter(Miner* pMiner)
{
    pMiner->AddMessage("Smells Reaaal goood Elsa!");
}

void EatStew::Execute(Miner* pMiner)
{
    pMiner->AddMessage("Tastes real good too!");

  pMiner->GetFSM()->RevertToPreviousState();
}

void EatStew::Exit(Miner* pMiner)
{
    pMiner->AddMessage("Thankya li'lle lady. Ah better get back to whatever ah wuz doin'");
}


bool EatStew::OnMessage(Miner* pMiner, const Telegram& msg)
{
  //send msg to global message handler
  return false;
}


//------------------------------------------------------------------------FightMode

FightMode* FightMode::Instance()
{
    static FightMode instance;

    return &instance;
}


void FightMode::Enter(Miner* pMiner)
{
    pMiner->AddMessage("I bet you can't tell skunks from house cats!");
}

void FightMode::Execute(Miner* pMiner)
{
    pMiner->AddMessage("Take that, yellow belly!");

    pMiner->IncreaseFatigue();

    if(RandFloat() < 0.5)
    {
        Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
                                  pMiner->ID(),      //ID of sender
                                  ent_Drunk_Claudius,   //ID of recipient
                                  Msg_WinFight,         //the message
                                  NO_ADDITIONAL_INFO);

        pMiner->GetFSM()->ChangeState(GoHomeAndSleepTilRested::Instance());
    }
    else
    {
        Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
                                  pMiner->ID(),      //ID of sender
                                  ent_Drunk_Claudius,   //ID of recipient
                                  Msg_LostFight,        //the message
                                  NO_ADDITIONAL_INFO);
        
        pMiner->GetFSM()->ChangeState(QuenchThirst::Instance());
    }
}

void FightMode::Exit(Miner* pMiner)
{
    pMiner->AddMessage("I'd better get some rest...");
}


bool FightMode::OnMessage(Miner* pMiner, const Telegram& msg)
{
    //send msg to global message handler
    return false;
}


