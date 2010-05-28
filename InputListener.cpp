// InputListener.cpp
// @author Eric D. Wills

#include "Airplane.h"
#include "Level.h"
#include "InputListener.h"
#include "Game.h"

InputListener::InputListener(Game* game, Ogre::RenderWindow * renderWindow) :
    game(game), renderWindow(renderWindow), 
    forwardKeyDown(false), backKeyDown(false), upKeyDown(false), downKeyDown(false),
    leftKeyDown(false), rightKeyDown(false), yawLeftKeyDown(false), yawRightKeyDown(false),
    shutdownKeyPressed(false), breakKeyPressed(false) {
        size_t windowHandle;
        renderWindow->getCustomAttribute("WINDOW", &windowHandle);

        inputManager = OIS::InputManager::createInputSystem(windowHandle);

        mouse = static_cast<OIS::Mouse*>(inputManager->createInputObject(OIS::OISMouse, true));
        mouse->setEventCallback(this);
        mouse->capture();

        keyboard = static_cast<OIS::Keyboard*>(inputManager->createInputObject(OIS::OISKeyboard, true));
        keyboard->setEventCallback(this);
        keyboard->capture();
    }

InputListener::~InputListener() {
    inputManager->destroyInputObject(mouse);
    inputManager->destroyInputObject(keyboard);
    OIS::InputManager::destroyInputSystem(inputManager);
}

bool InputListener::frameStarted(const Ogre::FrameEvent& event) {
    if (shutdownKeyPressed || renderWindow->isClosed()){
        return false;
    }

    mouse->capture();
    keyboard->capture();

    Airplane * const airplane = game->getAirplane();

    if (forwardKeyDown)
        airplane->increaseThrust();
    if (backKeyDown)
        airplane->decreaseThrust();
    if (leftKeyDown)
        airplane->rollLeft();
    if (rightKeyDown)
        airplane->rollRight();
    if (yawLeftKeyDown)
        airplane->yawLeft();
    if (yawRightKeyDown)
        airplane->yawRight();
    if (upKeyDown)
        airplane->pitchDown();
    if (downKeyDown)
        airplane->pitchUp();
    
    if (breakKeyPressed)
        // Set breakpoint here
        breakKeyPressed = false;
    
    game->update(event.timeSinceLastFrame);

    return true;
}

bool InputListener::frameEnded(const Ogre::FrameEvent& event) { return true; }

bool InputListener::mousePressed(const OIS::MouseEvent& event, OIS::MouseButtonID buttonID) { return true; }
bool InputListener::mouseReleased(const OIS::MouseEvent& event, OIS::MouseButtonID buttonID) { return true; }

bool InputListener::mouseMoved(const OIS::MouseEvent& event) {
    //  cameraPitchIncrement = event.state.Y.rel;
    //  cameraYawIncrement= event.state.X.rel;

    return true;
}

bool InputListener::keyPressed(const OIS::KeyEvent& event) {
    switch (event.key) {
        case OIS::KC_W:
            forwardKeyDown = true;
        break;
        case OIS::KC_S:
            backKeyDown = true;
        break;
        case OIS::KC_LEFT:
            leftKeyDown = true;
        break;
        case OIS::KC_RIGHT:
            rightKeyDown = true;
        break;
        case OIS::KC_A:
            yawLeftKeyDown = true;
        break;
        case OIS::KC_D:
            yawRightKeyDown = true;
        break;
        case OIS::KC_UP:
            upKeyDown = true;
        break;
        case OIS::KC_DOWN:
            downKeyDown = true;
        break;
        case OIS::KC_ESCAPE:
            shutdownKeyPressed = true;
        case OIS::KC_B:
            breakKeyPressed = true;
        break;
    }

    return true;
}

bool InputListener::keyReleased(const OIS::KeyEvent& event) {
    switch (event.key) {
        case OIS::KC_W:
            forwardKeyDown = false;
        break;
        case OIS::KC_S:
            backKeyDown = false;
        break;
        case OIS::KC_LEFT:
            leftKeyDown = false;
        break;
        case OIS::KC_RIGHT:
            rightKeyDown = false;
        break;
        case OIS::KC_A:
            yawLeftKeyDown = false;
        break;
        case OIS::KC_D:
            yawRightKeyDown = false;
        break;
        case OIS::KC_UP:
            upKeyDown = false;
        break;
        case OIS::KC_DOWN:
            downKeyDown = false;
        break;
    }

    return true;
}

