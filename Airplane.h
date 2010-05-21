#include "Game.h"

class Airplane {
    Game * const game;
    Ogre::SceneNode * const sceneNode;
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

    float getPitch();
    float getRoll();
    float getYaw();

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
};


