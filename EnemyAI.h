#include "AI.h"
#include "Ogre.h"

class Enemy;

class EnemyAI : public AI {
    Ogre::SceneNode * sceneNode;
    float velocity;
    int difficulty;

public:
    EnemyAI(Ogre::SceneNode *);
    void update(Enemy * enemy, Airplane * player, float dt);
};
