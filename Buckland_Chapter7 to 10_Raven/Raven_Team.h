#ifndef RAVEN_TEAM_H
#define RAVEN_TEAM_H

#include <vector>
#include <iosfwd>
#include <map>
#include <list>

#include "game/MovingEntity.h"
#include "misc/utils.h"
#include "Raven_TargetingSystem.h"

class Raven_Bot;



class Raven_Team {
    static int Raven_Team_NextTeamID;
private:
    int m_id;
    std::string m_name;
    std::vector<Raven_Bot*> m_members;
    Raven_Bot* m_leader;
    Raven_Bot* m_target;

    Vector2D m_lootPoint;
    std::vector<Vector2D> m_spawnPoints;

    bool IsMember(Raven_Bot *bot);
public:


    Raven_Team(std::string name, Vector2D lootPoint);
    ~Raven_Team();

    void AddMember(Raven_Bot* bot);
    void RemoveMember(Raven_Bot* bot);
    void SetLeader(Raven_Bot* bot);
    void SetTarget(Raven_Bot* target, int id_bot);
    void ClearTarget(int id_bot);

    int ID() const { return m_id; }
    std::string GetName();
    Raven_Bot* GetLeader();
    Raven_Bot* GetTarget();
    std::vector<Raven_Bot*> GetMembers();
    bool IsTarget(int id);

    Vector2D GetLootPoint();

    void AddSpawnPoint(Vector2D point);
    const std::vector<Vector2D>& GetSpawnPoints() const;
    Vector2D GetRandomSpawnPoint();


    bool operator == (const Raven_Team& rhs);
};


#endif //RAVEN_TEAM_H
