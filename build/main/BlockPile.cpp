#include "BlockPile.h"

BlockPile::BlockPile()
{

}

BlockPile::~BlockPile()
{

}

//change this to a better variable thing
void BlockPile::initBlockPile(Sprite& blockSprite, int gridWidth, int gridHeight, string spriteId)
{
	//the sprite id parameter will be used for saving/loading??

	mGridWidth = gridWidth;
	mGridHeight = gridHeight;	
	mSpriteWidth = blockSprite.getSpriteWidth();

	/*
	2D array as a 1D arry:
	new array[width * height];
	x and y are the 2D coords. like array[x,y]

	To get the index at a theoretical x and y:
	index = y*width+x 

	If you don't know the x and y, you can get them like this:
	y = index / width
	x = index % width
	*/

	mNumPileBits = mGridWidth * mGridHeight;
	mPileVector.resize(mNumPileBits);

	//This resets the board. Might be useful
	for (int i = 0; i < mNumPileBits; i++)
	{
		mPileVector[i].mPos.setX(0);
		mPileVector[i].mPos.setY(0);
		mPileVector[i].mOccupied = false;
		mPileVector[i].mSpriteId = spriteId;
	}

	for (int i = 0; i < mGridWidth; i++)
	{
		mPileVector[i].mSprite = blockSprite;
		mPileVector[i].mPos.setX((float)(i * mSpriteWidth));
		mPileVector[i].mPos.setY((float)(mSpriteWidth * (mGridHeight-1)));
		mPileVector[i].mOccupied = true;		
		mPileVector[i].mSpriteId = spriteId;
	}

	for (int i = 1; i < mGridHeight; i++)
	{
		mPileVector[i*mGridWidth].mSprite = blockSprite;
		mPileVector[i*mGridWidth].mPos.setX(0);
		mPileVector[i*mGridWidth].mPos.setY((float)((i - 1) * mSpriteWidth));
		mPileVector[i*mGridWidth].mOccupied = true;
		mPileVector[i*mGridWidth].mSpriteId = spriteId;
	}

	for (int i = 1; i < mGridHeight; i++)
	{
		mPileVector[i*mGridWidth + (mGridWidth -1)].mSprite = blockSprite;
		mPileVector[i*mGridWidth + (mGridWidth -1)].mPos.setX((float)((mGridWidth -1) * mSpriteWidth));
		mPileVector[i*mGridWidth + (mGridWidth -1)].mPos.setY((float)((i - 1) * mSpriteWidth));
		mPileVector[i*mGridWidth + (mGridWidth -1)].mOccupied = true;
		mPileVector[i*mGridWidth + (mGridWidth - 1)].mSpriteId = spriteId;
	}

	
	//mNumPileBits = width + (depth*2);//num kill bits

	//mPileVector.resize(mNumPileBits);

	//for (int i = 0; i < width; i++)
	//{
	//	mPileVector[i].mSprite = blockSprite;
	//	mPileVector[i].mPos.setX(i * spriteWidth);
	//	mPileVector[i].mPos.setY(spriteWidth * depth);
	//	mPileVector[i].mOccupied = true;
	//}

	//for (int i = width; i-width < depth; i++)
	//{
	//	mPileVector[i].mSprite = blockSprite;
	//	mPileVector[i].mPos.setX(0);
	//	mPileVector[i].mPos.setY((i-width) * spriteWidth);
	//	mPileVector[i].mOccupied = true;
	//}

	//for (int i = width+depth; i-(width+depth) < depth; i++)
	//{
	//	mPileVector[i].mSprite = blockSprite;
	//	mPileVector[i].mPos.setX((width-1) * spriteWidth);
	//	mPileVector[i].mPos.setY((i-(width+depth)) * spriteWidth);
	//	mPileVector[i].mOccupied = true;
	//}
}

//give entire object instead of parts. 
void BlockPile::addBlockToPile(Vector2D blockToAddPos, Sprite &spriteToAdd, string spriteID)
{
	//mNumPileBits ++;
	//mPileVector.resize(mNumPileBits);
	int x = (int)(blockToAddPos.getX() / 16);
	int y = (int)(29-(blockToAddPos.getY() / 16));

	//mPileVector[mNumPileBits-1].mOccupied = true;
	//mPileVector[mNumPileBits - 1].mPos.setX(blockToAddPos.getX());
	//mPileVector[mNumPileBits - 1].mPos.setY(blockToAddPos.getY());
	//mPileVector[mNumPileBits - 1].mSprite = spriteToAdd;

	mPileVector[y * mGridWidth + x].mOccupied = true;
	mPileVector[y * mGridWidth + x].mPos.setX(blockToAddPos.getX());
	mPileVector[y * mGridWidth + x].mPos.setY(blockToAddPos.getY());
	mPileVector[y * mGridWidth + x].mSprite = spriteToAdd;
	mPileVector[y * mGridWidth + x].mSpriteId = spriteID;
}

