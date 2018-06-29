#include "Entity.h"

Entity::Entity()
{
	mVisibility = true;
	mXLoc = 0;
	mYLoc = 0;
	mXScale = 1.0;
	mYScale = 1.0;
	mShouldAnimate = true;
}


Entity::~Entity() 
{
}


void Entity::update(double timeElapsed)
{
	mAnim.update(timeElapsed, mShouldAnimate);
}


void Entity::draw(GraphicsSystem *graphicsSystem)
{
	if (mVisibility)
	{
		Sprite curSpr = mAnim.getCurrentSprite();
//		graphicsSystem->draw(xLoc, yLoc, curSpr, 1.0, 1.0);
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
	return mVisibility;
}


void Entity::setVisible(bool setVisibilty)
{
	mVisibility = setVisibilty;
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