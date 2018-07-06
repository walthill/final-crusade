#include "Player.h"

Player::Player()
{
	mPlayerFrameSpeed = 10;
	mVelocity = 3;
	//xBound = 1600;
	//yBound = 1200; //screen sizes 
}

Player::~Player()
{

}

void Player::init(int xBounds, int yBounds)
{
	mXBound = xBounds;
	mYBound = yBounds;
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

void Player::draw(GraphicsSystem *graphicsSystem, int camX, int camY)
{
	if (mIsVisible)
	{
		Sprite curSpr = mAnim.getCurrentSprite();

		//draw relative to camera position
		graphicsSystem->draw(mXLoc - camX, mYLoc - camY, curSpr, mXScale, mYScale, mRotation);
	}
}

void Player::checkBounds()
{
 	if (mXLoc < 0)
		mXLoc = 0;
	if (mXLoc > mXBound)
		mXLoc = mXBound;
	if (mYLoc < 0)
		mYLoc = 0;
	if (mYLoc > mYBound)
		mYLoc = mYBound;
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