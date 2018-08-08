#include "Mountain.h"
#include "Game.h"

Mountain::Mountain()
{
	mSpeed = 2.5;
	multiplier = 1;
	mScoreValue = 350;
	stateChanged = false;
	srand(unsigned(time(NULL)));

	/*	random_device rd;
	uniform_int_distribution<int> randGen(0, 360);

	randGen(rd);*/

	rotate();
}

Mountain::~Mountain()
{
	//Game::getInstance()->mScore++;
}

/*void Mountain::destroy()
{

}*/

void Mountain::update(double timeElapsed, Player *playerObj)
{
	if (mIsVisible)
	{
		Entity::update(timeElapsed); //animate
		checkBounds();


		//TODO: line of sight collision check
		/*if (currentState == IDLE)
		{
			if (hasLineOfSight(playerObj))
			{

				dirToPlayer = directionToPlayer(playerObj->getX(), playerObj->getY());
				relativeDirToPlayer = abs(mRotation - dirToPlayer);

				if (relativeDirToPlayer < halfFOV || relativeDirToPlayer > 360 - halfFOV)
				{
					currentState = ALERT;
			}
			}
		}
		else if (currentState == ALERT)
		*/ {
		//mSpeed = 3;
			if (hasLineOfSight(playerObj)) // has line of sight
			{
				{
					rotateToPlayer(playerObj->getX(), playerObj->getY());
				}
			}
			else
			{
				//currentState = IDLE;

			}

		}

			mXLoc += (mXVelocity *mSpeed);
			mYLoc += (mYVelocity *mSpeed);
	}
	else if (destroyedLastFrame)
	{
	//	cout << "ADD POINTS AND COMBO" << endl;

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

bool Mountain::hasLineOfSight(Player *playerObj)
{
	bool result = false;

	//see dist between player and enemy and check the value
	xDistance = abs(mXLoc - playerObj->getX());
	yDistance = abs(mYLoc - playerObj->getY());
	int lineOfSiteLength = 175;

	if (xDistance <= lineOfSiteLength && yDistance <= lineOfSiteLength)
	{
	//	cout << "LINE COLLISION" << endl;
		result = true;
	}

	return result;
}

void Mountain::checkBounds()
{
	//this enemy roams the map 
	//when it reaches an edge, it rotates away and moves in a new direction

	if (mXLoc < 0 || mXLoc > mMapXBound || mYLoc < 0 || mYLoc > mMapYBound)
	{
		rotate();
	}
}

void Mountain::rotate()
{
	mAngle = rand() % 360;

	double direction = mAngle;

	direction *= (PI / DEGREE_CONVERSION_VALUE);

	mXVelocity = -cos(direction);
	mYVelocity = -sin(direction);

	setRotation(mAngle);
}

void Mountain::rotateToPlayer(int playerX, int playerY)
{

	direction = directionToPlayer(playerX, playerY);

	mXVelocity = -cos(direction);
	mYVelocity = -sin(direction);

	setRotation(direction);
}


double Mountain::directionToPlayer(int playerX, int playerY)
{
	dx = (mXLoc-playerX);
	dy = (/*+ 8*/ mYLoc- playerY);

	angle = (atan2(dy, dx)*DEGREE_CONVERSION_VALUE) / PI;
	
	return angle;
}