#ifndef AGENTPOURSUIVEUR_H
#define AGENTPOURSUIVEUR_H

#include "Vehicle.h"
#include "SteeringBehaviors.h"

class AgentPoursuiveur : public Vehicle
{

/*public:
    Vector2D m_vOffset;
    Vehicle* m_vLeader;
    Vehicle* m_vTarget;

    int m_iNbreAgent;*/

public:
    AgentPoursuiveur(GameWorld* world,
                     Vector2D position,
                     double rotation,
                     Vector2D velocity,
                     double mass,
                     double max_force,
                     double max_speed,
                     double max_turn_rate,
                     double scale,
                     Vehicle* leader,
                     Vehicle* target,
                     Vector2D offset,
                     int id,
                     int count);

};


#endif //AGENTPOURSUIVEUR_H
