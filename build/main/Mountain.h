#ifndef MOUNTAIN_H
#define MOUNTAIN_H

#include "Player.h"

class Mountain : public Entity
{
		enum State { IDLE, ALERT };
		int currentState = IDLE;
	
		const double PI = 3.1416, DEGREE_CONVERSION_VALUE = 180.0000;
		int mMapXBound = 1600, mMapYBound = 1200;
		
		double mXVelocity = 1, mYVelocity = 1, direction, angle;
		double oldXVelocity, oldYVelocity, oldAngle;
		double xDistance, yDistance;
		int dirToPlayer, relativeDirToPlayer;
		int halfFOV = 45;
		double dx, dy;
		double dtTime;
		bool chasing;

		double mFrameSpeed;
		bool stateChanged;

		int mAngle;

		int multiplier, mScoreValue;

		double mSpeed;

	public:
		Mountain();
		~Mountain();

//		void initEnemy(int levelW, int levelH);
		void update(double timeElapsed, Player *playerObj);
		
		bool hasLineOfSight(Player *player);

		void checkBounds();
		
		void rotate();
		void rotateToPlayer(int playerX, int playerY);
		double directionToPlayer(int playerX, int playerY);
};

#endif // !MOUNTAIN_H
