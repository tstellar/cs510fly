
#include "AI.h"

class Enemy;

class AirplaneAI : public AI {
    Airplane * airplane;
    public:
    void update(Enemy * enemy, Airplane * player, float dt);
    AirplaneAI(Airplane *);
};
