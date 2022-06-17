#include "Raven_BotLearning.h"

#include "misc/Cgdi.h"
#include "misc/utils.h"
#include "2D/Transformations.h"
#include "2D/Geometry.h"
#include "lua/Raven_Scriptor.h"
#include "Raven_Game.h"
#include "navigation/Raven_PathPlanner.h"
#include "Raven_SteeringBehaviors.h"
#include "Raven_UserOptions.h"
#include "time/Regulator.h"
#include "Raven_WeaponSystem.h"
#include "Raven_SensoryMemory.h"

#include "Messaging/Telegram.h"
#include "Raven_Messages.h"
#include "Messaging/MessageDispatcher.h"

#include "goals/Raven_Goal_Types.h"
#include "goals/Goal_Think.h"


#include "Debug/DebugConsole.h"

Raven_BotLearning::Raven_BotLearning(Raven_Game* world, Vector2D pos) : Raven_Bot(world, pos) {
    m_ModeleAppris = world->GetModeleApprentissage();

}

Raven_BotLearning::~Raven_BotLearning()
{
    debug_con << "deleting " << typeid(this).name() << " (id = " << ID() << ")" << "";
}


void Raven_BotLearning::SetAlive()
{
    timer = std::chrono::high_resolution_clock::now();
    Raven_Bot::SetAlive();
}

void Raven_BotLearning::SetDead()
{
    auto end = std::chrono::high_resolution_clock::now();

    debug_con << "Bot apprenant " << ID() << " a vecu pendant : " << (std::chrono::duration_cast<std::chrono::seconds> (end - timer).count()) << "s" << "";
    debug_con << "Bot apprenant " << ID() << ": score : " << Score() << "";
    Raven_Bot::SetDead();
}

//-------------------------------- Update -------------------------------------
//
void Raven_BotLearning::Update()
{
    //process the currently active goal. Note this is required even if the bot
    //is under user control. This is because a goal is created whenever a user
    //clicks on an area of the map that necessitates a path planning request.
    m_pBrain->Process();

    //Calculate the steering force and update the bot's velocity and position
    UpdateMovement();

    //add one frame to TimeSinceTargetSeen
    //AddOneFrameToTargetSeen();

    //if the bot is under AI control but not scripted
    if (!isPossessed())
    {
        //examine all the opponents in the bots sensory memory and select one
        //to be the current target
        if (m_pTargetSelectionRegulator->isReady())
        {
            m_pTargSys->Update();
        }

        //appraise and arbitrate between all possible high level goals
        if (m_pGoalArbitrationRegulator->isReady())
        {
            m_pBrain->Arbitrate();
        }

        //update the sensory memory with any visual stimulus
        if (m_pVisionUpdateRegulator->isReady())
        {
            m_pSensoryMem->UpdateVision();
        }

        //select the appropriate weapon to use from the weapons currently in
        //the inventory
        if (m_pWeaponSelectionRegulator->isReady())
        {
            m_pWeaponSys->SelectWeapon();
        }

        //this method aims the bot's current weapon at the current target
        //and takes a shot if a shot is possible

        if (m_pTargSys->isTargetPresent()) {

            m_vecObservation.clear();

            m_vecObservation.push_back((Pos().Distance(m_pTargSys->GetTarget()->Pos())));
            m_vecObservation.push_back(m_pTargSys->isTargetWithinFOV());
            m_vecObservation.push_back(m_pWeaponSys->GetAmmoRemainingForWeapon(m_pWeaponSys->GetCurrentWeapon()->GetType()));
            m_vecObservation.push_back(m_pWeaponSys->GetCurrentWeapon()->GetType());
            m_vecObservation.push_back((Health()));
            m_vecObservation.push_back(m_pTargSys->GetTarget()->Health());


            std::vector<double> output = m_ModeleAppris.Update(m_vecObservation);

            bool canShoot = false;

            if(output.size() > 0)
                canShoot = output[0];

            if (canShoot)
                m_pWeaponSys->TakeAimAndShoot();

        }
    }
}

void Raven_BotLearning::Render() {
    Raven_Bot::Render();

    gdi->OrangeBrush();
    gdi->Circle(Pos(), 6.0 * Scale().x);
}

