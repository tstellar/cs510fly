#include "Game.h"

class Airplane;

class Enemy{
protected:
    Game * const game;
public:
    Enemy(Game * const game);
    virtual void update(Airplane * player, float dt) = 0;


private:
    virtual const Ogre::Vector3& getPosition() = 0;

protected:
    bool posBelow(const Ogre::Vector3& pos);
    bool posLeft(const Ogre::Vector3& pos);
    bool posInFront(const Ogre::Vector3& pos);
};
