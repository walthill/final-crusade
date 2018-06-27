#ifndef SPRITE
#define SPRITE

#include "GraphicsBuffer.h"

class Sprite : public Trackable
{
private:

	GraphicsBuffer * mGraphicsBuffer;
	int mSpriteX, mSpriteY;
	int mSpriteWidth, mSpriteHeight;

public:
	
	Sprite();
	Sprite(GraphicsBuffer *graphicsBufferPtr, int spriteX, int spriteY,
		int spriteWidth, int spriteHeight);

	~Sprite();

	void initSprite(GraphicsBuffer *graphicsBufferPtr, int spriteX, int spriteY,
		int spriteWidth, int spriteHeight);

	GraphicsBuffer* getBuffer();

	//Retrieve sprite information
	int getSpriteX();
	int getSpriteY();

	int getSpriteWidth();
	int getSpriteHeight();

};

#endif