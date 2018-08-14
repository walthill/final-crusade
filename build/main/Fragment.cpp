#include "Fragment.h"
#include "Game.h"

Fragment::Fragment()
{
	collisionDetected = false;
	lineOfSightTriggered = false;
	addPoints = false;
}

Fragment::~Fragment()
{

}

void Fragment::init(string pickupAudioId)
{
	fragmentPickup = pickupAudioId;
}

void Fragment::update(double timeElapsed, Player *playerObj)
{
	if (mIsVisible)
	{
		Entity::update(timeElapsed);

		if (collisionDetected)
		{
			if (lineOfSightTriggered || hasLineOfSight(playerObj))
			{
				lineOfSightTriggered = true;
				rotateToPlayer(playerObj->getX(), playerObj->getY()); //item will move to the player
			}

			mXLoc += mXVelocity * 4.2f;
			mYLoc += mYVelocity * 4.2f;

			if ((mXLoc > playerObj->getX() - 2 && mXLoc < playerObj->getX() + 2)
				&& (mYLoc > playerObj->getY() - 2 && mYLoc < playerObj->getY() + 2))
			{
				mIsVisible = false;
				addPoints = true;
			}
		}
		else if (checkCollision(mThisCollider, *playerObj->getCollider()) && !collisionDetected)
		{
			collisionDetected = true;
		}
	}
	else
	{
		if (addPoints)
		{
			Game::getInstance()->_Scene->playSfx(fragmentPickup);
			Game::getInstance()->_NumFragments++; 
			//play sound
			addPoints = false;
		}
	}
}

bool Fragment::hasLineOfSight(Player *playerObj)
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


void Fragment::rotateToPlayer(float playerX, float playerY)
{
	direction = directionToPlayer(playerX, playerY);

	//trig functions return in radians
	mXVelocity = -cos(direction* PI / DEGREE_CONVERSION_VALUE);
	mYVelocity = -sin(direction* PI / DEGREE_CONVERSION_VALUE);

	setRotation(direction);
}


float Fragment::directionToPlayer(float playerX, float playerY)
{
	dx = (mXLoc - playerX);
	dy = (mYLoc - playerY);

	angle = (float)(atan2(dy, dx)*DEGREE_CONVERSION_VALUE) / PI;

	return angle;
}