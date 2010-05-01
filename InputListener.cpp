// InputListener.cpp
// @author Eric D. Wills

#include "InputListener.h"
#include "World.h"

const float InputListener::ROTATION_INCREMENT    = 1.5f;
const float InputListener::TRANSLATION_INCREMENT = 30.0f;

InputListener::InputListener(World* world) :
	world(world), renderWindow(world->getRenderWindow()), camera(world->getCamera()), cameraTranslate(Ogre::Vector3::ZERO),
	cameraPitch(0.0f), cameraYaw(1.3f*Ogre::Math::PI), cameraPitchIncrement(0.0f), cameraYawIncrement(0.0f), forwardKeyDown(false),
	backKeyDown(false), leftKeyDown(false), rightKeyDown(false), shutdownKeyPressed(false) {
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

	float dt = event.timeSinceLastFrame;
	if (cameraPitchIncrement != 0.0f) {
		cameraPitch -= ROTATION_INCREMENT*dt*cameraPitchIncrement;
		cameraPitchIncrement = 0.0f;
	}
	if (cameraYawIncrement != 0.0f) {
		cameraYaw -= ROTATION_INCREMENT*dt*cameraYawIncrement;

		cameraYawIncrement = 0.0f;
	}

	camera->setOrientation(Ogre::Quaternion(Ogre::Radian(cameraYaw), 
		Ogre::Vector3::UNIT_Y)* Ogre::Quaternion(
			Ogre::Radian(cameraPitch), Ogre::Vector3::UNIT_X));

	if (forwardKeyDown){
		cameraTranslate.z = -TRANSLATION_INCREMENT*dt;
	}
	if (backKeyDown){
		cameraTranslate.z = TRANSLATION_INCREMENT*dt;
	}
	if (leftKeyDown){
		cameraTranslate.x = -TRANSLATION_INCREMENT*dt;
	}
	if (rightKeyDown){
		cameraTranslate.x = TRANSLATION_INCREMENT*dt;
	}

	camera->moveRelative(cameraTranslate);

	cameraTranslate = Ogre::Vector3::ZERO;

	world->adjustCameraHeightToTerrain();

	return true;
}

bool InputListener::frameEnded(const Ogre::FrameEvent& event) { return true; }

bool InputListener::mousePressed(const OIS::MouseEvent& event, OIS::MouseButtonID buttonID) { return true; }
bool InputListener::mouseReleased(const OIS::MouseEvent& event, OIS::MouseButtonID buttonID) { return true; }

bool InputListener::mouseMoved(const OIS::MouseEvent& event) {
	cameraPitchIncrement = event.state.Y.rel;
	cameraYawIncrement= event.state.X.rel;

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
	case OIS::KC_A:
		leftKeyDown = true;
		break;
	case OIS::KC_D:
		rightKeyDown = true;
		break;
	case OIS::KC_ESCAPE:
		shutdownKeyPressed = true;
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
	case OIS::KC_A:
		leftKeyDown = false;
		break;
	case OIS::KC_D:
		rightKeyDown = false;
		break;
	}

	return true;
}
