#include "Bullet.h"

Bullet::Bullet()
{
	mState.mLive.mInUse = false;
	mState.mLive.mDamageValue = 0;
	mState.mLive.mXVelocity = 0;
	mState.mLive.mYVelocity = 0;
}

Bullet::~Bullet()
{
}

/*void Bullet::initBullet(int x, int y, int xVelocity, int yVelocity, bool isPlayers)
{
	mState.mLive.mInUse = false;
	mState.mLive.mBelongsToPlayer = isPlayers;
	
	mXLoc = x;
	mYLoc = y;
	mState.mLive.mXVelocity = xVelocity;
	mState.mLive.mYVelocity = yVelocity;
}*/

bool Bullet::update(double timeElapsed)
{
	if (!mState.mLive.mInUse)
		return false;

	//call entity update to animate
	Entity::update(timeElapsed);

	mXLoc += mState.mLive.mXVelocity;
	mYLoc += mState.mLive.mYVelocity;

	return checkState();
}

bool Bullet::checkState()
{
	bool isDead = false;

	if (mXLoc < 0 || mXLoc > mLiveBoundX)
	{
		mState.mLive.mInUse = false;
		isDead = true;
	}
	if (mYLoc < 0 || mYLoc >mLiveBoundY)
	{
		mState.mLive.mInUse = false;
		isDead = true;
	}

	return isDead;
}


void Bullet::fire(int spawnX, int spawnY, float xVelocity, float yVelocity)
{
	mState.mLive.mInUse = true;
	mXLoc = spawnX;
	mYLoc = spawnY;
	mState.mLive.mXVelocity = xVelocity * 3;
	mState.mLive.mYVelocity = yVelocity * 3;
}


void Bullet::setInUse(bool isInUse)
{
	mState.mLive.mInUse = isInUse;
}

bool Bullet::isInUse()
{
	return mState.mLive.mInUse;
}


void Bullet::setXBound(int xBound)
{
	mLiveBoundX = xBound;
}

void Bullet::setYBound(int yBound)
{
	mLiveBoundY = yBound;
}
