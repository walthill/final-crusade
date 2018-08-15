#ifndef FRAGMENT_H
#define FRAGMENT_H

#include "Player.h"

class Fragment : public Entity
{
	private:
		string fragmentPickup;

		const float PI = 3.1416f, DEGREE_CONVERSION_VALUE = 180.0000f;
		const int LINE_SIGHT_LENGTH = 48;
		const float SPEED_TOWARDS_PLAYER = 7.0f;
		bool collisionDetected, lineOfSightTriggered, addPoints;

		float direction, dx, dy, angle, xDistance, yDistance,
			  mXVelocity, mYVelocity;
	public:
		Fragment();
		~Fragment();

		void init(string pickupAudioId);

		void update(double timeElapsed, Player *playerObj);

		bool hasLineOfSight(Player *playerObj);
		void rotateToPlayer(float playerX, float playerY);
		float directionToPlayer(float playerX, float playerY);

};

#endif // !FRAGMENT_H