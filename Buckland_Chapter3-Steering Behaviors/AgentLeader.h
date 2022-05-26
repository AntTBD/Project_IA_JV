#ifndef AGENTLEADER_H
#define AGENTLEADER_H

#include "Vehicle.h"
#include "SteeringBehaviors.h"


class AgentLeader : public Vehicle
{

protected:
    bool useKeyboardInput;

public:

    AgentLeader(GameWorld* world,
                Vector2D position,
                double rotation,
                Vector2D velocity,
                double mass,
                double max_force,
                double max_speed,
                double max_turn_rate,
                double scale);

    void TurnLeft();
    void TurnRight();
    void ToggleManualDriving();

    void IncreaseSpeed();
    void DecreaseSpeed();

};


#endif //AGENTLEADER_H
