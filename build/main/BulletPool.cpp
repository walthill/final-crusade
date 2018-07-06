#include "BulletPool.h"

BulletPool::BulletPool()
{
	//initPool();
}

BulletPool::~BulletPool()
{
}

void BulletPool::initBulletData(Animation bulletSpriteData, int windowW, int windowH, bool isOwnerPlayer)
{
	for (int i = 0; i < mPOOL_SIZE; i++)
	{
		bullets[i].setXBound(windowW);
		bullets[i].setYBound(windowH);

		bullets[i].setAnimation(bulletSpriteData);
	}

  	initPool();
}

void BulletPool::create(int x, int y, int xVelocity, int yVeloctity, bool isPlayerBullet)
{
	//make sure pool isn't full
	assert(mFirstAvailable != NULL);

	//grab next available
	Bullet *newBullet = mFirstAvailable;
	mFirstAvailable = newBullet->getNext();

	//newBullet->initBullet(x, y, xVelocity, yVeloctity, isPlayerBullet);

/*	for (int i = 0; i < mPOOL_SIZE; i++)
	{
		if (!bullets[i].isInUse())
		{
			bullets[i].initBullet(x, y, xVelocity, yVeloctity, isPlayerBullet);
			return;
		}
	}*/

}

void BulletPool::initPool()
{
	mFirstAvailable = &bullets[0];

	for (int i = 0; i < mPOOL_SIZE - 1; i++)
	{
		bullets[i].setNext(&bullets[i] + 1);
	}

	bullets[mPOOL_SIZE - 1].setNext(NULL);
}


void BulletPool::resetPool()
{
	for (int i = 0; i < mPOOL_SIZE - 1; i++)
	{
		bullets[i].setInUse(false);
		initPool();
	}
}


void BulletPool::update(double timeElapsed)
{
	for (int i = 0; i < mPOOL_SIZE; i++)
	{
		if (bullets[i].update(timeElapsed)) //returns true if bullet is not in use
		{
 			bullets[i].setNext(mFirstAvailable);
			mFirstAvailable = &bullets[i];
		}
	}
}

void BulletPool::draw(GraphicsSystem *graphicsSystem, int camX, int camY)
{
	for (int i = 0; i < mPOOL_SIZE; i++)
	{
		if(bullets[i].isInUse())
			bullets[i].draw(graphicsSystem, camX, camY);
	}
}


void BulletPool::fireProjectile(double deltaTime, int playerX, int playerY, 
								float direction)
{
	//make sure pool isn't full
	assert(mFirstAvailable != NULL);

	Bullet *newBullet = mFirstAvailable;
	mFirstAvailable = newBullet->getNext();

	mDeltaTime += deltaTime;
	if (mDeltaTime >= mFireRate)
	{
		mDeltaTime = mFireRate;

		direction *= (PI / DEGREE_CONVERSION_VALUE);
		bulletX = cos(direction);
		bulletY = sin(direction);

		newBullet->fire(playerX, playerY, -bulletX, -bulletY);

		mDeltaTime = 0;
	}
}



