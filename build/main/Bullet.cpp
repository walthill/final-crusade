#include "Bullet.h"

Bullet::Bullet()
{
	mState.mLive.mInUse = false;
	mState.mLive.mDamageValue = 0;
	mState.mLive.mXVelocity = 0;
	mState.mLive.mYVelocity = 0;

	mBulletSpeed = 5;
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

bool Bullet::update(double timeElapsed, vector<Collider*> colliderList)
{
	if (!mState.mLive.mInUse)
		return false;

	//call entity update to animate & update collison
	Entity::update(timeElapsed);

	mXLoc += mState.mLive.mXVelocity;;// *1.4;
	mYLoc += mState.mLive.mYVelocity;//  * 1.4;

	return checkState(colliderList);
}

bool Bullet::checkState(vector<Collider*> colliderList)//Collider* b)
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
	

	for (unsigned int i = 0; i < colliderList.size(); i++)
	{
		Collider b = *colliderList[i];

		if (checkCollision(mThisCollider, b))
		{
			if (b.getTag() == "ronin")
			{
				cout << "ENEMY HIT" << endl;
				mState.mLive.mInUse = false;
				isDead = true;

				b.getEntity()->destroy();
			}
		}
	}


	return isDead;
}


void Bullet::fire(int spawnX, int spawnY, float xVelocity, float yVelocity)
{
	mState.mLive.mInUse = true;
	mXLoc = spawnX;
	mYLoc = spawnY;
	mState.mLive.mXVelocity = xVelocity * mBulletSpeed;
	mState.mLive.mYVelocity = yVelocity * mBulletSpeed;
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
