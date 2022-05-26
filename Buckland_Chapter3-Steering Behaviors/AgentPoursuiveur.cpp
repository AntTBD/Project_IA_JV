#include "AgentPoursuiveur.h"
AgentPoursuiveur::AgentPoursuiveur(GameWorld* world, Vector2D position, double rotation, Vector2D velocity, double mass, double max_force, double max_speed, double max_turn_rate, double scale,
                                   Vehicle* leader) :
        Vehicle(world, position, rotation, velocity, mass, max_force, max_speed, max_turn_rate, scale) {
    this->SetRenderColor(Poursuiveur);

    this->m_vLeader = leader;


    this->Steering()->SeparationOn();
    this->Steering()->WanderOn();
}

void AgentPoursuiveur::FormationWander(){
    this->Steering()->OffsetPursuitOff();
    this->Steering()->WanderOn();
}
void AgentPoursuiveur::FormationCustom(Vehicle* target, Vector2D offset){
    this->Steering()->WanderOff();
    this->Steering()->OffsetPursuitOn(target, offset);
}
void AgentPoursuiveur::FormationLine(Vehicle* target, int offset){
    this->FormationCustom(target, Vector2D(offset,0));
}
void AgentPoursuiveur::FormationCirle(Vector2D offset){
    this->FormationCustom(this->m_vLeader, offset);
}
void AgentPoursuiveur::FormationV(Vehicle* target, Vector2D offset, int position){
    if (position < 3) {
        this->Steering()->OffsetPursuitOn(target, Vector2D(-offset.x, (position % 2 == 0 ? offset.x : -offset.y)));
    } else {
        this->Steering()->OffsetPursuitOn(target, Vector2D(-offset.x, (position % 2 == 0 ? offset.x : -offset.y)));
    }
}