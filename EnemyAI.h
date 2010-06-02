#include "AI.h"
#include "Ogre.h"

class Enemy;

class EnemyAI : public AI {
    Ogre::SceneNode * sceneNode;

public:
    void update(Enemy * enemy, Airplane * player, float dt);
    EnemyAI(Ogre::SceneNode *);
};
