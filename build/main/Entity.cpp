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
	mHasCollider = false;
	mHasCollided = false;
}


Entity::~Entity() 
{
}


void Entity::destroy()
{
	setVisible(false);
	destroyedLastFrame = true;
}

void Entity::update(double timeElapsed)
{
	mAnim.update(timeElapsed, mShouldAnimate);

	if (mHasCollider)
	{
		mThisCollider.setX(mXLoc);
		mThisCollider.setY(mYLoc);
	}
}

bool Entity::checkCollision(Collider *a, Collider *b)
{
	assert(mHasCollider); //you're checking for collisions w/o a collider

	if (b->getEntity()->isVisible())
	{
		//calculate sides of a
		mLeftA = a->getX();
		mRightA = mLeftA + a->getW();
		mTopA = a->getY();
		mBottomA = mTopA + a->getH();

		//calculate sides of b
		mLeftB = b->getX();
		mRightB = mLeftB + b->getW();
		mTopB = b->getY();
		mBottomB = mTopB + b->getH();

		//collision detection
		if (mBottomA <= mTopB)
		{
			return false;
		}
		if (mTopA >= mBottomB)
		{
			return false;
		}
		if (mRightA <= mLeftB)
		{
			return false;
		}
		if (mLeftA >= mRightB)
		{
			return false;
		}

		return true;

	}
	
	return false;
	
}


void Entity::draw(GraphicsSystem *graphicsSystem, int camX, int camY)
{
	if (mIsVisible)
	{
		Sprite curSpr = mAnim.getCurrentSprite();
		graphicsSystem->draw((int)mXLoc-camX, (int)mYLoc-camY, curSpr, mXScale, mYScale, mRotation);
	}
}


void Entity::draw(GraphicsSystem *graphicsSystem)
{
	if (mIsVisible)
	{
		Sprite curSpr = mAnim.getCurrentSprite();
		graphicsSystem->draw((int)mXLoc, (int)mYLoc, curSpr, mXScale, mYScale, mRotation);
	}
}


void Entity::setAnimation(Animation targetAnim)
{
	mAnim = targetAnim;
}

/*Animation Entity::getAnimation()
{
	return mAnim;
}*/

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


void Entity::setLoc(float x, float y)
{
	mXLoc = x;
	mYLoc = y;
}


float Entity::getX()
{
	return mXLoc;
}


float Entity::getY()
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


void Entity::setRotation(float rot)
{
	mRotation = rot;
}

float Entity::getRotation()
{
	return mRotation;
}

void Entity::setCollider(string colliderNameTag)
{
	mHasCollider = true;

	mThisCollider.initCollider(0, 0, mAnim.getCurrentSprite().getSpriteWidth(), mAnim.getCurrentSprite().getSpriteHeight(),
							   colliderNameTag, this);
}

void Entity::setCollider(int w, int h, string colliderNameTag)
{
	mHasCollider = true;

	mThisCollider.initCollider(0, 0, w, h, colliderNameTag, this);
}

Collider* Entity::getCollider()
{
	return &mThisCollider;
}