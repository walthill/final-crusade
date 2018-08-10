#ifndef BULLET_POOL_H
#define BULLET_POOL_H

#include "Bullet.h"

class BulletPool
{
	//Game Programming Patterns was a great help https://bit.ly/2LrxN20

	private:
		double mFireRate = 16.7;
		double mDeltaTime = 16.7;

		const float PI = 3.14156f, DEGREE_CONVERSION_VALUE = 180.0f;
	
		float bulletX, bulletY;

		int mPoolSize;
		Bullet *bullets;// [mPOOL_SIZE];

		Bullet *mFirstAvailable;
	
	public:
		BulletPool();
		~BulletPool();

		void initBulletData(int poolSize, int bulletSpeed, Animation bulletSpriteData,
							int levelW, int levelH, string colliderTag);


		void create(int x, int y, int xVelocity, int yVeloctity, bool isPlayerBullet);
		
		void initPool();
		void resetPool();

		void update(double timeElapsed, vector<Collider*> colliderList);//Collider* b);
		void draw(GraphicsSystem *graphicsSystem, int camX, int camY);

		void fireProjectile(double deltaTime, float playerX, float playerY, float direction);

		Collider* getBulletCollider(int index);
		int getPoolSize();
};

#endif // !BULLET_POOL_H
