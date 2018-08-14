#ifndef RONIN_H
#define RONIN_H

//#include "Game.h"
#include "Entity.h"
#include <time.h>

//class Game;

class Ronin : public Entity
{


	private:
		string enemyHit;
		const float PI = 3.1416f, DEGREE_CONVERSION_VALUE = 180.0000f;
		
		int mMapXBound, mMapYBound;
		float mXVelocity = 1, mYVelocity = 1, direction;
		int mAngle;

		int multiplier, mScoreValue;

		float mSpeed;// = 2.1;

	public:
		Ronin();
		~Ronin();

		void init(int scoreValue, string  hitAudioId);
		void update(double timeElapsed);
		
		void checkBounds();
		void rotate();



};

#endif // !RONIN_H
