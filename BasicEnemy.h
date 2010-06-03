#include "Game.h"

class Airplane;

class BasicEnemy : public Enemy{

    Ogre::SceneNode * sceneNode;
    float velocity;
    int difficulty;
    unsigned int alSource;

public:
    BasicEnemy(Game *, Ogre::SceneNode *, const Ogre::Vector3&, Ogre::String);
    ~BasicEnemy();
    
    const Ogre::Vector3& getPosition() { return sceneNode->getPosition(); }
    void update(Airplane * player , float dt);

};
