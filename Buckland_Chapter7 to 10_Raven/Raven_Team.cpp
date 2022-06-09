#include "Raven_Team.h"
#include "Messaging/Telegram.h"
#include "Raven_Messages.h"
#include "Messaging/MessageDispatcher.h"
#include "Raven_Bot.h"

int Raven_Team::Raven_Team_NextTeamID = 0;

Raven_Team::Raven_Team(std::string name, std::pair<Vector2D,int> lootPoint){
    m_name = name;
    m_lootPoint = lootPoint;
    m_spawnPoints = std::vector<Vector2D>();
    m_id = Raven_Team_NextTeamID;
    Raven_Team_NextTeamID++;
    m_target = NULL;
    m_leader = NULL;
}

Raven_Team::~Raven_Team(){

}

void Raven_Team::AddMember(Raven_Bot *bot) {

    if(!IsMember(bot)){
        if(m_members.empty()){
            SetLeader(bot);
        }

        m_members.push_back(bot);
    }
}

void Raven_Team::RemoveMember(Raven_Bot *bot) {
    m_members.erase(std::remove(m_members.begin(), m_members.end(), bot), m_members.end());
}

bool Raven_Team::IsMember(Raven_Bot *bot) {
    return std::find(m_members.begin(), m_members.end(), bot) != m_members.end();
}

void Raven_Team::SetLeader(Raven_Bot *bot) {
    m_leader = bot;
}

void Raven_Team::SetTarget(Raven_Bot *target, int id_bot) {
    m_target = target;

    for (Raven_Bot* bot: m_members) {
        Dispatcher->DispatchMsg(SEND_MSG_IMMEDIATELY,
                                id_bot,
                                bot->ID(),
                                Msg_NewTeamTarget,
                                NO_ADDITIONAL_INFO);
    }
}

void Raven_Team::ClearTarget(int id_bot) {
    m_target = NULL;
    for (Raven_Bot* bot: m_members) {
        Dispatcher->DispatchMsg(SEND_MSG_IMMEDIATELY,
                                id_bot,
                                bot->ID(),
                                Msg_TeamTargetDown,
                                NO_ADDITIONAL_INFO);
    }
}

Raven_Bot* Raven_Team::GetLeader(){
    return m_leader;
}
Raven_Bot* Raven_Team::GetTarget(){
    return m_target;
}
std::string Raven_Team::GetName(){
    return m_name;
}
std::vector<Raven_Bot*> Raven_Team::GetMembers(){
    return m_members;
}

bool Raven_Team::IsTarget(int id){
    if(m_target != NULL){
        return m_target->ID() == id;
    }
    return false;
}

Vector2D Raven_Team::GetLootPoint(){
    return m_lootPoint.first;
}
int Raven_Team::GetLootPointNodeId() {
    return m_lootPoint.second;
}

void Raven_Team::AddSpawnPoint(Vector2D point){
    m_spawnPoints.push_back(point);
}
const std::vector<Vector2D>& Raven_Team::GetSpawnPoints() const{
    return m_spawnPoints;
}
Vector2D Raven_Team::GetRandomSpawnPoint(){
    if(m_spawnPoints.empty()){
        return Vector2D(0,0);
    }
    return m_spawnPoints[RandInt(0, m_spawnPoints.size() - 1)];
}

bool Raven_Team::operator == (const Raven_Team& rhs){
    return m_id == rhs.m_id;
}
