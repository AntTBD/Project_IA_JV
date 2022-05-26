#include "AgentPoursuiveur.h"
AgentPoursuiveur::AgentPoursuiveur(GameWorld* world, Vector2D position, double rotation, Vector2D velocity, double mass, double max_force, double max_speed, double max_turn_rate, double scale,
                                   Vehicle* leader,
                                   Vehicle* target,
                                   Vector2D offset,
                                   int id,
                                   int count) :
        Vehicle(world, position, rotation, velocity, mass, max_force, max_speed, max_turn_rate, scale) {
    this->SetRenderColor(Poursuiveur);

   /* this->m_vLeader = leader;
    this->m_vTarget = target;
    this->m_vOffset = offset;
    this->m_iNbreAgent = id;*/


    this->Steering()->SeparationOn();
    this->Steering()->WanderOn();
}

