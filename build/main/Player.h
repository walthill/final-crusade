#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"

class Player : public Entity
{
	private:
		//rotation math vars
		int dX; 
		int dY; 
		double angle;
		const double PI = 3.1416, DEGREE_CONVERSION_VAL = 180.0000;

		//movement vars
		bool mLeft, mRight, mUp, mDown;
		int mPlayerFrameSpeed, mVelocity; 
		int xBound, yBound;//TODO: data driven here

	public:
		Player();
		~Player();

		void update(double timeElapsed, int mouseX, int mouseY);

		void checkBounds();
		void move();
		void rotate(int mouseX, int mouseY);

		void setLeft(bool isMovingLeft);
		void setRight(bool isMovingRight);
		void setDown(bool isMovingDown);
		void setUp(bool isMovingUp);
};

#endif //!PLAYER_H