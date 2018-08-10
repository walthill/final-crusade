#include "Ronin.h"
#include "Game.h"

Ronin::Ronin()
{
	mSpeed = 2.1f;
	multiplier = 1;
	mScoreValue = 250;
	srand(unsigned(time(NULL)));
	
/*	random_device rd;
	uniform_int_distribution<int> randGen(0, 360);

	randGen(rd);*/

	rotate();
}

void Ronin::init()
{
	mMapXBound = Game::getInstance()->_LevelWidth;
	mMapYBound = Game::getInstance()->_LevelHeight;
}


Ronin::~Ronin()
{
	//Game::getInstance()->mScore++;
}

/*void Ronin::destroy()
{

}*/

void Ronin::update(double timeElapsed)
{
	if (mIsVisible)
	{
		Entity::update(timeElapsed); //animate

		checkBounds();

		mXLoc += mXVelocity * mSpeed;
		mYLoc += mYVelocity * mSpeed; 
	}
	else if(destroyedLastFrame)
	{
		cout << "ADD POINTS AND COMBO" << endl;

		if (Game::getInstance()->_ComboCount > 5)
			multiplier = 2;
		else if (Game::getInstance()->_ComboCount > 15)
			multiplier = 3;
		else
			multiplier = 1;

		Game::getInstance()->_Score += mScoreValue * multiplier; 
		Game::getInstance()->_CanCombo = true;
	
		destroyedLastFrame = false;
	}
}

void Ronin::checkBounds()
{
	//this enemy roams the map 
	//when it reaches an edge, it rotates away and moves in a new direction

	if (mXLoc < 0 || mXLoc > mMapXBound || mYLoc < 0 || mYLoc > mMapYBound)
	{
		rotate();
	}
}

void Ronin::rotate()
{
	mAngle = rand() % 360;

	direction = mAngle;

	direction *= (PI / DEGREE_CONVERSION_VALUE);

	mXVelocity = -cos(direction);
	mYVelocity = -sin(direction);

	setRotation(mAngle);
}
