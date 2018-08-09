#include "Mountain.h"
#include "Game.h"


Mountain::Mountain()
{
	mSpeed = 2.0f;
	multiplier = 1;
	mScoreValue = 350;
	srand(unsigned(time(NULL)));

	/*	random_device rd;
	uniform_int_distribution<int> randGen(0, 360);

	randGen(rd);*/

	rotate();
}

void Mountain::init()
{
 	mMapXBound = Game::getInstance()->_LevelWidth;
	mMapYBound = Game::getInstance()->_LevelHeight;
}

Mountain::~Mountain()
{
}

void Mountain::update(double timeElapsed, Player *playerObj)
{
	if (mIsVisible)
	{
		Entity::update(timeElapsed); //animate
		
		checkBounds();
		ai(playerObj);

		mXLoc += (mXVelocity *mSpeed);
		mYLoc += (mYVelocity * mSpeed);
	}
	else if (destroyedLastFrame)
	{
		//scoring and combo handling
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

void Mountain::ai(Player *playerObj)
{
	if (currentState == IDLE)
	{
		mSpeed = 2.0f;
		if (hasLineOfSight(playerObj))
		{
			dirToPlayer = (int)directionToPlayer(playerObj->getX(), playerObj->getY());
			relativeDirToPlayer = (int)abs(mRotation - dirToPlayer);

			if (relativeDirToPlayer < HALF_FOV || relativeDirToPlayer > 360 - HALF_FOV)
			{
				currentState = ALERT;
			}
		}
	}
	else if (currentState == ALERT)
	{
		mSpeed = 3.0f;
		if (hasLineOfSight(playerObj)) // has line of sight
		{
			rotateToPlayer(playerObj->getX(), playerObj->getY());
		}
		else
		{
			currentState = IDLE;
		}
	}
}

bool Mountain::hasLineOfSight(Player *playerObj)
{
	bool result = false;

	//see dist between player and enemy and check the value
	xDistance = abs(mXLoc - playerObj->getX());
	yDistance = abs(mYLoc - playerObj->getY());

	if (xDistance <= LINE_SIGHT_LENGTH && yDistance <= LINE_SIGHT_LENGTH)
	{
		result = true;
	}

	return result;
}

void Mountain::checkBounds()
{
	//this enemy roams the map 
	//when it reaches an edge, it rotates away and moves in a new direction

	if (mXLoc < 5 || mXLoc > mMapXBound - 12 || mYLoc < 5 || mYLoc > mMapYBound -12)
	{
		rotate();
	}
}

void Mountain::rotate()
{
	mAngle = rand() % 360;

	direction = mAngle;

	direction *= (PI / DEGREE_CONVERSION_VALUE);

	mXVelocity = -cos(direction);
	mYVelocity = -sin(direction);

	setRotation(mAngle);
}

void Mountain::rotateToPlayer(float playerX, float playerY)
{
	direction = directionToPlayer(playerX, playerY);
	
	//trig functions return in radians
	mXVelocity = -cos(direction* PI/DEGREE_CONVERSION_VALUE);
	mYVelocity = -sin(direction* PI / DEGREE_CONVERSION_VALUE);

	setRotation(direction);
}


float Mountain::directionToPlayer(float playerX, float playerY)
{
	dx = (mXLoc- playerX);
	dy = (mYLoc- playerY);

	angle = (float)(atan2(dy, dx)*DEGREE_CONVERSION_VALUE) / PI;

	return angle;
}