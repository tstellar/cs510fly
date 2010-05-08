#include "Airplane.h"

const Ogre::String Airplane::SCENE_NODE_NAME = "Airplane";

static const float MASS = 18885.f; // NTO mass of F-15 Eagle in kg
static const float WEIGHT = 184000.0f; // NTO weight of F-15 Eagle in newtons
static const float HEIGHT = 5.63f;  // Height of F-15 Eagle in meters.
static const float THRUST_DELTA = 5000.0; // Adjust thrust by 1 kN
static const Ogre::Radian ROLL_DELTA = Ogre::Radian(Ogre::Math::HALF_PI/4.0f); // Adjust roll by pi/16
static const Ogre::Radian PITCH_DELTA = Ogre::Radian(Ogre::Math::HALF_PI/2.0f); // Adjust pitch by pi/8

Airplane::Airplane(World * world, Ogre::SceneNode * sceneNode) :
    world(world), sceneNode(sceneNode),
    position(sceneNode->getPosition() + Ogre::Vector3(0.0f, HEIGHT, 0.0f)),
    orientation(sceneNode->getOrientation()),
    velocity(Ogre::Vector3::ZERO), thrustAmount(0.0f),
    thrustInc(false), thrustDec(false), pitchInc(false), pitchDec(false), rollInc(false), rollDec(false) { 
    }

Airplane::~Airplane() { }

Ogre::Vector3 Airplane::thrust() {
  return thrustAmount * Ogre::Vector3::NEGATIVE_UNIT_Z;
}

Ogre::Vector3 Airplane::lift() {
  // Cheating for now: In level flight, |lift| = |weight|, but lift isn't pointed straight up if we're turning.
  return WEIGHT * Ogre::Vector3::UNIT_Y;
}

Ogre::Vector3 Airplane::weight() {
  return WEIGHT * (orientation.Inverse() * Ogre::Vector3::NEGATIVE_UNIT_Y);
}

Ogre::Vector3 Airplane::drag() {
  // We'll be physics people for the moment: No friction.
  return Ogre::Vector3::ZERO;
}

Ogre::Vector3 Airplane::netForce() {
  // Separate these out for ease of debugging
  
  Ogre::Vector3
    thrust = this->thrust(),
    lift = this->lift(),
    drag = this->drag(),
    weight = this->weight();
  return thrust + lift + drag + weight;
}

void Airplane::increaseThrust() { thrustInc = true; }
void Airplane::decreaseThrust() { thrustDec = true; }
void Airplane::pitchUp() { pitchInc = true; }
void Airplane::pitchDown() { pitchDec = true; }
void Airplane::rollLeft() { rollDec = true; }
void Airplane::rollRight() { rollInc = true; }

void Airplane::update(float dt) {
  if (thrustInc) {
    thrustAmount += THRUST_DELTA * dt;
    thrustInc = false;
  }
  if (thrustDec) {
    thrustAmount -= THRUST_DELTA * dt;
    thrustDec = false;
  }
  if (pitchInc) {
    orientation = Ogre::Quaternion(PITCH_DELTA * dt, Ogre::Vector3::UNIT_X) * orientation;
    pitchInc = false;
  }
  if (pitchDec) {
    orientation = Ogre::Quaternion(PITCH_DELTA * dt, Ogre::Vector3::NEGATIVE_UNIT_X) * orientation;
    pitchDec = false;
  }
  if (rollInc) {
    orientation = Ogre::Quaternion(ROLL_DELTA * dt, Ogre::Vector3::NEGATIVE_UNIT_Z) * orientation;
    rollInc = false;
  }
  if (rollDec) {
    orientation = Ogre::Quaternion(ROLL_DELTA * dt, Ogre::Vector3::UNIT_Z) * orientation;
    rollDec = false;
  }
  
  Ogre::Vector3 relativeNetForce = netForce();
  Ogre::Vector3 absoluteNetForce = orientation.Inverse() * relativeNetForce;
   
  // Hi, Newton!
  velocity += dt * absoluteNetForce / MASS;
  
  position += dt * velocity;
  
  // Orient toward the direction we're heading (we assume a perfect rudder)
  // TODO (can't quite get it right ...)

  sceneNode->setOrientation(orientation);
  sceneNode->setPosition(position);

}





