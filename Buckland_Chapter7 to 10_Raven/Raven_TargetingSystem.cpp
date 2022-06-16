#include "Raven_TargetingSystem.h"
#include "Raven_Bot.h"
#include "Raven_SensoryMemory.h"



//-------------------------------- ctor ---------------------------------------
//-----------------------------------------------------------------------------
Raven_TargetingSystem::Raven_TargetingSystem(Raven_Bot* owner):m_pOwner(owner),
                                                               m_pCurrentTarget(0)
{}



//----------------------------- Update ----------------------------------------

//-----------------------------------------------------------------------------
void Raven_TargetingSystem::Update() {
    double ClosestDistSoFar = MaxDouble;
    m_pCurrentTarget = 0;


    // Si sa team a une target, il la prend par défaut (si dans la boucle for, il trouve un bot plus près, alors il change de cible)
    if (m_pOwner->HasTeam() && m_pOwner->GetTeam()->GetTarget() != NULL) {
        double dist = Vec2DDistanceSq(m_pOwner->GetTeam()->GetTarget()->Pos(), m_pOwner->Pos());

        if (dist < ClosestDistSoFar) {

            ClosestDistSoFar = dist;
            m_pCurrentTarget = m_pOwner->GetTeam()->GetTarget();
        }
    }

    //grab a list of all the opponents the owner can sense
    std::list < Raven_Bot * > SensedBots;
    SensedBots = m_pOwner->GetSensoryMem()->GetListOfRecentlySensedOpponents();

    std::list<Raven_Bot *>::const_iterator curBot = SensedBots.begin();
    for (curBot; curBot != SensedBots.end(); ++curBot) {
        // switch to the next bot they are both in the same team
        if(m_pOwner->HasTeam() && (*curBot)->HasTeam() && m_pOwner->GetTeam() == (*curBot)->GetTeam())
            continue;

        //make sure the bot is alive and that it is not the owner
        if ((*curBot)->isAlive() && (*curBot != m_pOwner)) {

            double dist = Vec2DDistanceSq((*curBot)->Pos(), m_pOwner->Pos());

            if (dist < ClosestDistSoFar) {

                ClosestDistSoFar = dist;
                m_pCurrentTarget = *curBot;
            }
        }
    }
    if(m_pCurrentTarget != 0 && m_pOwner->HasTeam() && m_pOwner == m_pOwner->GetTeam()->GetLeader() && m_pOwner->GetTeam()->GetTarget() != m_pCurrentTarget){
        m_pOwner->GetTeam()->SetTarget(m_pCurrentTarget, m_pOwner->ID());
    }
}




bool Raven_TargetingSystem::isTargetWithinFOV()const
{
  return m_pOwner->GetSensoryMem()->isOpponentWithinFOV(m_pCurrentTarget);
}

bool Raven_TargetingSystem::isTargetShootable()const
{
  return m_pOwner->GetSensoryMem()->isOpponentShootable(m_pCurrentTarget);
}

Vector2D Raven_TargetingSystem::GetLastRecordedPosition()const
{
  return m_pOwner->GetSensoryMem()->GetLastRecordedPositionOfOpponent(m_pCurrentTarget);
}

double Raven_TargetingSystem::GetTimeTargetHasBeenVisible()const
{
  return m_pOwner->GetSensoryMem()->GetTimeOpponentHasBeenVisible(m_pCurrentTarget);
}

double Raven_TargetingSystem::GetTimeTargetHasBeenOutOfView()const
{
  return m_pOwner->GetSensoryMem()->GetTimeOpponentHasBeenOutOfView(m_pCurrentTarget);
}
