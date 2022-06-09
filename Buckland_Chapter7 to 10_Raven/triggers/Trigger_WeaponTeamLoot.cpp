#include "Trigger_WeaponTeamLoot.h"
#include "misc/Cgdi.h"
#include "../lua/Raven_Scriptor.h"
#include "../constants.h"
#include "../Raven_ObjectEnumerations.h"
#include "../Raven_WeaponSystem.h"

//uncomment to write object creation/deletion to debug console
#define  LOG_CREATIONAL_STUFF
#include "debug/DebugConsole.h"

Trigger_WeaponTeamLoot::Trigger_WeaponTeamLoot(Vector2D pos, Raven_Team* team, Raven_Weapon* weapon, Raven_Game* world, int node_idx) :
        Trigger<Raven_Bot>(BaseGameEntity::GetNextValidID()),
        m_pWorld(world),
        m_Team(team)
{
    SetPos(pos);
    SetBRadius(5.0);
    SetGraphNodeIndex(node_idx);
    SetEntityType(weapon->GetType());
    m_ammoCurrentWeaponType = weapon->NumRoundsRemaining();

    //create this trigger's region of fluence
    AddCircularTriggerRegion(Pos(), BRadius());

#ifdef LOG_CREATIONAL_STUFF
    debug_con << "Creating a Trigger_WeaponTeamLoot with " << GetNameOfType(EntityType()) << " and " << m_ammoCurrentWeaponType << " ammo for Team : " << m_Team->GetName() << " at " << Pos() << "";
#endif

    // if not in team match mode remove this trigger
    if (m_pWorld->GetMode() != Raven_Game::Mode::Team)
    {
        SetToBeRemovedFromGame();
    }
}

void Trigger_WeaponTeamLoot::Try(Raven_Bot* pBot)
{
    // if not in team match mode remove this trigger
    if (m_pWorld->GetMode() == Raven_Game::Mode::Team) {

        if (this->isActive() && this->isTouchingTrigger(pBot->Pos(), pBot->BRadius()) &&
            pBot->GetTeam() == m_Team) {

            //if the bot holds a weapon of this type, add its ammo
            Raven_Weapon* present = pBot->GetWeaponSys()->GetWeaponFromInventory(EntityType());

            if (present)
            {
                if(m_ammoCurrentWeaponType > 0)
                    present->IncrementRounds(m_ammoCurrentWeaponType);
            }
            else
            {
                pBot->GetWeaponSys()->AddWeapon(EntityType());
            }

            SetInactive();
            SetToBeRemovedFromGame();
        }
    }
}

void Trigger_WeaponTeamLoot::Update() {

    // if not in team match mode remove this trigger
    if (m_pWorld->GetMode() != Raven_Game::Mode::Team)
    {
        SetToBeRemovedFromGame();
    }
}

void Trigger_WeaponTeamLoot::Render(){
    // if in team match mode
    if (isActive() && m_pWorld->GetMode() == Raven_Game::Mode::Team)
    {
        if (m_Team != NULL) {
            gdi->SetPenColor(m_Team->ID() % NumColors);
            gdi->SetBrushColor(m_Team->ID() % NumColors);
        } else {
            gdi->BlackPen();
            gdi->BlackBrush();
        }
        gdi->Circle(Pos(), BRadius());
    }
}