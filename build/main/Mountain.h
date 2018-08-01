#ifndef MOUNTAIN_H
#define MOUNTAIN_H

#include "Entity.h"

class Mountain : public Entity
{
		const double PI = 3.1416, DEGREE_CONVERSION_VALUE = 180.0000;
		int mMapXBound = 1600, mMapYBound = 1200;
		double mXVelocity = 1, mYVelocity = 1, direction;
		int mAngle;

		int multiplier, mScoreValue;

		double mSpeed;

	public:
		Mountain();
		~Mountain();

//		void initEnemy(int levelW, int levelH);
		void update(double timeElapsed);

		void checkBounds();
		void rotate();
};

#endif // !MOUNTAIN_H
