#ifndef BULLET_POOL_H
#define BULLET_POOL_H

#include "Bullet.h"

class BulletPool
{
	//Game Programming Patterns was a great help https://bit.ly/2LrxN20

	private:
		int mFireRate = 16.7f;
		float mDeltaTime = 16.7;

		const float PI = 3.14156, DEGREE_CONVERSION_VALUE = 180;
	
		float bulletX, bulletY;

		static const int mPOOL_SIZE = 100;
		Bullet bullets[mPOOL_SIZE];

		Bullet *mFirstAvailable;
	
	public:
		BulletPool();
		~BulletPool();

		void initBulletData(Animation bulletSpriteData, int windowW, int windowH, bool isOwnerPlayer);


		void create(int x, int y, int xVelocity, int yVeloctity, bool isPlayerBullet);
		
		void initPool();
		void resetPool();

		void update(double timeElapsed);
		void BulletPool::draw(GraphicsSystem *graphicsSystem, int camX, int camY);

		void fireProjectile(double deltaTime, int playerX, int playerY, float direction);
};

#endif // !BULLET_POOL_H