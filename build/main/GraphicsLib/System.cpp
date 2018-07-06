#include "System.h"

System::System()
{
}


System::~System()
{
	cleanupSystem();
}


bool System::initSystem(string windowName, int displayWidth, int displayHeight, Camera *camera)
{
	
	if (!mGraphicsSystem.initGraphics(windowName, displayWidth, displayHeight, camera))
	{
		cout << "error - could not init graphics.\n";
		return false;
	}

	//Initialize SDL_mixer
	if (Mix_OpenAudio(FREQUENCY, MIX_DEFAULT_FORMAT, NUM_CHANNELS, SAMP_SIZE) < 0)
	{
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		return false;
	}

	mInputSystem = new InputSystem;
	mInputSystem->initInputSystem();

	EventSystem::initInstance();

	return true;
}


void System::cleanupSystem()
{
	mGraphicsSystem.cleanupGraphics();

	if (mInputSystem != NULL)
	{
		delete mInputSystem;
		mInputSystem = NULL;
	}
}


InputSystem* System::getInputSystem()
{
	return mInputSystem;
}


GraphicsSystem* System::getGraphicsSystem()
{
	return &mGraphicsSystem;
}
