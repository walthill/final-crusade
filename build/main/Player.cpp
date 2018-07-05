#include "Player.h"

Player::Player()
{
	mPlayerFrameSpeed = 10;
	mVelocity = 3;
	xBound = 790;
	yBound = 590; //screen sizes 
}

Player::~Player()
{

}

void Player::update(double timeElapsed, int mouseX, int mouseY)
{
	Entity::update(timeElapsed);

	

 	if (timeElapsed > mPlayerFrameSpeed)
	{
		rotate(mouseX, mouseY);

		checkBounds();
		move(mouseX, mouseY);
	}
}


void Player::checkBounds()
{
	if (mXLoc < 0)
		mXLoc = 0;
	if (mXLoc > xBound)
		mXLoc = xBound;
	if (mYLoc < 0)
		mYLoc = 0;
	if (mYLoc > yBound)
		mYLoc = yBound;
}

void Player::move(int mouseX, int mouseY)
{
	//Help from WAKS on the c++ forums @ https://bit.ly/2KHZL8T 

	if (mLeft)
	{
		mXLoc += -mVelocity;
	}
	if (mRight)
	{
		mXLoc += mVelocity;
	}
	if (mUp)
	{
		mYLoc += -mVelocity;
	}
	if (mDown)
	{
		mYLoc += mVelocity;
	}

	rotate(mouseX, mouseY);
}


void Player::rotate(int mouseX, int mouseY)
{
	//Math salvation from jordsti on StackOverflow @ https://bit.ly/2KrJx7Y

	if (mouseX != 0)
	{
		//NOTE: origin is in the top left
		dX = (mXLoc)-mouseX;
		dY = (mYLoc+ ROT_ALIGNMENT)-mouseY;

		angle = (atan2(dY, dX)*DEGREE_CONVERSION_VAL) / PI;
		setRotation(angle);
	}
}

void Player::setLeft(bool isMovingLeft)
{
	mLeft = isMovingLeft;
}

void Player::setRight(bool isMovingRight)
{
	mRight = isMovingRight;
}
void Player::setDown(bool isMovingDown)
{
	mDown = isMovingDown;
}
void Player::setUp(bool isMovingUp)
{
	mUp= isMovingUp;

}

int Player::getWidth()
{
	return mAnim.getCurrentSprite().getSpriteWidth();
}
int Player::getHeight()
{
	return mAnim.getCurrentSprite().getSpriteHeight();
}