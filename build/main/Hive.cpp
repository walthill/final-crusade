#include "Hive.h"
#include "Game.h"

//SOUND - a slow moving, canon type sound

//TODO: (low) takes 2 shots to kill? the first will disable one gun. 2nd will kill

Hive::Hive()
{
	mCurrentState = IDLE;
	dtTime = 0;
	//mRotation = 45.0f;
}

Hive::~Hive()
{
}


void Hive::init(BulletPool* bp, int scoreValue, string hitAudioId, string shootAudioId)
{
	enemyHit = hitAudioId;
	enemyShoot = shootAudioId;

	mBulletPool = bp;
	mScoreValue = scoreValue;

	mMapXBound = Game::getInstance()->_LevelWidth;
	mMapYBound = Game::getInstance()->_LevelHeight;
}

void Hive::update(double timeElapsed, Player *playerObj)
{
	
	if (mIsVisible)
	{
		Entity::update(timeElapsed);

		if (hasLineOfSight(playerObj))
		{
			mCurrentState = ALERT;

			dtTime += timeElapsed;
			if (dtTime > 334.0f)
			{
				dtTime = 0;

				mBulletSpawnX1 = mXLoc + mAnim.getCurrentSprite().getSpriteWidth() / 2;
				mBulletSpawnY1 = mYLoc + 8;

				mBulletSpawnX2 = mXLoc + mAnim.getCurrentSprite().getSpriteWidth() / 2;
				mBulletSpawnY2 = mYLoc + 8;


			//	mBulletSpawnX3 = mXLoc + mAnim.getCurrentSprite().getSpriteWidth() / 2;
			//	mBulletSpawnY3 = mYLoc + mAnim.getCurrentSprite().getSpriteHeight() / 2;

			//	mBulletSpawnX4 = mXLoc + mAnim.getCurrentSprite().getSpriteWidth() / 2;
			//	mBulletSpawnY4 = mYLoc + mAnim.getCurrentSprite().getSpriteHeight() / 2;

				mBulletPool->fireProjectile(timeElapsed, mBulletSpawnX1, mBulletSpawnY1, mRotation);
				mBulletPool->fireProjectile(timeElapsed, mBulletSpawnX2, mBulletSpawnY2, mRotation + 180);
				Game::getInstance()->_Scene->playSfx(enemyShoot);
			}
		}
		else
		{
			mCurrentState = IDLE;
		}

	if (mCurrentState == IDLE)
			mRotation += 5.2f;
		else if (mCurrentState == ALERT)
			mRotation += 0.75f;
	}
	else if (destroyedLastFrame)
	{
		Game::getInstance()->_Scene->playSfx(enemyHit);

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

bool Hive::hasLineOfSight(Player *playerObj)
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
