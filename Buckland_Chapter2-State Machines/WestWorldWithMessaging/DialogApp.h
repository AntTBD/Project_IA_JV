#ifndef DIALOGAPP_H
#define DIALOGAPP_H

#include <fstream>
#include <time.h>

#include "Locations.h"
#include "Miner.h"
#include "MinersWife.h"
#include "DrunkMan.h"
#include "EntityManager.h"
#include "MessageDispatcher.h"
#include "misc/ConsoleUtils.h"
#include "EntityNames.h"
#include "MessagesManager.h"
#include <thread>



class DialogApp {
public:
    class OneEntity{
    public:
        OneEntity(BaseGameEntity* pBaseGameEntity, int numberOfCalls);
        ~OneEntity();
        void UpdateBaseGameEntity();
        BaseGameEntity* baseGameEntity;
        std::thread* thread;
        int numberOfCalls;
        bool isUpdated;
        bool isAlive;
    };


    DialogApp(int numberOfCalls);
    ~DialogApp();

    //void UpdateBaseGameEntity(OneEntity* pOneEntity);
    bool IsAllUpdated();
    bool IsAllFinish();
    void SyncAllEntities();

    int numberOfCalls;
    std::vector<OneEntity*> allEntities;
    std::vector<std::thread*> threads;
};


#endif //DIALOGAPP_H
