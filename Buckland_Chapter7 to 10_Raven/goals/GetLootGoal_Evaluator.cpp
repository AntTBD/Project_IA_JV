#include "GetLootGoal_Evaluator.h"
#include "../Raven_ObjectEnumerations.h"
#include "Goal_Think.h"
#include "Raven_Goal_Types.h"
#include "misc/Stream_Utility_Functions.h"
#include "Raven_Feature.h"
#include "../Raven_WeaponSystem.h"
#include "../armory/Raven_Weapon.h"
#include "../Raven_Game.h"
#include "../Raven_Map.h"
#include "../Raven_Team.h"
#include "../triggers/Trigger_WeaponTeamLoot.h"


//---------------------- CalculateDesirability -------------------------------------
//-----------------------------------------------------------------------------
double GetLootGoal_Evaluator::CalculateDesirability(Raven_Bot* pBot)
{
    if(pBot->HasTeam() == false) return 0;

    double Distance = 1;
    //grab the distance to the closest instance of the weapon type
    Raven_Map::NavGraph& navGraph = pBot->GetWorld()->GetMap()->GetNavGraph();
    if (navGraph.isNodePresent(pBot->GetTeam()->GetLootPointNodeId())) {
        Raven_Map::NavGraph::NodeType& node = navGraph.GetNode(pBot->GetTeam()->GetLootPointNodeId());
        if (node.ExtraInfo() != NULL && node.ExtraInfo()->isActive() && node.ExtraInfo()->EntityType() == type_loot && (static_cast<Trigger_WeaponTeamLoot*>(node.ExtraInfo()))->GetTeam()->GetLootPointNodeId() == pBot->GetTeam()->GetLootPointNodeId()) {
            Distance = Raven_Feature::DistanceToPosition(pBot, pBot->GetTeam()->GetLootPointNodeId());
        }
    }

    //if the distance feature is rated with a value of 1 it means that the
    //item is either not present on the map or too far away to be worth
    //considering, therefore the desirability is zero
    if (Distance == 1)
    {
        return 0;
    }
    else
    {
        //value used to tweak the desirability
        const double Tweaker = 0.15;

        double TotalWeaponStrength  = Raven_Feature::TotalWeaponStrength(pBot);

        double Desirability = (Tweaker * (1 - TotalWeaponStrength)) / Raven_Feature::DistanceToPosition(pBot, pBot->GetTeam()->GetLootPointNodeId());

        //ensure the value is in the range 0 to 1
        Clamp(Desirability, 0, 1);

        Desirability *= m_dCharacterBias;

        return Desirability;
    }
}



//----------------------------- SetGoal ---------------------------------------
//-----------------------------------------------------------------------------
void GetLootGoal_Evaluator::SetGoal(Raven_Bot* pBot)
{
    if (pBot->HasTeam() == false) return;
    pBot->GetBrain()->AddGoal_GetItem(type_loot);
}

//-------------------------- RenderInfo ---------------------------------------
//-----------------------------------------------------------------------------
void GetLootGoal_Evaluator::RenderInfo(Vector2D Position, Raven_Bot* pBot)
{
    std::string s = "LOOT: ";

    gdi->TextAtPos(Position, s + ttos(CalculateDesirability(pBot), 2));
}