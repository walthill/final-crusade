#include "Player.h"

Player::Player()
{
	mPlayerFrameSpeed = 10;
	mVelocity = 3;
	mXBound = 0;
	mYBound = 0; 
	mScreenXSize = 0;
	mScreenYSize = 0; //screen sizes 
}

Player::~Player()
{

}

void Player::init(int xBounds, int yBounds, int screenXSize, int screenYSize)
{
	mXBound = xBounds;
	mYBound = yBounds;
	mScreenXSize = screenXSize;
	mScreenYSize = screenYSize;
}

void Player::update(double timeElapsed, Collider *b, int mouseX, int mouseY, int camX, int camY)
{
	Entity::update(timeElapsed);

 	if (timeElapsed > mPlayerFrameSpeed)
	{
		rotate(mouseX, mouseY, camX, camY);

		checkBounds();
		
		if (checkCollision(mThisCollider, *b))
		{
			if (b->getTag() == "ronin")
				cout << "ENEMY COLLISION" << endl; //TODO(high): player death
		}

		move(mouseX, mouseY, camX, camY);
	}
}

/*void Player::draw(GraphicsSystem *graphicsSystem, int camX, int camY)
{
	if (mIsVisible)
	{
		Sprite curSpr = mAnim.getCurrentSprite();

		//draw relative to camera position
		graphicsSystem->draw(mXLoc - camX, mYLoc - camY, curSpr, mXScale, mYScale, mRotation);
	}
}*/

void Player::checkBounds()
{
 	if (mXLoc < 0)
		mXLoc = 1;
	if (mXLoc > mXBound)
		mXLoc = mXBound-5;
	if (mYLoc < 0)
		mYLoc = 1;
	if (mYLoc > mYBound)
		mYLoc = mYBound-5;
}


/*bool Player::checkCollision(Collider a, Collider b)
{

}*/

void Player::move(int mouseX, int mouseY, int camX, int camY)
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

	rotate(mouseX, mouseY, camX, camY);
}


void Player::rotate(int mouseX, int mouseY, int camX, int camY)
{
	//Math salvation from jordsti on StackOverflow @ https://bit.ly/2KrJx7Y

	if (mouseX != 0)
	{
		//subtract camera position to accurately rotate outside of screen space coordinates
		dX = (mXLoc - camX) - mouseX;
		dY = (mYLoc + ROT_ALIGNMENT - camY) - mouseY;

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