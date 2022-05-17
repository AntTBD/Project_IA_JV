#include "DrunkMan.h"

bool DrunkMan::HandleMessage(const Telegram& msg)
{
    return m_pStateMachine->HandleMessage(msg);
}


void DrunkMan::Update()
{
    m_pStateMachine->Update();
}