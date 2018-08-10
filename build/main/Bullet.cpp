#include "Bullet.h"

Bullet::Bullet()
{
	mState.mLive.mInUse = false;
	mState.mLive.mDamageValue = 0;
	mState.mLive.mXVelocity = 0;
	mState.mLive.mYVelocity = 0;

//	mBulletSpeed = 5;
}

Bullet::~Bullet()
{
}

void Bullet::freeBullet()
{
	mState.mLive.mInUse = false;
	isDead = true;
}

bool Bullet::update(double timeElapsed, vector<Collider*> colliderList, int sizeOfBulletPool)
{
	if (!mState.mLive.mInUse)
		return false;

	//call entity update to animate & update collison
	Entity::update(timeElapsed);

	mXLoc += mState.mLive.mXVelocity;;// *1.4;
	mYLoc += mState.mLive.mYVelocity;//  * 1.4;

	return checkState(colliderList, sizeOfBulletPool);
}

bool Bullet::checkState(vector<Collider*> colliderList, int sizeOfBulletPool)//Collider* b)
{
	isDead = false;

	if (mXLoc < -DELETE_BOUNDS || mXLoc > mLiveBoundX+ DELETE_BOUNDS)
	{
		mState.mLive.mInUse = false;
		isDead = true;
	}
	if (mYLoc < -DELETE_BOUNDS || mYLoc > mLiveBoundY + DELETE_BOUNDS)
	{
		mState.mLive.mInUse = false;
		isDead = true;
	}
	
	if (mThisCollider.getTag() == PLAYER_BULLET_TAG)
	{
		numEnemyColliders = colliderList.size() - sizeOfBulletPool;

		for (unsigned int i = 0; i < colliderList.size() - sizeOfBulletPool; i++)
		{
			b = *colliderList[i];

			if (checkCollision(mThisCollider, b))
			{
				if (mThisCollider.getTag() == PLAYER_BULLET_TAG)
				{
					if (b.getTag() == "ronin" || b.getTag() == "hive")
					{
						freeBullet();
						b.getEntity()->destroy();
						break; //TODO:(TESTING) only act on one collision per frame
					}
					else if (b.getTag() == "mountain")
					{

						float currRot = b.getEntity()->getRotation();

						if (currRot > 180.0f)
							currRot -= 360.0f;

						//make single accesor calls here?
						//int thisColliderX, thisColliderY;
						//int bColliderX, bColliderY;
						//int bColliderW, bColliderH;

						//45 deg to -45 deg
						if ((currRot > 0 && currRot <= 45) || (currRot < 0 && currRot >= -45))
						{
							//deflect
							//TODO: add deflect/block sound here
							if ((mThisCollider.getX() >= b.getX() && mThisCollider.getX() <= b.getX() + b.getW() - 4)) //&& mThisCollider.getY() >= b.getY())
							{

								freeBullet();
								//knockback
								b.getEntity()->setLoc(b.getEntity()->getX() + (mState.mLive.mXVelocity*2.0f), b.getEntity()->getY());
								break;
							}
							//ded
							else if ((mThisCollider.getX() >= b.getX() + b.getW() - 4 && mThisCollider.getX() <= b.getX() + b.getW()))
							{
								freeBullet();
								b.getEntity()->destroy(); break;
							}
						}



						//135 deg to -135 deg
						else if (currRot > 135 && currRot <= 180 || (currRot < -135 && currRot >= -179))
						{
							//deflect
							if (mThisCollider.getX() > b.getX() + 4 && mThisCollider.getX() <= b.getX() + b.getW())
							{
								freeBullet();
								//knockback - velocity will be negative
								b.getEntity()->setLoc(b.getEntity()->getX() + (mState.mLive.mXVelocity*2.0f), b.getEntity()->getY());
								break;
							}//ded
							else if (mThisCollider.getX() > b.getX() && mThisCollider.getX() <= b.getX() + 4)
							{
								freeBullet();
								b.getEntity()->destroy();
								break;
							}
						}


						//45 deg to 135 deg
						else if (currRot > 45 && currRot <= 90 || (currRot > 90 && currRot <= 135))
						{
							//deflect
							if (mThisCollider.getY() > b.getY() && mThisCollider.getY() <= b.getY() + b.getH() - 4)
							{
								freeBullet();
								//knockback
								b.getEntity()->setLoc(b.getEntity()->getX(), b.getEntity()->getY() + (mState.mLive.mYVelocity*2.0f));
								break;
							}
							//ded
							else if (mThisCollider.getY() > b.getY() + b.getH() - 4 && mThisCollider.getY() <= b.getY() + b.getH())
							{
								freeBullet();
								b.getEntity()->destroy();
								break;
							}
						}

						//-45 deg to -135 deg
						else if (currRot < -45 && currRot >= -90 || (currRot < -90 && currRot >= -135))
						{
							//deflect
							if (mThisCollider.getY() > b.getY() + 4 && mThisCollider.getY() <= b.getY() + b.getH())
							{
								freeBullet();
								//knockback- velocity will be negative
								b.getEntity()->setLoc(b.getEntity()->getX(), b.getEntity()->getY() + (mState.mLive.mYVelocity*2.0f));
								break;
							}
							//ded
							else if (mThisCollider.getY() < b.getY() && mThisCollider.getY() <= b.getY() + 4)
							{
								freeBullet();
								b.getEntity()->destroy();
								break;
							}
						}
					}
				}
			}
		}
	}
	else
	{
			b = *colliderList.back(); //NOTE: last index should be player
			if (checkCollision(mThisCollider, b))
			{
				freeBullet();
			}
	}

	return isDead;
}


void Bullet::fire(float spawnX, float spawnY, float xVelocity, float yVelocity)
{
	mState.mLive.mInUse = true;
	mXLoc = spawnX;
	mYLoc = spawnY;
	mState.mLive.mXVelocity = xVelocity * mBulletSpeed;
	mState.mLive.mYVelocity = yVelocity * mBulletSpeed;
}

void Bullet::setBulletSpeed(int bulletSpeed)
{
	mBulletSpeed = bulletSpeed;
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
