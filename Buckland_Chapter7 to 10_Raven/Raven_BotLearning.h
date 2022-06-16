#ifndef RAVEN_BOTLEARNING_H
#define RAVEN_BOTLEARNING_H

#include "Raven_Bot.h"
#include "neuralNetwork/CData.h"
#include "neuralNetwork/CNeuralNet.h"
#include <chrono>

class Raven_BotLearning: public Raven_Bot {

    private:
        CNeuralNet m_ModeleAppris;
        std::chrono::steady_clock::time_point timer;

    public:
        Raven_BotLearning(Raven_Game *world, Vector2D pos);
        ~Raven_BotLearning();

        void SetAlive();
        void SetDead();

        void Update();
        void Render();
};


#endif //RAVEN_BOTLEARNING_H
