#ifndef BULLET_H
#define BULLET_H

#include "Entity.h"
#include "RoninManager.h"

class Bullet : public Entity
{
	private:
		const int DELETE_BOUNDS = 24;

		int mLiveBoundX, mLiveBoundY;
		int mBulletSpeed;
		
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

		Bullet* getNext() const { return mState.nextBullet;  }
		void setNext(Bullet* next) { mState.nextBullet = next;  }

		bool update(double timeElapsed, vector<Collider*> colliderList);//Collider* b);

		bool checkState(vector<Collider*> colliderList);//Collider* b);
		void fire(float spawnX, float spawnY, float xVelocity, float yVelocity);

		void setInUse(bool isInUse);
		bool isInUse();
		
		void setXBound(int xBound);
		void setYBound(int yBound);

};

#endif // !BULLET_H
