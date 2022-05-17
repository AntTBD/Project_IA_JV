#ifndef MESSAGESMANAGER_H
#define MESSAGESMANAGER_H

#include <vector>
#include <string>
#include <iostream>
#include "EntityNames.h"
#include "misc/ConsoleUtils.h"






class MessagesManagerClass {

public:
    class Message{
        public:
            Message(std::string msg, int id, bool isHandleMessage) : messageData(msg), entityID(id), isHandleMessage(isHandleMessage) {}

            int entityID;
            bool isHandleMessage;
            std::string messageData;

            bool operator ==(const Message& rhs) const {
                return (messageData == rhs.messageData && entityID == rhs.entityID && isHandleMessage == rhs.isHandleMessage);
            }
    };


    MessagesManagerClass();
    ~MessagesManagerClass();
    void AddMessage(std::string message, int entityId, bool isHandleMessage = false);
    void ShowAllMessages();
    void RemoveMessage(Message msg);

private:

    std::vector<Message> m_messages;
};

extern MessagesManagerClass MessagesManager;


#endif //MESSAGESMANAGER_H
