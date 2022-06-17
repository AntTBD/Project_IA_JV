#ifndef GOAL_DODGE_SIDE_H
#define GOAL_DODGE_SIDE_H
#pragma warning (disable:4786)

#include "../Common/Goals/Goal.h"
#include "Raven_Goal_Types.h"
#include "../Raven_Bot.h"


class Goal_Fuite : public Goal<Raven_Bot>
{
public:
	Goal_Fuite(Raven_Bot* pBot) : Goal<Raven_Bot>(pBot, goal_fuite) {}

	void Activate();

	int Process();

	void Terminate();
};

#endif