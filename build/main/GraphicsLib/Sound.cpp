#include "Sound.h"

//Recieved help from LazyFoo @ https://bit.ly/2HjaZz5

Sound::Sound()
{
	Mix_Chunk *mSfx = NULL;
	Mix_Music *mMusic = NULL;

	mIsSfx = true;
	bool isLooping = true;
}

Sound::~Sound()
{
	cleanupSound();
}


void Sound::initSound(bool isSFX, string audioFile, bool isLooping)
{
	mIsLooping = isLooping;

	if (isSFX)
	{
		mIsSfx = true;
		mIsLooping = false; //NOTE: sfx has no way to pause/resume

		//Load sound effects
		mSfx = Mix_LoadWAV(audioFile.c_str());
		if (mSfx == NULL)
		{
			printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		}
	}
	else
	{
		mIsSfx = false;

		//Load music
		mMusic = Mix_LoadMUS(audioFile.c_str());
		if (mMusic == NULL)
		{
			printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
		}
	}

	cout << "Initialized sound" << endl;
}

void Sound::cleanupSound()
{
	//Free the sound effects
	Mix_FreeChunk(mSfx);
	mSfx = NULL;
	
	//Free the music
	Mix_FreeMusic(mMusic);
	mMusic = NULL;
}

void Sound::playSound()
{
	//sfx
	if (mIsSfx)
		Mix_PlayChannel(-1, mSfx, 0);

	//music
	else if (!mIsSfx && mIsLooping)
	{
		if (Mix_PlayingMusic() == 0)
			Mix_PlayMusic(mMusic, -1);
	}
	else if (mIsSfx && !mIsLooping)
	{
		if (Mix_PlayingMusic() == 0)
			Mix_PlayMusic(mMusic, 0);
	}
}


void Sound::pauseSound()
{
	if (!mSfx)
	{
		if (Mix_PlayingMusic() != 0)
		{
			//If the music is paused
			if (Mix_PausedMusic() == 1)
				Mix_ResumeMusic();
			else
				Mix_PauseMusic();
		}
	}
}

void Sound::stopSound()
{
	if(!mSfx && Mix_PlayingMusic() != 0)
		Mix_HaltMusic();
}

bool Sound::isSfx()
{
	return mIsSfx;
}