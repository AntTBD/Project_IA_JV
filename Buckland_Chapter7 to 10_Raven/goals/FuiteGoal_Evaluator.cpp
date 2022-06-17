#include "FuiteGoal_Evaluator.h"
#include "../navigation/Raven_PathPlanner.h"
#include "../Raven_ObjectEnumerations.h"
#include "../lua/Raven_Scriptor.h"
#include "misc/Stream_Utility_Functions.h"
#include "Raven_Feature.h"

#include "Goal_Think.h"
#include "Raven_Goal_Types.h"


double FuiteGoal_Evaluator::CalculateDesirability(Raven_Bot* pBot)
{
	double desirability = 0.02;

	if (pBot->Health() <= 20){
		desirability = 1;
	}
	else {
		desirability *= m_dCharacterBias + 0.03;
	}

	return desirability;
}

void FuiteGoal_Evaluator::SetGoal(Raven_Bot* pBot)
{
	pBot->GetBrain()->AddGoal_Fuite();
}

void FuiteGoal_Evaluator::RenderInfo(Vector2D position, Raven_Bot* pBot)
{
	std::string s = "Fuite : ";
	gdi->TextAtPos(position, s + ttos(CalculateDesirability(pBot), 2));
}