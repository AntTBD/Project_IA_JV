#ifndef DRUNKMANOWNEDSTATES_H
#define DRUNKMANOWNEDSTATES_H
//------------------------------------------------------------------------
//
//  Name:   DrunkManOwnedStates.h
//
//  Desc:   All the states that can be assigned to the Drunk Man class.
//          Note that a global state has not been implemented.
//
//  Author:
//
//------------------------------------------------------------------------
#include "fsm/State.h"


class DrunkMan;
struct Telegram;


//------------------------------------------------------------------------
//
//
//------------------------------------------------------------------------
class IsDrinking : public State<DrunkMan>
{
private:

    IsDrinking(){}

    //copy ctor and assignment should be private
    IsDrinking(const IsDrinking&);
    IsDrinking& operator=(const IsDrinking&);

public:

    //this is a singleton
    static IsDrinking* Instance();

    virtual void Enter(DrunkMan* drunkMan);

    virtual void Execute(DrunkMan* drunkMan);

    virtual void Exit(DrunkMan* drunkMan);

    virtual bool OnMessage(DrunkMan* agent, const Telegram& msg);

};

//------------------------------------------------------------------------
//
//
//------------------------------------------------------------------------
class SoberingUp : public State<DrunkMan>
{
private:

    SoberingUp(){}

    //copy ctor and assignment should be private
    SoberingUp(const SoberingUp&);
    SoberingUp& operator=(const SoberingUp&);

public:

    //this is a singleton
    static SoberingUp* Instance();

    virtual void Enter(DrunkMan* drunkMan);

    virtual void Execute(DrunkMan* drunkMan);

    virtual void Exit(DrunkMan* drunkMan);

    virtual bool OnMessage(DrunkMan* agent, const Telegram& msg);

};


//------------------------------------------------------------------------
//
//
//------------------------------------------------------------------------
class DrunkFight : public State<DrunkMan>
{
private:

    DrunkFight(){}

    //copy ctor and assignment should be private
    DrunkFight(const DrunkFight&);
    DrunkFight& operator=(const DrunkFight&);

public:

    //this is a singleton
    static DrunkFight* Instance();

    virtual void Enter(DrunkMan* drunkMan);

    virtual void Execute(DrunkMan* drunkMan);

    virtual void Exit(DrunkMan* drunkMan);

    virtual bool OnMessage(DrunkMan* agent, const Telegram& msg);

};

//------------------------------------------------------------------------
//
//
//------------------------------------------------------------------------
class IsSinging : public State<DrunkMan>
{
private:

    IsSinging(){}

    //copy ctor and assignment should be private
    IsSinging(const IsSinging&);
    IsSinging& operator=(const IsSinging&);

public:

    //this is a singleton
    static IsSinging* Instance();

    virtual void Enter(DrunkMan* drunkMan);

    virtual void Execute(DrunkMan* drunkMan);

    virtual void Exit(DrunkMan* drunkMan);

    virtual bool OnMessage(DrunkMan* agent, const Telegram& msg);

};
#endif //DRUNKMANOWNEDSTATES_H