bool BlockPile::getBlockBitOccupy(float posX, float posY)
{
	for (int i = 0; i < mNumPileBits; i++)
	{
		if (mPileVector[i].mPos.getX() == posX && mPileVector[i].mPos.getY() == posY)
		{
			return true;
		}
	}
	return false;
}

int BlockPile::checkPileFull()
{
	int lineY = 1;
	int linesCleared = 0;
	bool goForClear = true;

	do
	{
		for (int i = 1; i < mGridWidth -1; i++)
		{
			if (mPileVector[lineY * mGridWidth + i].mOccupied == false)
			{
				goForClear = false;
			}
		}

		if(goForClear == true)
		{
			linesCleared++;
			clearLine(lineY--);
		}
		goForClear = true;
	}
	while(checkLineEmpty(++lineY) == false);

	if (lineY == mGridHeight || lineY==(mGridHeight-1))
		linesCleared = -1;

	return linesCleared;
}

bool BlockPile::checkLineEmpty(int y)
{
	if (y < mGridHeight)
	{
		for (int i = 1; i < 13 - 1; i++)
		{
			if (mPileVector[y * mGridWidth + i].mOccupied == true)
			{
				return false;
			}
		}
	}
	
	return true;
}

void BlockPile::clearLine(int y)
{
	//clears the line
	for(int i = 1; i < mGridWidth -1; i++)
	{
		mPileVector[y * mGridWidth + i].mPos.setX(0);
		mPileVector[y * mGridWidth + i].mPos.setY(0);
		mPileVector[y * mGridWidth + i].mOccupied = false;
	}

	//brings everything else down
	while(checkLineEmpty(++y) == false)
	{
		for (int i = 1; i < mGridWidth -1; i++)
		{
			//copy the contents of the y to the y-1
			if (mPileVector[y * mGridWidth + i].mOccupied == true)
			{ 
				mPileVector[(y - 1) * mGridWidth + i].mPos.setX(mPileVector[y * mGridWidth + i].mPos.getX());
				mPileVector[(y - 1) * mGridWidth + i].mPos.setY(mPileVector[y * mGridWidth + i].mPos.getY()+mSpriteWidth);
				mPileVector[(y - 1) * mGridWidth + i].mSprite = mPileVector[y * mGridWidth + i].mSprite;
				mPileVector[(y - 1) * mGridWidth + i].mSpriteId = mPileVector[y * mGridWidth + i].mSpriteId;
				mPileVector[(y - 1) * mGridWidth + i].mOccupied = true;
			}
			//clear the y-1
			mPileVector[y * mGridWidth + i].mPos.setX((float)i);
			mPileVector[y * mGridWidth + i].mPos.setY(0);
			mPileVector[y * mGridWidth + i].mOccupied = false;
		}
	}
}


void BlockPile::draw(GraphicsSystem *graphicsSystem)
{
	for (int i = 0; i < mNumPileBits; i++)
	{
		if (mPileVector[i].mOccupied == true)
			graphicsSystem->draw((int)(mPileVector[i].mPos.getX() + 300), (int)(mPileVector[i].mPos.getY()), mPileVector[i].mSprite, 1.0, 1.0);
	}
}

int BlockPile::getSize()
{
	return mPileVector.size();
}

Vector2D BlockPile::getLoc(int index)
{
	return mPileVector[index].mPos;
}


int BlockPile::getBitX(int index)
{
	return (int)mPileVector[index].mPos.getX();
}

int BlockPile::getBitY(int index)
{
	return (int)mPileVector[index].mPos.getY();
}

bool BlockPile::getIsOccupied(int index)
{
	return mPileVector[index].mOccupied;
}

string BlockPile::getID(int index)
{
	return mPileVector[index].mSpriteId;
}

void BlockPile::setSize(int size)
{
	mPileVector.resize(size);
	mNumPileBits = size;
}

void BlockPile::setBitX(int index, int x)
{
	mPileVector[index].mPos.setX((float)x);
}
void BlockPile::setBitY(int index, int y)
{
	mPileVector[index].mPos.setY((float)y);
}

void BlockPile::setIsOccupied(int index, bool occupied)
{
	mPileVector[index].mOccupied = occupied;
}

void BlockPile::setSprite(int index, Sprite &sprite)
{
	mPileVector[index].mSprite = sprite;
}

void BlockPile::setID(int index, string spriteID)
{
	mPileVector[index].mSpriteId = spriteID;
}

