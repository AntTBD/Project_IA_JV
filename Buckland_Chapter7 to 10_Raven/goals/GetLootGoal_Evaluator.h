#ifndef GETLOOTGOAL_EVALUATOR_H
#define GETLOOTGOAL_EVALUATOR_H

#include "Goal_Evaluator.h"
#include "../triggers/Trigger_WeaponTeamLoot.h"
#include "../Raven_Bot.h"


class GetLootGoal_Evaluator : public Goal_Evaluator {

public:

    GetLootGoal_Evaluator(double bias):Goal_Evaluator(bias){}

    double CalculateDesirability(Raven_Bot* pBot);

    void  SetGoal(Raven_Bot* pEnt);

    void RenderInfo(Vector2D Position, Raven_Bot* pBot);
};


#endif //GETLOOTGOAL_EVALUATOR_H
