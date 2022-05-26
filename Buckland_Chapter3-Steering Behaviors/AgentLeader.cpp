#include "AgentLeader.h"
#include "MovingEntity.h"

AgentLeader::AgentLeader(GameWorld* world, Vector2D position, double rotation, Vector2D velocity, double mass, double max_force, double max_speed, double max_turn_rate, double scale) :
Vehicle(world, position, rotation, velocity, mass, max_force, max_speed, max_turn_rate, scale) {

    this->SetRenderColor(Leader);
    this->useKeyboardInput = false;

    this->Steering()->FlockingOff();
    this->SetScale(Vector2D(10, 10));
    this->Steering()->WanderOn();
    this->SetMaxSpeed(70);
}

void AgentLeader::TurnLeft()
{
    if(this->useKeyboardInput){
        this->RotateHeadingToFacePosition(this->Pos() + this->Heading() - this->Side());
    }
}

void AgentLeader::TurnRight()
{
    if(this->useKeyboardInput) {
        this->RotateHeadingToFacePosition(this->Pos() + this->Heading() + this->Side());
    }
}

void AgentLeader::ToggleManualDriving(){
    this->useKeyboardInput = !this->useKeyboardInput;
    if (this->useKeyboardInput)
    {
        this->Steering()->WanderOff();
        this->Steering()->OffsetPursuitOn(this, Vector2D(150, 0));
    }
    else
    {
        this->Steering()->OffsetPursuitOff();
        this->Steering()->WanderOn();
    }
}

void AgentLeader::IncreaseSpeed() {
    if(this->useKeyboardInput) {
        this->SetMaxSpeed(this->MaxSpeed() + 10);
    }
}

void AgentLeader::DecreaseSpeed() {
    if(this->useKeyboardInput) {
        this->SetMaxSpeed(this->MaxSpeed() > 0 ? this->MaxSpeed() - 10 : 0);
    }
}