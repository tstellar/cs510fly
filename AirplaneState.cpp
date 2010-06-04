#include "Game.h"
#include "AirplaneState.h"
#include "ConfigReader.h"

static const Ogre::String
    POSITION_NAME = "Position",
    ORIENTATION_NAME = "Orientation",
    VELOCITY_NAME = "Velocity",
    THRUST_NAME = "Thrust";

static Ogre::Quaternion parsePitchRollYaw(const Ogre::String&);

AirplaneState AirplaneState::readFromConfig(const ConfigReader * reader) {
    Ogre::Vector3 position = reader->parse(POSITION_NAME, &Ogre::StringConverter::parseVector3, Ogre::Vector3::ZERO);
    Ogre::Quaternion orientation = reader->parse(ORIENTATION_NAME, &parsePitchRollYaw, Ogre::Quaternion::IDENTITY);
    Ogre::Vector3 velocity = reader->parse(VELOCITY_NAME, &Ogre::StringConverter::parseVector3, Ogre::Vector3::ZERO);
    Ogre::Real thrust = reader->parse(THRUST_NAME, &Ogre::StringConverter::parseReal, 0.0f);
    
    return AirplaneState(position, orientation, velocity, thrust);
}

static Ogre::Quaternion parsePitchRollYaw(const Ogre::String& str) {
    const Ogre::Vector3& vec = Ogre::StringConverter::parseVector3(str);
    const float pitch = vec.x, roll = vec.y, yaw = vec.z;
    return
        Ogre::Quaternion(Ogre::Degree(roll),    Ogre::Vector3::NEGATIVE_UNIT_Z) *
        Ogre::Quaternion(Ogre::Degree(pitch),   Ogre::Vector3::UNIT_X) *
        Ogre::Quaternion(Ogre::Degree(yaw),     Ogre::Vector3::UNIT_Y);
}

void AirplaneState::syncToNode(Ogre::Node * node) const {
    node->setPosition(position);
    node->setOrientation(orientation);
}

void AirplaneState::syncFromNode(const Ogre::Node * node) {
    position = node->getPosition();
    orientation = node->getOrientation();
}

bool AirplaneState::clampAboveHeight(float height) {
    if (position.y < height) {
        position.y = height;
        return true;
    } else
        return false;
}

Ogre::Radian AirplaneState::angleOfAttack() const {
    // N.B.: Do NOT use Ogre::Vector3::angleBetween(). It only returns values from 0 to pi. Which is pretty $!#$@!#$
    // useless for our purposes.
    
    const Ogre::Vector3& localVelocity = orientation.Inverse() * velocity;
    return -Ogre::Radian(atan2(localVelocity.y, -localVelocity.z));
}

void AirplaneState::syncToALSource(unsigned int alSource) {
    alSource3f(alSource, AL_POSITION, position.x, position.y, position.z);
    alSource3f(alSource, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
}
