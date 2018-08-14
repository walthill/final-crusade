#include "View.h"

View::View()
{
	originalShakeValue = 3.0f;
	screenShakeIntensity = originalShakeValue;
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
		
		if (shakeScreen)
		{
			screenShake();
		}
	}
}


void View::checkBounds()
{
	//Keep the camera in bounds of the background
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


void View::screenShake()
{
	originalCamX = mCamera.getX();
	originalCamY = mCamera.getY();

	random_device rd;
	uniform_int_distribution<int> randGen(-screenShakeIntensity, screenShakeIntensity); //shake intesity

	shakeX = randGen(rd);
	shakeY = randGen(rd);
	
	mCamera.setW(mDisplayWidth - abs(shakeX));
	mCamera.setH(mDisplayHeight - abs(shakeY));

	mCamera.setX(mCamera.getX() + shakeX);
	mCamera.setY(mCamera.getY() + shakeY);

	if(screenShakeIntensity > 0)
	{
		screenShakeIntensity -= 0.35f;
	}
	else	
	{
		screenShakeIntensity = originalShakeValue;
		mCamera.setX(originalCamX);
		mCamera.setY(originalCamY);
		shakeScreen = false;
	}
}

void View::toggleScreenShake(bool shouldShake)
{
	shakeScreen = shouldShake;
}


Camera* View::getCamera()
{
	return &mCamera;
}