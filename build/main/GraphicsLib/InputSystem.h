#ifndef INPUT_SYS
#define INPUT_SYS

#include <cassert>
#include "EventSystem.h"
#include "Event.h"
#include "MouseEvent.h"
#include "SDL.h"
/*
Object headers include Trackable in order
to use time and memory managment tool
*/

class InputSystem : public Trackable
{
	private:
		Event mKeyEvent;
		MouseEvent mMouseEvent;

		SDL_Event mEvent;


		bool mIsKeyDown; //For getting input

	public:

		InputSystem();
		~InputSystem();

		//Init and cleanup
		bool initInputSystem();
		void cleanupInputSystem();

		//Input and mouse position
		void getKeyEvents();
		void getMouseEvents();

		//Call mouse and key events
		void update(double elapsedTime);
};

#endif // !INPUT_SYS
