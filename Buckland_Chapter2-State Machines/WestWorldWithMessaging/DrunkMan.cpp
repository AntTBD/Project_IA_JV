#include "DrunkMan.h"

bool DrunkMan::HandleMessage(const Telegram& msg)
{
    return m_pStateMachine->HandleMessage(msg);
}


void DrunkMan::Update()
{
    SetTextColor(FOREGROUND_BLUE| FOREGROUND_INTENSITY);

    m_pStateMachine->Update();
}