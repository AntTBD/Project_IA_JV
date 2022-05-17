#include "DrunkManOwnedStates.h"
#include "fsm/State.h"
#include "DrunkMan.h"
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

//methods for IsDrinking-----------------------------------------------------------------------------
IsDrinking* IsDrinking::Instance()
{
  static IsDrinking instance;

  return &instance;
}

void IsDrinking::Enter(DrunkMan* pDrunkMan)
{
    pDrunkMan->AddMessage("Let's drink !");
}

void IsDrinking::Execute(DrunkMan* pDrunkMan)
{
    pDrunkMan->AddMessage("Drinkin' som' beer !");
  pDrunkMan->IncreaseAlcoholLevel();

  if(pDrunkMan->isAlcoholic())
  {
      pDrunkMan->GetFSM()->ChangeState(DrunkFight::Instance());
  }
}

void IsDrinking::Exit(DrunkMan* pDrunkMan)
{
    pDrunkMan->AddMessage("Think i'm gettin' drunk man !");
}

bool IsDrinking::OnMessage(DrunkMan* pDrunkMan, const Telegram& msg)
{
    //send msg to global message handler
    return false;
}

//methods for DrunkFight-----------------------------------------------------------------------------
DrunkFight* DrunkFight::Instance()
{
  static DrunkFight instance;

  return &instance;
}

void DrunkFight::Enter(DrunkMan* pDrunkMan)
{
    pDrunkMan->AddMessage("Who wants to get these damn hands ?");
}

void DrunkFight::Execute(DrunkMan* pDrunkMan) {
    pDrunkMan->AddMessage("Ready to kick some ass ! *burp*");

    pDrunkMan->IncreaseAlcoholLevel();

    Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
                              pDrunkMan->ID(),      //ID of sender
                              ent_Miner_Bob,        //ID of recipient
                              Msg_Fight,            //the message
                              NO_ADDITIONAL_INFO);
}

void DrunkFight::Exit(DrunkMan* pDrunkMan)
{
    pDrunkMan->AddMessage("Claudius stopped fighting.");
}

bool DrunkFight::OnMessage(DrunkMan* pDrunkMan, const Telegram& msg)
{
    switch(msg.Msg)
    {
        case Msg_LostFight:

            pDrunkMan->AddMessage("\nMessage handled by " + GetNameOfEntity(pDrunkMan->ID())
                 + " at time: " + std::to_string(Clock->GetCurrentTime()), true);

            pDrunkMan->AddMessage("F*** You Bob, i'll get ya next time !");

            pDrunkMan->GetFSM()->ChangeState(SoberingUp::Instance());

            return true;

        case Msg_WinFight:

            pDrunkMan->AddMessage("\nMessage handled by " + GetNameOfEntity(pDrunkMan->ID())
                 + " at time: " + std::to_string(Clock->GetCurrentTime()), true);

            pDrunkMan->AddMessage("I beat his damn ass ! Time to shake some booties !");

            pDrunkMan->GetFSM()->ChangeState(IsSinging::Instance());

            return true;

    }//end switch

    return false; //send message to global message handler
}

//methods for SoberingUp-----------------------------------------------------------------------------

SoberingUp* SoberingUp::Instance()
{
    static SoberingUp instance;

    return &instance;
}

void SoberingUp::Enter(DrunkMan *pDrunkMan)
{
    pDrunkMan->AddMessage("ZzZZzz...zzZzz...");
}

void SoberingUp::Execute(DrunkMan *pDrunkMan)
{
    pDrunkMan->AddMessage("Zzz.. That is a big sheep..zzZ...");

    pDrunkMan->DecreaseAlcoholLevel();

    if(pDrunkMan->isSober())
    {
        pDrunkMan->GetFSM()->ChangeState(IsDrinking::Instance());
    }
}

void SoberingUp::Exit(DrunkMan *pDrunkMan)
{
    pDrunkMan->AddMessage("Argh... Ma goddamn jaw hurts...");
}

bool SoberingUp::OnMessage(DrunkMan *pDrunkMan, const Telegram &msg)
{
    //send msg to global message handler
    return false;
}

//methods for IsSinging-----------------------------------------------------------------------------

IsSinging* IsSinging::Instance()
{
    static IsSinging instance;

    return &instance;
}

void IsSinging::Enter(DrunkMan *pDrunkMan)
{
    pDrunkMan->AddMessage("Yeeeeeehaaaaaa! Claudius starts dancing");
}

void IsSinging::Execute(DrunkMan *pDrunkMan)
{
    if (RandFloat() > 0.5)
    {
        pDrunkMan->AddMessage("Where did you come from, where did you go?! Where did you come from, Cotton-Eye Joe?");
    }
    else
    {
        pDrunkMan->AddMessage("If it hadn't been for Cotton-Eye Joe! I'd been married long time ago!");
    }
    pDrunkMan->GetFSM()->ChangeState(SoberingUp::Instance());
}

void IsSinging::Exit(DrunkMan *pDrunkMan)
{
    pDrunkMan->AddMessage("Is da table tilted or is it ma legs ?? *falls*");
}

bool IsSinging::OnMessage(DrunkMan *pDrunkMan, const Telegram &msg)
{
    //send msg to global message handler
    return false;
}