#ifndef AGENTPOURSUIVEUR_H
#define AGENTPOURSUIVEUR_H

#include "Vehicle.h"
#include "SteeringBehaviors.h"

class AgentPoursuiveur : public Vehicle
{

private:
    Vehicle* m_vLeader;

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
                     Vehicle* leader);


    void FormationWander();
    void FormationCustom(Vehicle* target, Vector2D offset);
    void FormationLine(Vehicle* target, int offset);
    void FormationCirle(Vector2D offset);
    void FormationV(Vehicle* target, Vector2D offset, int position);

};


#endif //AGENTPOURSUIVEUR_H
