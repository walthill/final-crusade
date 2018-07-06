#ifndef VIEW_H
#define VIEW_H

#include "Camera.h"
#include "Player.h"


//Lazyfoo helped a whole lot here https://bit.ly/2KDwJva
class View 
{
	private:
		int mDisplayWidth, mDisplayHeight, mLevelWidth, mLevelHeight;

		Camera mCamera;
		Player *mPlayerRef;

		int mTimeBeforeUpdate = 10;

	public:
		View();
		View(Player *player, int displayWidth, int displayHeight, int levelWidth, int levelHeight);
		~View();

		void initView(Player *player, int displayWidth, int displayHeight, int levelWidth, int levelHeight);

		void update(double timeElapsed);

		void checkBounds();
		void centerView();

		Camera* getCamera();

};

#endif // !VIEW_H


//300