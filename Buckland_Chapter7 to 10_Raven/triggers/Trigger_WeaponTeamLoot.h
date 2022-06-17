#ifndef TRIGGER_WEAPONTEAMLOOT_H
#define TRIGGER_WEAPONTEAMLOOT_H

#include "Triggers/Trigger.h"
#include "../Raven_Bot.h"
#include "../Raven_Team.h"
#include "../armory/Raven_Weapon.h"
#include "../Raven_Game.h"


class Trigger_WeaponTeamLoot : public Trigger<Raven_Bot>
{
private :

    // the team that will receive the weapons
    Raven_Team* m_Team;
    int m_weaponType;
    int m_ammoCurrentWeaponType;
    Raven_Game* m_pWorld;


    //vrtex buffers for rocket shape
    std::vector<Vector2D>         m_vecRLVB;
    std::vector<Vector2D>         m_vecRLVBTrans;

public :
    // if undifined team, set it to NULL
    Trigger_WeaponTeamLoot(Vector2D pos, Raven_Team* team, Raven_Weapon* weapon, Raven_Game* world, int node_idx);

    void Update();
    void Try(Raven_Bot* pBot);

    void Render();

    Raven_Team* GetTeam(){ return m_Team; }
};


#endif //TRIGGER_WEAPONTEAMLOOT_H
