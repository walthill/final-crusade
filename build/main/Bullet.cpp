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
				if ( (currRot > 0 && currRot <= 45) || (currRot < 0 && currRot >= -45) )
				{
					//deflect
					//TODO: add deflect/block sound here
					if ((mThisCollider.getX() >= b.getX() && mThisCollider.getX() <= b.getX() + b.getW() - 4)) //&& mThisCollider.getY() >= b.getY())
					{
						cout << "ENEMY HIT" << endl;
						mState.mLive.mInUse = false;
						isDead = true;
						
						//knockback
						b.getEntity()->setLoc(b.getEntity()->getX() + (mState.mLive.mXVelocity*2.0f), b.getEntity()->getY());

					}
					//ded
					else if ((mThisCollider.getX() >= b.getX() + b.getW() - 4 && mThisCollider.getX() <= b.getX() + b.getW()))
					{
						cout << "ENEMY HIT" << endl;
						mState.mLive.mInUse = false;
						isDead = true;

						b.getEntity()->destroy();
					}
				}



				//135 deg to -135 deg
				else if (currRot > 135 && currRot <= 180 || (currRot < -135 && currRot >= -179))
				{
					//deflect
					if (mThisCollider.getX() > b.getX() + 4 && mThisCollider.getX() <= b.getX() + b.getW())
					{
						cout << "ENEMY HIT" << endl;
						mState.mLive.mInUse = false;
						isDead = true;

						//knockback - velocity will be negative
						b.getEntity()->setLoc(b.getEntity()->getX() + (mState.mLive.mXVelocity*2.0f), b.getEntity()->getY());
					}//ded
					else if (mThisCollider.getX() > b.getX() && mThisCollider.getX() <= b.getX()+4)
					{
						cout << "ENEMY HIT" << endl;
						mState.mLive.mInUse = false;
						isDead = true;

						b.getEntity()->destroy();
					}
				}


				//45 deg to 135 deg
				else if (currRot > 45 && currRot <= 90 || (currRot > 90 && currRot <= 135))
				{
					//deflect
					if (mThisCollider.getY() > b.getY() && mThisCollider.getY() <= b.getY() + b.getH() - 4)
					{
						cout << "ENEMY HIT" << endl;
						mState.mLive.mInUse = false;
						isDead = true;

						//knockback
						b.getEntity()->setLoc(b.getEntity()->getX(), b.getEntity()->getY() + (mState.mLive.mYVelocity*2.0f));

					}
					//ded
					else if (mThisCollider.getY() > b.getY() + b.getH() - 4 && mThisCollider.getY() <= b.getY() + b.getH())
					{
						cout << "ENEMY HIT" << endl;
						mState.mLive.mInUse = false;
						isDead = true;

						b.getEntity()->destroy();
					}
				}

				//-45 deg to -135 deg
				else if (currRot < -45 && currRot >= -90 || (currRot < -90 && currRot >= -135))
				{
					//deflect
					if (mThisCollider.getY() > b.getY() + 4 && mThisCollider.getY() <= b.getY() + b.getH())
					{
						cout << "ENEMY HIT" << endl;
						mState.mLive.mInUse = false;
						isDead = true;

						//knockback- velocity will be negative
						b.getEntity()->setLoc(b.getEntity()->getX(), b.getEntity()->getY() + (mState.mLive.mYVelocity*2.0f));
					}
					//ded
					else if (mThisCollider.getY() < b.getY() && mThisCollider.getY() <= b.getY() + 4)
					{
						cout << "ENEMY HIT" << endl;
						mState.mLive.mInUse = false;
						isDead = true;

						b.getEntity()->destroy();
					}
				}			
			}
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
