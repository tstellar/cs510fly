#include "Game.h"

class Object {
protected:
    Game * const game;
    Ogre::SceneNode * const sceneNode;

public:
    Object(Game * game, Ogre::SceneNode * sceneNode) :
            game(game), sceneNode(sceneNode) { }
    virtual ~Object() { }

    virtual void update(float dt) { }
};
