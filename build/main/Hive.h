#ifndef HIVE_H
#define HIVE_H

#include "Player.h"
#include "BulletPool.h"

class Hive : public Entity
{
	private:
		string enemyShoot, enemyHit;


		enum State { IDLE, ALERT };
		int mCurrentState;

		BulletPool* mBulletPool;

		double dtTime;
		int multiplier, mScoreValue;
		float mBulletSpawnX1, mBulletSpawnY1,
			mBulletSpawnX2, mBulletSpawnY2,
			mBulletSpawnX3, mBulletSpawnY3,
			mBulletSpawnX4, mBulletSpawnY4;
		int mMapXBound, mMapYBound;
		
		const int LINE_SIGHT_LENGTH = 315;
		int xDistance, yDistance;

	public:
		Hive();
		~Hive();

		void init(BulletPool* bp, int scoreValue, string hitAudioId, string shootAudioId);

		void update(double timeElpased, Player *playerObj);
		bool hasLineOfSight(Player *playerObj);

};

#endif // !HIVE_H

