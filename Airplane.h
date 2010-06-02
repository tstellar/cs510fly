#include "Game.h"
#include "Object.h"
#include "AirplaneState.h"

class Airplane : public Object {
    AirplaneState state;

    bool thrustInc, thrustDec, pitchInc, pitchDec, rollInc, rollDec, yawInc, yawDec;

    float delay;

    unsigned int alSource;

    Ogre::ParticleSystem * engineParticles;
public:
    static const Ogre::String SCENE_NODE_NAME;

    Airplane(Game *, Ogre::SceneNode *, const AirplaneState&);
    ~Airplane();

    void update(float dt);

    float getThrust() const { return state.thrust; }
    void setThrust(float thrustAmount);
    bool atMaximumThrust() const;
    
    const Ogre::Vector3& getPosition() const { return state.position; }
    const Ogre::Vector3& getVelocity() const { return state.velocity; }
    
    const AirplaneState& getState() const { return state; }

    Ogre::Radian getPitch() const;
    Ogre::Radian getRoll() const;
    Ogre::Radian getYaw() const;

    void increaseThrust();
    void decreaseThrust();
    void pitchUp();
    void pitchDown();
    void rollLeft();
    void rollRight();
    void yawLeft();
    void yawRight();

private:
    Ogre::Vector3 thrust() const;
    Ogre::Vector3 lift() const;
    Ogre::Vector3 drag() const;
    Ogre::Vector3 weight() const;

    Ogre::Vector3 netForce() const;
    
    float liftCoefficient(float aoa) const;
    float dragCoefficient(float aoa) const;

    void updateSound();
};


