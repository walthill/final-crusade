#ifndef SOUND_H
#define SOUND_H

#include <string>
#include "SDL_mixer.h"
#include "Trackable.h"

using namespace std;

class Sound : public Trackable
{
	private:
		Mix_Chunk *mSfx;
		Mix_Music *mMusic;

		bool mIsSfx;
		bool mIsLooping;

	public:
		Sound();
		~Sound();

		void initSound(bool isSFX, string audioFile, bool isLooping);
		void cleanupSound();

		void playSound();
		void pauseSound();
		void stopSound();

		bool isSfx();
};

#endif // !SOUND_H
