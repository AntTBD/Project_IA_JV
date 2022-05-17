#include "MessagesManager.h"

MessagesManagerClass MessagesManager;

MessagesManagerClass::MessagesManagerClass(){}
MessagesManagerClass::~MessagesManagerClass(){
    this->m_messages.clear();
}
void MessagesManagerClass::AddMessage(std::string message, int entityId, bool isHandleMessage){
    m_messages.push_back(Message(message, entityId, isHandleMessage));
}
void MessagesManagerClass::ShowAllMessages() {
    std::vector<Message> tempMessages = this->m_messages;

    for(Message msg : tempMessages) {
        if(msg.isHandleMessage) {
            SetTextColor(BACKGROUND_RED|FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);
            std::cout << msg.messageData;
        }else{
            switch (msg.entityID) {
                case ent_Miner_Bob:
                    SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
                    break;
                case ent_Elsa:
                    SetTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                    break;
                case ent_Drunk_Claudius:
                    SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
                    break;
            }
            std::cout << "\n" + GetNameOfEntity(msg.entityID) << ": " << msg.messageData;
        }

        this->RemoveMessage(msg);
    }
}
void MessagesManagerClass::RemoveMessage(Message msg){
    this->m_messages.erase(std::find(this->m_messages.begin(), this->m_messages.end(), msg));
}