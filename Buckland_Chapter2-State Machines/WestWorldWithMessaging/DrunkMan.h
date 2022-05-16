#ifndef DRUNKMAN_H
#define DRUNKMAN_H

//------------------------------------------------------------------------
//
//  Name:   DrunkMan.h
//
//  Desc:   A class defining a drunk man.
//
//  Author:
//
//------------------------------------------------------------------------
#include <string>
#include <cassert>
#include <iostream>

#include "BaseGameEntity.h"
#include "Locations.h"
#include "misc/ConsoleUtils.h"
#include "DrunkManOwnedStates.h"
#include "fsm/StateMachine.h"
#include "misc/Utils.h"

template <class entity_type> class State;
struct Telegram;

//above this value a drunker is drunk
const int  DrunkLevel = 5;



class DrunkMan : public BaseGameEntity {
private:

    //an instance of the state machine class
    StateMachine<DrunkMan>*  m_pStateMachine;

    // how many grammes of alcools in blood
    int m_iAlcoholLevel;

public:

    DrunkMan(int id):m_iAlcoholLevel(0),
                  BaseGameEntity(id)

    {
        //set up state machine
        m_pStateMachine = new StateMachine<DrunkMan>(this);

        m_pStateMachine->SetCurrentState(IsDrinking::Instance());

        /* NOTE, A GLOBAL STATE HAS NOT BEEN IMPLEMENTED FOR THE MINER */
    }

    ~DrunkMan(){delete m_pStateMachine;}

    //this must be implemented
    void Update();

    //so must this
    virtual bool  HandleMessage(const Telegram& msg);


    StateMachine<DrunkMan>* GetFSM()const{return m_pStateMachine;}


    void IncreaseAlcoholLevel(){m_iAlcoholLevel++;}
    void DecreaseAlcoholLevel(){m_iAlcoholLevel--;}

    //-------------------------------------------------------------accessors
    int GetAlcoholLevel()const{return m_iAlcoholLevel;}
    bool isSober()const{return m_iAlcoholLevel==0;}
    bool isAlcoholic()const{return m_iAlcoholLevel > DrunkLevel;}

};


#endif //DRUNKMAN_H
