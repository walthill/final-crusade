#ifndef SYS_H
#define SYS_H

#include "GraphicsSystem.h"
#include "InputSystem.h"
#include "Sound.h"

/*
Object headers include Trackable in order
to use time and memory managment tool
*/

class System : public Trackable
{
	private:
		GraphicsSystem mGraphicsSystem; 
		InputSystem *mInputSystem;

		const int FREQUENCY = 44100, NUM_CHANNELS = 2, SAMP_SIZE = 2048;
		
	public:	
		System();
		~System();

		//Init graphics and input
		bool initSystem(string windowName, int displayWidth, int displayHeight, Camera *camera);

		//Explicit destructor
		void cleanupSystem();

		InputSystem* getInputSystem();

		//Accessors
		GraphicsSystem* getGraphicsSystem();
};

#endif // !SYS_H