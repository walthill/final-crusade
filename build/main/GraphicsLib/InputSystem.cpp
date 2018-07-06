#include "InputSystem.h"

InputSystem::InputSystem()
{
	mIsKeyDown = false;
}


InputSystem::~InputSystem()
{
	cleanupInputSystem();
}


void InputSystem::cleanupInputSystem()
{
	//Karim commented out. Dunno how to clean up SDL
	/*if (mInputQueue != NULL)
	{
		al_destroy_event_queue(mInputQueue);
		mInputQueue = NULL;
	}*/
}


bool InputSystem::initInputSystem()
{
	cout << "Initialized InputSystem" << endl;

	return true;
}


void InputSystem::update(double elapsedTime)
{	
	getInputEvents();
}


void InputSystem::getInputEvents()
{

	while (SDL_PollEvent(&mEvent))
	{
		switch (mEvent.type)
		{
		case SDL_MOUSEMOTION:
			EventSystem::getInstance()->fireEvent(MouseEvent(MOUSE_MOTION, mEvent.motion.x, mEvent.motion.y));
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (mEvent.button.button == SDL_BUTTON_LEFT)
			{
				mKeyEvent.setType(LEFT_MOUSE_DOWN);
				EventSystem::getInstance()->fireEvent(mKeyEvent);
			}
			break;
		case SDL_KEYDOWN:
			if (mEvent.key.keysym.sym == SDLK_ESCAPE)
			{
				cout << "InputSystem: ESC" << endl;
				mKeyEvent.setType(ESC);
				EventSystem::getInstance()->fireEvent(mKeyEvent);
			}
			if (mEvent.key.keysym.sym == SDLK_RETURN)
			{
				cout << "InputSystem: Space" << endl;
				mKeyEvent.setType(ENTER);
				EventSystem::getInstance()->fireEvent(mKeyEvent);
			}
			if (mEvent.key.keysym.sym == SDLK_w)
			{
				cout << "InputSystem: Up arrow" << endl;
				mKeyEvent.setType(UP_ARROW);
				EventSystem::getInstance()->fireEvent(mKeyEvent);
			}
			if (mEvent.key.keysym.sym == SDLK_s)
			{
				cout << "InputSystem: Down arrow" << endl;
				mKeyEvent.setType(DOWN_ARROW);
				EventSystem::getInstance()->fireEvent(mKeyEvent);
			}
			if (mEvent.key.keysym.sym == SDLK_a)
			{
				cout << "InputSystem: Left arrow" << endl;
				mKeyEvent.setType(LEFT_ARROW);
				EventSystem::getInstance()->fireEvent(mKeyEvent);
			}
			if (mEvent.key.keysym.sym == SDLK_d)
			{
				cout << "InputSystem: Right arrow" << endl;
				mKeyEvent.setType(RIGHT_ARROW);
				EventSystem::getInstance()->fireEvent(mKeyEvent);
			}

			break;

		case SDL_KEYUP:
			if (mEvent.key.keysym.sym == SDLK_w)
			{
				cout << "InputSystem: Up arrow" << endl;
				mKeyEvent.setType(UP_ARROW_RELEASED);
				EventSystem::getInstance()->fireEvent(mKeyEvent);
			}
			if (mEvent.key.keysym.sym == SDLK_s)
			{
				cout << "InputSystem: Down arrow" << endl;
				mKeyEvent.setType(DOWN_ARROW_RELEASED);
				EventSystem::getInstance()->fireEvent(mKeyEvent);
			}
			if (mEvent.key.keysym.sym == SDLK_a)
			{
				cout << "InputSystem: Left arrow" << endl;
				mKeyEvent.setType(LEFT_ARROW_RELEASED);
				EventSystem::getInstance()->fireEvent(mKeyEvent);
			}
			if (mEvent.key.keysym.sym == SDLK_d)
			{
				cout << "InputSystem: Right arrow" << endl;
				mKeyEvent.setType(RIGHT_ARROW_RELEASED);
				EventSystem::getInstance()->fireEvent(mKeyEvent);
			}

			break;
		}
	};
}
