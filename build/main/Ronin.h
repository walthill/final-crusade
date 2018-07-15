#ifndef RONIN_H
#define RONIN_H

#include "Entity.h"
#include <time.h>

class Ronin : public Entity
{
	private:
		const double PI = 3.1416, DEGREE_CONVERSION_VALUE = 180.0000;
		int mMapXBound = 1600, mMapYBound=1200;
		double mXVelocity = 1, mYVelocity = 1, direction;
		int mAngle;

		double mSpeed = 2.1; //TODO: data driven here

	public:
		Ronin();
		~Ronin();

		void initEnemy(int levelW, int levelH);

		void update(double timeElapsed);

		
		void checkBounds();
		void rotate();

};

#endif // !RONIN_H
