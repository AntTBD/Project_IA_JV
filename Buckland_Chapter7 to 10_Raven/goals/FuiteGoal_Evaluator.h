#include "../Raven_Bot.h"
#include "Goal_Evaluator.h"


class FuiteGoal_Evaluator : public Goal_Evaluator
{
protected:
	double m_dCharacterBias;

public:
	FuiteGoal_Evaluator(double CharacterBias):Goal_Evaluator(CharacterBias){}

	double CalculateDesirability(Raven_Bot* pBot);

	void SetGoal(Raven_Bot* pBot);

	void RenderInfo(Vector2D position, Raven_Bot* pBot);
};