#ifndef AIRPLANESTATE_H
#define AIRPLANESTATE_H

#include "Ogre.h"

class ConfigReader;

struct AirplaneState {
    Ogre::Vector3 position;
    Ogre::Quaternion orientation;
    Ogre::Vector3 velocity;
    float thrust;
    
    AirplaneState(
            const Ogre::Vector3& position = Ogre::Vector3::ZERO,
            const Ogre::Quaternion& orientation = Ogre::Quaternion::IDENTITY,
            const Ogre::Vector3& velocity = Ogre::Vector3::ZERO,
            float thrust = 0.0f) :
        position(position),
        orientation(orientation),
        velocity(velocity),
        thrust(thrust) { }
    AirplaneState(const AirplaneState& other) :
        position(other.position),
        orientation(other.orientation),
        velocity(other.velocity),
        thrust(other.thrust) { }
    explicit AirplaneState(const Ogre::Node * node) :
        position(node->getPosition()),
        orientation(node->getOrientation()),
        velocity(Ogre::Vector3::ZERO),
        thrust(0.0f) { }
    
    static AirplaneState readFromConfig(const ConfigReader * reader);
    
    void syncToNode(Ogre::Node *) const;
    void syncFromNode(const Ogre::Node *);
    
    void syncToALSource(unsigned int alSource);
    
    bool clampAboveHeight(float height);
    
    Ogre::Radian angleOfAttack() const;
};

#endif
