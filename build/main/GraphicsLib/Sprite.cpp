#include "Sprite.h"

Sprite::Sprite()
{

}


Sprite::Sprite(GraphicsBuffer *graphicsBufferPtr, int spriteX, int spriteY, int spriteWidth, int spriteHeight)
{
	mSpriteX = spriteX;
	mSpriteY = spriteY;

	mSpriteWidth = spriteWidth;
	mSpriteHeight = spriteHeight;

	mGraphicsBuffer = graphicsBufferPtr;

	cout << "Sprite initialized" << endl;
}


Sprite::~Sprite()
{
}


void Sprite::initSprite(GraphicsBuffer *graphicsBufferPtr, int spriteX, int spriteY,
				int spriteWidth, int spriteHeight)
{
	mSpriteX = spriteX;
	mSpriteY = spriteY;

	mSpriteWidth = spriteWidth;
	mSpriteHeight = spriteHeight;

	mGraphicsBuffer = graphicsBufferPtr;

	cout << "Sprite initialized" << endl;
}


GraphicsBuffer* Sprite::getBuffer()
{
	return mGraphicsBuffer;
}


int Sprite::getSpriteX()
{
	return mSpriteX;
}

int Sprite::getSpriteY()
{
	return mSpriteY;
}

int Sprite::getSpriteWidth()
{
	return mSpriteWidth;
}

int Sprite::getSpriteHeight()
{
	return mSpriteHeight;
}
