#include "InputSystem.h"

InputSystem::InputSystem()
{
	//mInputQueue = NULL;
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
	/*if (!al_install_keyboard())
	{
		cout << "error - Keyboard Add-on not initted\n";
		return false;
	}

	if (!al_install_mouse())
	{
		cout << "error - Mouse Add-on not initted\n";
		return false;
	}

	mInputQueue = al_create_event_queue();
	assert(mInputQueue);

	if (!mInputQueue)
	{
		cout << "failed to create event_queue!\n";
		return false;
	}

	al_register_event_source(mInputQueue, al_get_keyboard_event_source());
	al_register_event_source(mInputQueue, al_get_mouse_event_source());
	*/
	cout << "Initialized InputSystem" << endl;

	return true;
}


void InputSystem::update(double elapsedTime)
{
		getKeyEvents();
		getMouseEvents();
}


void InputSystem::getKeyEvents()
{

	// Alegro event queue help @ https://www.youtube.com/watch?v=upgb6HFUep4 
	//al_get_next_event(mInputQueue, &mEvents);
	while (SDL_PollEvent(&mEvent))
	{
		switch (mEvent.type)
		{
		case SDL_MOUSEBUTTONDOWN:
			//getMouseEvents();
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
			if (mEvent.key.keysym.sym == SDLK_UP)
			{
				cout << "InputSystem: Up arrow" << endl;
				mKeyEvent.setType(UP_ARROW);
				EventSystem::getInstance()->fireEvent(mKeyEvent);
			}
			if (mEvent.key.keysym.sym == SDLK_DOWN)
			{
				cout << "InputSystem: Down arrow" << endl;
				mKeyEvent.setType(DOWN_ARROW);
				EventSystem::getInstance()->fireEvent(mKeyEvent);
			}
			if (mEvent.key.keysym.sym == SDLK_LEFT)
			{
				cout << "InputSystem: Left arrow" << endl;
				mKeyEvent.setType(LEFT_ARROW);
				EventSystem::getInstance()->fireEvent(mKeyEvent);
			}
			if (mEvent.key.keysym.sym == SDLK_RIGHT)
			{
				cout << "InputSystem: Right arrow" << endl;
				mKeyEvent.setType(RIGHT_ARROW);
				EventSystem::getInstance()->fireEvent(mKeyEvent);
			}

			break;
		}
	};

	

//	mKeyState =	SDL_GetKeyboardState(NULL);
	

	/*if (mEvent.type == SDL_KEYDOWN)
	{
		mIsKeyDown = true;
		if (mEvent.key.keysym.sym == SDLK_ESCAPE)
		{
			cout << "InputSystem: ESC" << endl;
			mKeyEvent.setType(ESC);
			EventSystem::getInstance()->fireEvent(mKeyEvent);
		}
		if (mEvent.key.keysym.sym == SDLK_SPACE)
		{
			cout << "InputSystem: Space" << endl;
			mKeyEvent.setType(SPACE);
			EventSystem::getInstance()->fireEvent(mKeyEvent);
		}
		if (mEvent.key.keysym.sym == SDL_SCANCODE_UP)
		{
			cout << "InputSystem: Up arrow" << endl;
			mKeyEvent.setType(UP_ARROW);
			EventSystem::getInstance()->fireEvent(mKeyEvent);
		}
		if (mEvent.key.keysym.sym == SDL_SCANCODE_DOWN)
		{
			cout << "InputSystem: Right arrow" << endl;
			mKeyEvent.setType(DOWN_ARROW);
			EventSystem::getInstance()->fireEvent(mKeyEvent);
		}
		if (mEvent.key.keysym.sym == SDL_SCANCODE_LEFT)
		{
			cout << "InputSystem: Right arrow" << endl;
			mKeyEvent.setType(LEFT_ARROW);
			EventSystem::getInstance()->fireEvent(mKeyEvent);
		}
		if (mEvent.key.keysym.sym == SDL_SCANCODE_RIGHT)
		{
			cout << "InputSystem: Right arrow" << endl;
			mKeyEvent.setType(RIGHT_ARROW);
			EventSystem::getInstance()->fireEvent(mKeyEvent);
		}
	}
	else if(mEvent.type == SDL_KEYUP && mIsKeyDown)
	{
		mIsKeyDown = false;
	}
	*/
	
/*	if (mEvents.type == ALLEGRO_EVENT_KEY_DOWN)
	{
		mIsKeyDown = true;

		switch (mEvents.keyboard.keycode)
		{
			case ALLEGRO_KEY_ESCAPE:
				cout << "InputSystem: ESC" << endl;
				mKeyEvent.setType(ESC);
				EventSystem::getInstance()->fireEvent(mKeyEvent);
				
				break;

			case ALLEGRO_KEY_UP:
				cout << "InputSystem: Up arrow" << endl;
				mKeyEvent.setType(UP_ARROW);
				EventSystem::getInstance()->fireEvent(mKeyEvent);

				break;

			case ALLEGRO_KEY_DOWN:
				cout << "InputSystem: Down arrow" << endl;
				mKeyEvent.setType(DOWN_ARROW);
				EventSystem::getInstance()->fireEvent(mKeyEvent);

				break;
			case ALLEGRO_KEY_LEFT:
				cout << "InputSystem: Left arrow" << endl;
				mKeyEvent.setType(LEFT_ARROW);
				EventSystem::getInstance()->fireEvent(mKeyEvent);

				break;
			case ALLEGRO_KEY_RIGHT:
				cout << "InputSystem: Right arrow" << endl;
				mKeyEvent.setType(RIGHT_ARROW);
				EventSystem::getInstance()->fireEvent(mKeyEvent);

				break;
			case ALLEGRO_KEY_SPACE:
				cout << "InputSystem: Right arrow" << endl;
				mKeyEvent.setType(SPACE);
				EventSystem::getInstance()->fireEvent(mKeyEvent);

				break;

		}
	}
	else if (mEvents.type == ALLEGRO_EVENT_KEY_UP && mIsKeyDown)
	{
		mIsKeyDown = false;
	}*/

}


void InputSystem::getMouseEvents()
{
	/*al_get_mouse_state(&mMouseState); //Used to track mouse position

	al_get_next_event(mInputQueue, &mEvents);
	
	if(mEvents.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && !mIsKeyDown)
	{
		mIsKeyDown = true;
		switch (mEvents.mouse.button)
		{
			case 1:
				cout << "InputSystem: LMB" << endl;
				//mMouseEvent.setType(LMB);
				EventSystem::getInstance()->fireEvent(mMouseEvent);

				break;
			case 2:
				cout << "InputSystem: RMB" << endl;
				//mMouseEvent.setType(RMB);
				EventSystem::getInstance()->fireEvent(mMouseEvent);

				break;
		}
	}
	else if (mEvents.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP && mIsKeyDown)
	{
		mIsKeyDown = false;
	}*/
}


/*ALLEGRO_MOUSE_STATE InputSystem::getMouse()
{
	return mMouseState;
}*/
