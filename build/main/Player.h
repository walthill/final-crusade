#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"


class Player : public Entity
{
	private:
		//rotation math vars
		double dX, dY, angle;
		const double PI = 3.1416, DEGREE_CONVERSION_VAL = 180.0000;

		const int ROT_ALIGNMENT = 8;

		//movement vars
		bool mLeft, mRight, mUp, mDown;
		int mPlayerFrameSpeed, mVelocity; 
		int mXBound, mYBound;//TODO: data driven here

	public:
		Player();
		~Player();

		void init(int xBounds, int yBounds);

		void update(double timeElapsed, int mouseX, int mouseY);
		void draw(GraphicsSystem *graphicsSystem, int camX, int camY);

		void checkBounds();
		void move(int mouseX, int mouseY);
		void rotate(int mouseX, int mouseY);

		void setLeft(bool isMovingLeft);
		void setRight(bool isMovingRight);
		void setDown(bool isMovingDown);
		void setUp(bool isMovingUp);

		int getWidth();
		int getHeight();
};

#endif //!PLAYER_H