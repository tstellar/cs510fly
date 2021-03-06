#include "Airplane.h"

#include "Target.h"
#include "World.h"

const Ogre::String Airplane::SCENE_NODE_NAME = "Airplane";

static const float MINIMUM_TIME_STEP = 0.01f;

static const float MASS = 18885.f; // NTO mass of F-15 Eagle in kg
static const float WEIGHT = 184000.0f; // NTO weight of F-15 Eagle in newtons
static const float HEIGHT = 5.63f;  // Height of F-15 Eagle in meters
static const float Y_MIN = 0.5 * HEIGHT;
static const float AIR_DENSITY = 1.2f; // Density of air in kg/m^3 (assumed constant)
static const float PLANFORM_AREA = 56.5; // Planform area of F-15 Eagle in m^2
static const float THRUST_DELTA = 20000.0; // Adjust thrust by 20 kN/s
static const float THRUST_MAX = 77620.0f * 2; // Two engines at 77.62 kN each

/* The following numbers are somewhat invented. FAA regulations consider anything over 10 ft/s (3.3 m/s) a "hard
 * landing" severe enough to require a full inspection, but this as an F-15, not a 747, and we're talking more about
 * how hard a landing would destroy the airframe, not just injure people. Also, AFAICT landing without flaring out -
 * that is, just driving the plane into the runway at a normal 3-degree descent - is stupid and dangerous but *not*
 * necessarily a crash landing in the sense that the plane falls apart. So I'm padding both numbers. */
static const float LANDING_ROD_MAX = 5.0f; // More than 5 m/s is a crash
static const Ogre::Degree LANDING_PITCH_MIN(-5.0f); // More than 5-degree downward pitch attitude is a crash

static const Ogre::Radian ROLL_DELTA(Ogre::Math::HALF_PI/4.0f); // Adjust roll by pi/8 rad/s
static const Ogre::Radian PITCH_DELTA(Ogre::Math::HALF_PI/8.0f); // Adjust pitch by pi/16 rad/s
static const Ogre::Radian YAW_DELTA(Ogre::Math::HALF_PI/8.0F); // Adjust yaw by pi/16 rad/s

Airplane::Airplane(Game * game, Ogre::SceneNode * sceneNode, const AirplaneState& state) :
        Object(game, sceneNode),
        delay(0.0f),
        state(state),
        crashed(false),
        engineOn(true),
        thrustInc(false), thrustDec(false), pitchInc(false), pitchDec(false),
        rollInc(false), rollDec(false), yawInc(false), yawDec(false) {
    this->state.clampAboveHeight(Y_MIN);
    this->state.syncToNode(sceneNode);
        
    ALenum error;
    /* Setup Sound */
    alGenSources(1, &alSource);
    if((error = alGetError()) != AL_NO_ERROR){
        fprintf(stderr, "Error creating OpenAL source. %d\n",error);
    }
    alSourcei(alSource, AL_BUFFER, game->getMotorBuffer());
    if((error = alGetError()) != AL_NO_ERROR){
        fprintf(stderr, "Error attching motor buffer to source. %d\n",error);
    }
    alSourcei(alSource, AL_LOOPING, AL_TRUE);
    alSourcePlay(alSource);
    
    /* Create particle systems */
    engineParticles = sceneNode->getCreator()->createParticleSystem(sceneNode->getName() + "Engine", "Engine");
    sceneNode->attachObject(engineParticles);
}

Airplane::~Airplane() { }

void Airplane::setThrust(float thrustAmount) {
    state.thrust = Ogre::Math::Clamp(thrustAmount, 0.0f, THRUST_MAX);
}

bool Airplane::atMaximumThrust() const {
    return state.thrust == THRUST_MAX;
}

Ogre::Vector3 Airplane::thrust() const {
    return state.thrust * (state.orientation * Ogre::Vector3::NEGATIVE_UNIT_Z);
}

Ogre::Vector3 Airplane::lift() const {
    const float aoa = state.angleOfAttack().valueDegrees();
    const float velSquared = state.velocity.squaredLength();
    const float cl = liftCoefficient(aoa);
    
    const Ogre::Vector3 liftDir = state.orientation * Ogre::Vector3::UNIT_Y;
    
    return 0.5f * AIR_DENSITY * PLANFORM_AREA * velSquared * cl * liftDir;
}

float Airplane::liftCoefficient(float aoa) {
    // See docs/lift-coefficient.numbers
    
    if (aoa <= 7.0f)
        return 0.11f * aoa;
    else if (aoa < 15.0f)
        return aoa * (-0.0159f * aoa + 0.3581f) - 0.9474;
    else
        return -0.02f * aoa + 1.15;
}

Ogre::Vector3 Airplane::weight() const {
    return WEIGHT * Ogre::Vector3::NEGATIVE_UNIT_Y;
}

Ogre::Vector3 Airplane::drag() const {
    const float aoa = state.angleOfAttack().valueDegrees();
    const float velSquared = state.velocity.squaredLength();
    const float cd = dragCoefficient(aoa);
    
    const Ogre::Vector3 dragDir = -state.velocity.normalisedCopy();
    
    return 0.5f * AIR_DENSITY * PLANFORM_AREA * velSquared * cd * dragDir;
}

