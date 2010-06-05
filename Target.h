#include "Game.h"

class Target{
    Ogre::SceneNode * sceneNode;

public: 
    Target(Ogre::SceneNode *);
    ~Target();
    
    Ogre::Vector3 displacement(const Airplane *) const;
};
