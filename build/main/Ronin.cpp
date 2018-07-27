#include "Ronin.h"
#include "Game.h"

Ronin::Ronin()
{
	srand(unsigned(time(NULL)));
	
/*	random_device rd;
	uniform_int_distribution<int> randGen(0, 360);

	randGen(rd);*/

	rotate();
}

Ronin::~Ronin()
{
	//Game::getInstance()->mScore++;
}

/*void Ronin::destroy()
{

}*/

void Ronin::update(double timeElapsed)
{
	if (mIsVisible)
	{
		Entity::update(timeElapsed); //animate

		checkBounds();

		mXLoc += mXVelocity * mSpeed;
		mYLoc += mYVelocity * mSpeed; 
	}
	else if(destroyedLastFrame)
	{
		//tmp - add to score & combo
		cout << "ADD POINTS AND COMBO" << endl;

		Game::getInstance()->_Score += 450; //TODO: data driven - ronin point value
		Game::getInstance()->_ComboCount++;  
		//TODO(high): combo system - probably gonna need some ui work

		destroyedLastFrame = false;
	}
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
