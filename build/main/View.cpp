#include "View.h"

View::View()
{

}

View::View(Player *player, int displayWidth, int displayHeight, int levelWidth, int levelHeight)
{
	mPlayerRef = player;

	mDisplayWidth = displayWidth;
	mDisplayHeight = displayHeight;
	mLevelWidth = levelWidth;
	mLevelHeight = levelHeight;

	mCamera.setW(displayWidth);
	mCamera.setH(displayHeight);
	
	centerView();
}


View::~View()
{
}

void View::initView(Player *player, int displayWidth, int displayHeight, int levelWidth, int levelHeight)
{
	mPlayerRef = player;

	mDisplayWidth = displayWidth;
	mDisplayHeight = displayHeight;
	mLevelWidth = levelWidth;
	mLevelHeight = levelHeight;

	//camera x&y default to 0
	mCamera.setW(displayWidth);
	mCamera.setH(displayHeight);

	centerView();
	checkBounds();
}

void View::update(double timeElapsed)
{
 	if (mPlayerRef != nullptr)// && timeElapsed > mTimeBeforeUpdate)
	{
 		centerView();
		checkBounds();
		
	}
}


void View::checkBounds()
{
	//Keep the camera in bounds
	if (mCamera.getX() < 0)
	{
		mCamera.setX(0);
	}
	if (mCamera.getY() < 0)
	{
		mCamera.setY(0);
	}
	if (mCamera.getX() > mLevelWidth - mCamera.getW())
	{
		mCamera.setX(mLevelWidth - mCamera.getW());
	}
	if (mCamera.getY() > mLevelHeight - mCamera.getH())
	{
		mCamera.setY(mLevelHeight - mCamera.getH());
	}
}

void View::centerView()
{
	//center camera
	mCamera.setX((mPlayerRef->getX() + mPlayerRef->getWidth() / 2) - mDisplayWidth / 2);
	mCamera.setY((mPlayerRef->getY() + mPlayerRef->getHeight() / 2) - mDisplayHeight / 2);
}


Camera* View::getCamera()
{
	return &mCamera;
}