#ifndef BULLET_H
#define BULLET_H

#include "Entity.h"
#include "RoninManager.h"

class Bullet : public Entity
{
	private:
		const int DELETE_BOUNDS = 24;
		const float KNOCKBACK_VALUE = 2.5f;
		const string PLAYER_BULLET_TAG = "stdBullet";
		const string ENEMY_BULLET_TAG = "enemyBullet";
		
		int numEnemyColliders;
		Collider b;

		int mLiveBoundX, mLiveBoundY;
		int mBulletSpeed;
		bool isDead;

		union //used for a free list
		{
			//state when in use
			struct
			{
				int mDamageValue;
				float mXVelocity, mYVelocity;
				bool mInUse;
				bool mBelongsToPlayer;

			} mLive;

			Bullet* nextBullet;
		} mState;

	public:
		Bullet();
		~Bullet();

		void freeBullet();

		Bullet* getNext() const { return mState.nextBullet;  }
		void setNext(Bullet* next) { mState.nextBullet = next;  }

		bool update(double timeElapsed, vector<Collider*> colliderList, int sizeOfBulletPool);//Collider* b);

		bool checkState(vector<Collider*> colliderList, int sizeOfBulletPool);//Collider* b);
		void fire(float spawnX, float spawnY, float xVelocity, float yVelocity);

		void setBulletSpeed(int bulletSpeed);
		void setInUse(bool isInUse);
		bool isInUse();
		
		void setXBound(int xBound);
		void setYBound(int yBound);

};

#endif // !BULLET_H
