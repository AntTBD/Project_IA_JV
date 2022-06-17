#include "Goal_Fuite.h"
#include "../Raven_Bot.h"
#include "../Raven_SteeringBehaviors.h"
#include "../Raven_TargetingSystem.h"


void Goal_Fuite::Activate()
{
	m_iStatus = active;
	m_pOwner->GetSteering()->SeparationOn();
}

int Goal_Fuite::Process()
{
	ActivateIfInactive();
	return m_iStatus;
}

void Goal_Fuite::Terminate()
{
	m_pOwner->GetSteering()->SeparationOff();
}