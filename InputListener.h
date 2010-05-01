// InputListener.h
// @author Eric D. Wills

#ifndef __TERRAIN_DEMO_FRAME_LISTENER_H__
#define __TERRAIN_DEMO_FRAME_LISTENER_H__

#include "Ogre.h"
#include "OIS/OIS.h"

class World;

class InputListener : public Ogre::FrameListener, public OIS::MouseListener, public OIS::KeyListener {
 private:
	static const float ROTATION_INCREMENT;
	static const float TRANSLATION_INCREMENT;

	World* world;
	Ogre::RenderWindow* renderWindow;
	Ogre::Camera* camera;
	OIS::InputManager* inputManager;
	OIS::Mouse* mouse;
	OIS::Keyboard* keyboard;
	Ogre::Vector3 cameraTranslate;
	float cameraPitch;
	float cameraYaw;
	float cameraPitchIncrement;
	float cameraYawIncrement;
	bool forwardKeyDown;
	bool backKeyDown;
	bool leftKeyDown;
	bool rightKeyDown;
	bool shutdownKeyPressed;

 public:
	InputListener(World* world);

	~InputListener();

	bool frameStarted(const Ogre::FrameEvent& event);
	bool frameEnded(const Ogre::FrameEvent& event);

	bool mousePressed(const OIS::MouseEvent& event, OIS::MouseButtonID buttonID);
	bool mouseReleased(const OIS::MouseEvent& event, OIS::MouseButtonID buttonID);
	bool mouseMoved(const OIS::MouseEvent& event);

	bool keyPressed(const OIS::KeyEvent& event);
	bool keyReleased(const OIS::KeyEvent& event);
};

#endif
