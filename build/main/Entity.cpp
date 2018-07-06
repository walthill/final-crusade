#include "Entity.h"

Entity::Entity()
{
	mIsVisible = true;
	mXLoc = 0;
	mYLoc = 0;
	mXScale = 1.0;
	mYScale = 1.0;
	mRotation = 0;
	mShouldAnimate = true;
}


Entity::~Entity() 
{
}


void Entity::update(double timeElapsed)
{
 	mAnim.update(timeElapsed, mShouldAnimate);
}


void Entity::draw(GraphicsSystem *graphicsSystem, int camX, int camY)
{
	if (mIsVisible)
	{
		Sprite curSpr = mAnim.getCurrentSprite();
		graphicsSystem->draw(mXLoc-camX, mYLoc-camY, curSpr, mXScale, mYScale, mRotation);
	}
}


void Entity::draw(GraphicsSystem *graphicsSystem)
{
	if (mIsVisible)
	{
		Sprite curSpr = mAnim.getCurrentSprite();
		graphicsSystem->draw(mXLoc, mYLoc, curSpr, mXScale, mYScale, mRotation);
	}
}


void Entity::setAnimation(Animation targetAnim)
{
	mAnim = targetAnim;
}


Animation* Entity::getAnimation()
{
	return &mAnim;
}


void Entity::shouldAnimate(bool pause)
{
	mShouldAnimate = pause;
}



bool Entity::isVisible()
{
	return mIsVisible;
}


void Entity::setVisible(bool setVisibilty)
{
	mIsVisible = setVisibilty;
}


void Entity::setLoc(int x, int y)
{
	mXLoc = x;
	mYLoc = y;
}


double Entity::getX()
{
	return mXLoc;
}


double Entity::getY()
{
	return mYLoc;
}


void Entity::setScale(float x, float y)
{
	mXScale = x;
	mYScale = y;
}


float Entity::getXScale()
{
	return mXScale;
}


float Entity::getYScale()
{
	return mYScale;
}


void Entity::setRotation(double rot)
{
	mRotation = rot;
}

double Entity::getRotation()
{
	return mRotation;
}