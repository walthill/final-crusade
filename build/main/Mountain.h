#ifndef MOUNTAIN_H
#define MOUNTAIN_H

//#include "CossinTable.h"
#include "Player.h"

class Mountain : public Entity
{
	private:
		string enemyHit;

		enum State { IDLE, ALERT };
		int currentState = IDLE;
	
		const int HALF_FOV = 50;
		const int LINE_SIGHT_LENGTH = 110;
		const float PI = 3.1416f, DEGREE_CONVERSION_VALUE = 180.0000f;
		int mMapXBound = 1600, mMapYBound = 1200;
		
		float direction, angle;
		float mXVelocity, mYVelocity;
		float xDistance, yDistance;
		int dirToPlayer, relativeDirToPlayer;
		float dx, dy;
		int mAngle;
		int multiplier, mScoreValue;
		float mSpeed;

	public:
		Mountain();
		~Mountain();

		void init(int scoreValue, string hitAudioId);
		void update(double timeElapsed, Player *playerObj);
		void ai(Player *playerObj);
		bool hasLineOfSight(Player *player);

		void checkBounds();
		
		void rotate();
		void rotateToPlayer(float playerX, float playerY);
		float directionToPlayer(float playerX, float playerY);
};

#endif // !MOUNTAIN_H
