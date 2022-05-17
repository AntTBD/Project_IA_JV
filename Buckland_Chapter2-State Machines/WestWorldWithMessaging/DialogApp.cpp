#include "DialogApp.h"
std::ofstream os;

DialogApp::OneEntity::OneEntity(BaseGameEntity* pBaseGameEntity, int numberOfCalls) : baseGameEntity(pBaseGameEntity), numberOfCalls(numberOfCalls)
{
    this->thread = new std::thread(&DialogApp::OneEntity::UpdateBaseGameEntity, this);
    this->isUpdated = false;
    this->isAlive = true;

    //register them with the entity manager
    EntityMgr->RegisterEntity(this->baseGameEntity);
}

DialogApp::OneEntity::~OneEntity() {
    delete this->baseGameEntity;
    delete this->thread;
}

void DialogApp::OneEntity::UpdateBaseGameEntity(){
    this->isAlive = true;
    for(int i = 0; i < this->numberOfCalls; i++){
        // wait all updates to continue
        while (this->isUpdated) {}

        this->baseGameEntity->Update();
        this->isUpdated = true;

        std::this_thread::sleep_for(std::chrono::milliseconds(800));
    }
    this->isAlive = false;
    std::cout << "\n" << GetNameOfEntity(this->baseGameEntity->ID()) << " has ended !";
}

DialogApp::DialogApp(int numberOfCalls) : numberOfCalls(numberOfCalls) 
{

//define this to send output to a text file (see locations.h)
#ifdef TEXTOUTPUT
    os.open("output.txt");
#endif

    //seed random number generator
    srand((unsigned) time(NULL));

    //create a miner
    this->allEntities.push_back(new OneEntity(new Miner(ent_Miner_Bob),this->numberOfCalls));
    Sleep(400);
    //create his wife
    this->allEntities.push_back(new OneEntity(new MinersWife(ent_Elsa),this->numberOfCalls));
    Sleep(400);
    //create drunk man
    this->allEntities.push_back(new OneEntity(new DrunkMan(ent_Drunk_Claudius),this->numberOfCalls));
    Sleep(400);

    for (OneEntity* entity : this->allEntities) {
        this->threads.push_back(entity->thread);
    }

    this->threads.push_back(new std::thread(&DialogApp::SyncAllEntities, this));
    for(std::thread* thread : this->threads) {
        thread->join();
        Sleep(400);
    }
}
DialogApp::~DialogApp() {

    //tidy up
    for (OneEntity* entity : this->allEntities) {
        this->threads.erase(std::find(this->threads.begin(), this->threads.end(), entity->thread));// delete entity will delete associated thread so we can remove it from list to avoid a double delete
        delete entity;
    }
    for (std::thread* thread : this->threads) {
        delete thread;
    }
}

bool DialogApp::IsAllUpdated(){
    for(auto &entity : this->allEntities){
        if(entity->isUpdated == false){
            return false;
        }
    }
    return true;
}

bool DialogApp::IsAllFinish(){
    for(auto &entity : this->allEntities){
        if(entity->isAlive == true){
            return false;
        }
    }
    return true;
}

void DialogApp::SyncAllEntities() {

    while (true && this->allEntities.empty() == false)
    {
        if (this->IsAllFinish())
        {
            std::cout << "\n\n\tAll entities have ended\n\n" << std::endl;
            break;
        }

        while(!this->IsAllUpdated() && !this->IsAllFinish())
        {
            // wait all updates to finish
            std::this_thread::sleep_for(std::chrono::milliseconds(400));
        }

        // dispatch any delayed messages
        Dispatch->DispatchDelayedMessages();

        MessagesManager.ShowAllMessages();

        // reset all entities
        for(auto &entity : this->allEntities){
            if (entity->isAlive) {
                entity->isUpdated = false;
            }
        }

    }

    //wait for a keypress before exiting
    PressAnyKeyToContinue();
}