float Airplane::dragCoefficient(float aoa) {
    // See docs/drag-coefficient.numbers
    
    const float cl = liftCoefficient(aoa);
    return cl * (0.2837 * cl - 0.0693) + 0.0217;
}

Ogre::Vector3 Airplane::netForce() const {
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
void Airplane::yawLeft() { yawInc = true; }
void Airplane::yawRight() { yawDec = true; }

void Airplane::update(float dt) {
    if (crashed || dt == 0.0)
        return;

    if(engineOn){
    
        if (thrustInc) {
            state.thrust += THRUST_DELTA * dt;
            if (state.thrust > THRUST_MAX)
                state.thrust = THRUST_MAX;
            thrustInc = false;
        }
        if (thrustDec) {
            state.thrust -= THRUST_DELTA * dt;
            if (state.thrust < 0.0f)
                state.thrust = 0.0f;
            thrustDec = false;
        }
    }
    if (pitchInc) {
        state.orientation = state.orientation * Ogre::Quaternion(PITCH_DELTA * dt, Ogre::Vector3::UNIT_X);
        pitchInc = false;
    }
    if (pitchDec) {
        state.orientation = state.orientation * Ogre::Quaternion(PITCH_DELTA * dt, Ogre::Vector3::NEGATIVE_UNIT_X);
        pitchDec = false;
    }
    if (rollInc) {
        state.orientation = Ogre::Quaternion(ROLL_DELTA * dt, Ogre::Vector3::NEGATIVE_UNIT_Z) * state.orientation;
        rollInc = false;
    }
    if (rollDec) {
        state.orientation = Ogre::Quaternion(ROLL_DELTA * dt, Ogre::Vector3::UNIT_Z) * state.orientation;
        rollDec = false;
    }
    if (yawInc) {
        state.orientation = state.orientation * Ogre::Quaternion(YAW_DELTA * dt, Ogre::Vector3::UNIT_Y);
        yawInc = false;
    }
    if (yawDec) {
        state.orientation = state.orientation * Ogre::Quaternion(YAW_DELTA * dt, Ogre::Vector3::NEGATIVE_UNIT_Y);
        yawDec = false;
    }

    dt += delay;
    if (dt < MINIMUM_TIME_STEP) {
        delay = dt;
        return;
    }
    delay = 0.0f;
    
    const bool initiallyOnGround = (state.position.y == Y_MIN);

    Ogre::Vector3 absoluteNetForce = netForce();

    // Hi, Newton!
    Ogre::Vector3 acceleration = absoluteNetForce / MASS;

    // Velocity Verlet integration
    
    // Step 1: Calculate next position
    state.position += dt * state.velocity + 0.5f * dt * dt * acceleration;

    // Step 2: Calculate velocity a *half*-step from now
    const Ogre::Vector3 halfStepVelocity = state.velocity + 0.5f * dt * acceleration;

    // Step 3a: Project velocity forward another smidgen by Euler method
    state.velocity += dt * acceleration;

    // Step 3b: Re-evaluate acceleration in new state
    absoluteNetForce = netForce();
    acceleration = absoluteNetForce / MASS;

    // Step 4: Calculate new velocity from half-step velocity and new acceleration
    state.velocity = halfStepVelocity + 0.5f * dt * acceleration;

    // TODO We can get away with not normalizing orientation every frame if need be
    state.orientation.normalise();

    if (!initiallyOnGround)
        checkGroundCollision();

    state.syncToNode(sceneNode);
    state.syncToALSource(alSource);
}

void Airplane::checkGroundCollision() {
    if (state.position.y < Y_MIN) {
        if (state.velocity.y < -LANDING_ROD_MAX || getPitch() < LANDING_PITCH_MIN)
            crash();
        else
            land();
        
        state.position.y = Y_MIN;
        state.velocity.y = 0.0f;
    }
}

void Airplane::crash() {
    
    Ogre::ParticleSystem * smokeParticles = sceneNode->getCreator()->createParticleSystem(sceneNode->getName() + "Smoke", "Smoke");
    sceneNode->attachObject(smokeParticles);
    crashed = true;
    if (this == game->getAirplane())
        game->lose();
    alSourceStop(alSource);

}

void Airplane::land() {
    if (game->getWorld()->getTarget()->inRange(this))
        game->win();
}

void Airplane::stopEngine() {
    if(!engineOn){
        return;
    }
    sceneNode->detachObject(engineParticles);
    Ogre::ParticleSystem * explosion = sceneNode->getCreator()->createParticleSystem(sceneNode->getName() + "Explosion", "Explosion");
    sceneNode->attachObject(explosion);
    engineOn = false;
    setThrust(0.0f);
    alSourceStop(alSource);
}

Ogre::Radian Airplane::getPitch() const { return state.orientation.getPitch(false); }
Ogre::Radian Airplane::getRoll() const { return state.orientation.getRoll(false); }
Ogre::Radian Airplane::getYaw() const { return state.orientation.getYaw(false); }
