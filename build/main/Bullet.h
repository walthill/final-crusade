#ifndef BULLET_H
#define BULLET_H

#include "Entity.h"
#include "RoninManager.h"

class Bullet : public Entity
{
	private:
		int mLiveBoundX, mLiveBoundY;
		int mBulletSpeed = 10; //TODO: data driven

		//TODO: smaller bullet sprites

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

		//void initBullet(int x, int y, int xVelocity, int yVelocity, bool isPlayers);

		Bullet* getNext() const { return mState.nextBullet;  }
		void setNext(Bullet* next) { mState.nextBullet = next;  }

		bool update(double timeElapsed, vector<Collider*> colliderList);//Collider* b);
		//void draw(GraphicsSystem *graphicsSystem);

		bool checkState(vector<Collider*> colliderList);//Collider* b);
		void fire(int spawnX, int spawnY, float xVelocity, float yVelocity);

		void setInUse(bool isInUse);
		bool isInUse();
		
		void setXBound(int xBound);
		void setYBound(int yBound);

};

#endif // !BULLET_H
