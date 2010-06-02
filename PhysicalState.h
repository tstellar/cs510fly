#ifndef PHYSICALSTATE_H
#define PHYSICALSTATE_H

#include "Game.h"

struct PhysicalState {
    Ogre::Vector3 position;
    Ogre::Quaternion orientation;
    Ogre::Vector3 velocity;
    
    PhysicalState() :
        position(Ogre::Vector3::ZERO),
        orientation(Ogre::Quaternion::IDENTITY),
        velocity(Ogre::Vector3::ZERO) { }
    PhysicalState(Ogre::Vector3 position, Ogre::Quaternion orientation, Ogre::Vector3 velocity) :
        position(position),
        orientation(orientation),
        velocity(velocity) { }
    PhysicalState(const PhysicalState& other) :
        position(other.position),
        orientation(other.orientation),
        velocity(other.velocity) { }
    explicit PhysicalState(const Ogre::Node * node) :
        position(node->getPosition()),
        orientation(node->getOrientation()),
        velocity(Ogre::Vector3::ZERO) { }
    
    static PhysicalState readFromConfig(const Ogre::ConfigFile::SettingsMultiMap * settings);
    
    void syncToNode(Ogre::Node *) const;
    void syncFromNode(const Ogre::Node *);

    void syncToALSource(unsigned int alSource);
};

#endif
