#include "Ronin.h"

Ronin::Ronin()
{
	srand(unsigned(time(NULL)));
	rotate();
}

Ronin::~Ronin()
{
}

void Ronin::update(double timeElapsed)
{
	Entity::update(timeElapsed);

	checkBounds();

	mXLoc += mXVelocity*mSpeed;
	mYLoc += mYVelocity*mSpeed; // enemy should rotate in the direction of 

}

void Ronin::checkBounds()
{
	//this enemy roams the map 
	//when it reaches an edge, it rotates away and moves in a new direction

	if (mXLoc < 0 || mXLoc > mMapXBound || mYLoc < 0 || mYLoc > mMapYBound)
	{
		rotate();
	}
}

void Ronin::rotate()
{
	mAngle = rand() % 360;

	direction = mAngle;

	direction *= (PI / DEGREE_CONVERSION_VALUE);

	mXVelocity = -cos(direction);
	mYVelocity = -sin(direction);

	setRotation(mAngle);
}