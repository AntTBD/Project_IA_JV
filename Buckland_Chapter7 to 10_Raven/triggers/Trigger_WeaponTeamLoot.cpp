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
    SetEntityType(type_loot);
    m_weaponType = weapon->GetType();
    m_ammoCurrentWeaponType = weapon->NumRoundsRemaining();

    //create this trigger's region of fluence
    AddCircularTriggerRegion(Pos(), BRadius());

#ifdef LOG_CREATIONAL_STUFF
    debug_con << "Creating a " << GetNameOfType(EntityType()) << " with " << m_ammoCurrentWeaponType << " ammos of "  << GetNameOfType(m_weaponType) << " for Team : \"" << m_Team->GetName() << "\" at " << Pos() << " (node : " <<  node_idx << ")\n" << "";
#endif

    // if not in team match mode remove this trigger
    if (m_pWorld->GetMode() != Raven_Game::Mode::Team)
    {
        SetInactive();
        SetToBeRemovedFromGame();
    }
    else {
        SetActive();
    }



    //create the vertex buffer for the rocket shape
    const int NumRocketVerts = 8;
    const Vector2D rip[NumRocketVerts] = { Vector2D(0, 3),
                                            Vector2D(1, 2),
                                            Vector2D(1, 0),
                                            Vector2D(2, -2),
                                            Vector2D(-2, -2),
                                            Vector2D(-1, 0),
                                            Vector2D(-1, 2),
                                            Vector2D(0, 3) };

    for (int i = 0; i < NumRocketVerts; ++i)
    {
        m_vecRLVB.push_back(rip[i]);
    }
}

void Trigger_WeaponTeamLoot::Try(Raven_Bot* pBot)
{
    // if not in team match mode remove this trigger
    if (m_pWorld->GetMode() == Raven_Game::Mode::Team) {

        if (this->isActive() && this->isTouchingTrigger(pBot->Pos(), pBot->BRadius()) &&
            pBot->GetTeam() == m_Team) {

            //if the bot holds a weapon of this type, add its ammo
            Raven_Weapon* present = pBot->GetWeaponSys()->GetWeaponFromInventory(m_weaponType);

            if (present)
            {
                if(m_ammoCurrentWeaponType > 0)
                    present->IncrementRounds(m_ammoCurrentWeaponType);
            }
            else
            {
                pBot->GetWeaponSys()->AddWeapon(m_weaponType);
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
//#ifdef LOG_CREATIONAL_STUFF
//            debug_con << "TeamId: " << m_Team->ID() << " >> color " << (m_Team->ID() % NumColors) << " " << colors[m_Team->ID() % NumColors] << "";
//#endif
            gdi->SetBrushColor(m_Team->ID() % NumColors);
            gdi->SetPenColor(m_Team->ID() % NumColors);
           // CreatePen(PS_SOLID, 2, colors[m_Team->ID() % NumColors]);
            //gdi->WhiteBrush();
        } else {
            gdi->ThickBlackPen();
            gdi->BlackBrush();
            //gdi->WhiteBrush();
        }
        gdi->Circle(Pos(), BRadius()*2.f);

        switch (m_weaponType)
        {
        case type_rail_gun:
        {
            gdi->BluePen();
            gdi->BlueBrush();
            gdi->Circle(Pos(), 3);
            gdi->ThickBluePen();
            gdi->Line(Pos(), Vector2D(Pos().x, Pos().y - 9));
        }

        break;

        case type_shotgun:
        {

            gdi->BlackBrush();
            gdi->BrownPen();
            const double sz = 3.0;
            gdi->Circle(Pos().x - sz, Pos().y, sz);
            gdi->Circle(Pos().x + sz, Pos().y, sz);
        }

        break;

        case type_rocket_launcher:
        {

            Vector2D facing(-1, 0);

            m_vecRLVBTrans = WorldTransform(m_vecRLVB,
                Pos(),
                facing,
                facing.Perp(),
                Vector2D(2.5, 2.5));

            gdi->RedPen();
            gdi->ClosedShape(m_vecRLVBTrans);
        }

        break;

        default:
        {

#ifdef LOG_CREATIONAL_STUFF
            debug_con << "Weapon type unknown " << GetNameOfType(m_weaponType) << "";
#endif
            gdi->ThickBlackPen();
            gdi->WhiteBrush();
            gdi->Circle(Pos(), BRadius()*2.f);
        }
        break;

        }//end switch

    }
}