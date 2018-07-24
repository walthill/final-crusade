#ifndef VIEW_H
#define VIEW_H

#include "Camera.h"
#include "Player.h"
#include <random>


//Lazyfoo helped a whole lot here https://bit.ly/2KDwJva
class View
{
	private:
		int mDisplayWidth, mDisplayHeight, mLevelWidth, mLevelHeight;
		int shakeX, shakeY;

		Camera mCamera;
		Player *mPlayerRef;

		bool shakeScreen = false;
		//int mTimeBeforeUpdate = 10;

		int originalCamX, originalCamY;
		float originalShakeValue;
		float screenShakeIntensity;

	public:
		View();
		View(Player *player, int displayWidth, int displayHeight, int levelWidth, int levelHeight);
		~View();

		void initView(Player *player, int displayWidth, int displayHeight, int levelWidth, int levelHeight);

		void update(double timeElapsed);

		void checkBounds();
		void centerView();

		void toggleScreenShake(bool shouldShake);
		void screenShake();

		Camera* getCamera();

};

#endif // !VIEW_H
