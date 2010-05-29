#include "Game.h"
#include "Object.h"

class Airplane : public Object {
    Ogre::Vector3 position;
    Ogre::Quaternion orientation;
    Ogre::Vector3 velocity;

    float thrustAmount;

    bool thrustInc, thrustDec, pitchInc, pitchDec, rollInc, rollDec, yawInc, yawDec;

    float delay;

public:
    static const Ogre::String SCENE_NODE_NAME;

    Airplane(Game *, Ogre::SceneNode *);
    ~Airplane();

    void update(float dt);

    float getThrust() { return thrustAmount; }
    void setThrust(float thrustAmount) { this->thrustAmount = thrustAmount; }
    const Ogre::Vector3& getPosition() const { return position; }
    const Ogre::Vector3& getVelocity() const { return velocity; }

    Ogre::Radian getPitch();
    Ogre::Radian getRoll();
    Ogre::Radian getYaw();

    void increaseThrust();
    void decreaseThrust();
    void pitchUp();
    void pitchDown();
    void rollLeft();
    void rollRight();
    void yawLeft();
    void yawRight();

private:
    Ogre::Vector3 thrust();
    Ogre::Vector3 lift();
    Ogre::Vector3 drag();
    Ogre::Vector3 weight();

    Ogre::Vector3 netForce();
    
    float liftCoefficient(float aoa);
    float dragCoefficient(float aoa);
};


