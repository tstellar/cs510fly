#include "World.h"

class Airplane {
  World * const world;
  Ogre::SceneNode * const sceneNode;
  Ogre::Vector3 position;
  Ogre::Quaternion orientation;
  Ogre::Vector3 velocity;
  
  float thrustAmount;
  
  bool thrustInc, thrustDec, pitchInc, pitchDec, rollInc, rollDec;
  
 public:
  Airplane(World *, Ogre::SceneNode *);
  ~Airplane();
  
  void update(float dt);
  float getThrust();
  float getPitch();
  float getRoll();
  float getYaw();

  void increaseThrust();
  void decreaseThrust();
  void pitchUp();
  void pitchDown();
  void rollLeft();
  void rollRight();
  
 private:
  Ogre::Vector3 thrust();
  Ogre::Vector3 lift();
  Ogre::Vector3 drag();
  Ogre::Vector3 weight();
  
  Ogre::Vector3 netForce();
};


