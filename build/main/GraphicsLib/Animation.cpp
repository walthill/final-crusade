#include "Animation.h"

Animation::Animation()
{
	animSpeed = 1.0;
	currentSpriteIndex = 0;
	isLooping = false;
	msPerFrame = 100;
	isAnimating = true;		
}


Animation::Animation(bool looping)
{
	currentSpriteIndex = 0;
	animSpeed = 1.0;
	msPerFrame = 100;
	isLooping = looping;
	isAnimating = true;
}


Animation::~Animation()
{
}


void Animation::addSpriteSheet(GraphicsBuffer *sprBuffer, int sheetRows, int sheetColumns, int spriteWidth, int spriteHeight)
{

	for (int i = 0; i < sheetRows; i++)
	{
		for (int j = 0; j < sheetColumns; j++)
		{
			Sprite spriteToInsert(sprBuffer, spriteWidth * j, spriteHeight* i, spriteWidth, spriteHeight);
			spriteList.push_back(spriteToInsert);
		}
	}
}


void Animation::update(double timeElapsed, bool playAnim)
{
		
	if (playAnim)
	{
		msPerFrame -= timeElapsed;

		if (msPerFrame < 0)
		{
			msPerFrame = 100 * animSpeed;

			if (currentSpriteIndex != spriteList.size() - 1)
				currentSpriteIndex++;
			else if (isLooping)
				currentSpriteIndex = 0;
		}
	}
}

void Animation::setLooping(bool loop)
{
	isLooping = loop;
}


void Animation::speedUpAnim()
{
	animSpeed -= 0.25;
}


void Animation::slowAnim()
{
	animSpeed += 0.25;
}


Sprite Animation::getCurrentSprite()
{
	return spriteList[currentSpriteIndex];
}

void Animation::setSpriteIndex(int index)
{
	currentSpriteIndex = index;
}

int Animation::getSpriteIndex()
{
	return currentSpriteIndex;
}

Animation& Animation::operator =(const Animation &rhs)
{
	spriteList = rhs.spriteList;
	currentSpriteIndex = rhs.currentSpriteIndex;
	isLooping = rhs.isLooping;
	animSpeed = rhs.animSpeed;
	isAnimating = rhs.isAnimating;

	return *this;
}
