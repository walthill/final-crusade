#include "BlockObject.h"

BlockObject::BlockObject()
{
}

BlockObject::~BlockObject()
{

}

//all the blocks are 4 bits.
//so this will get a vector[4] and use  
//its positions to set the blocks position
void BlockObject::initBlockObject(int numBlocks, vector<Vector2D> positions, Sprite& blockSpr, float speed, int offset, string spriteID)
{
	mNumBlocks = numBlocks;
	mBlockSpeed = (int)speed;
	mTimePassed = 0;
	mCanUpdate = true;

	mInitialPos.resize(mNumBlocks);
	mBlockVector.resize(mNumBlocks);

	mSpriteSize = (float)blockSpr.getSpriteHeight();

	offset *= (int)mSpriteSize;

	for(int i = 0; i < mNumBlocks; i++)
	{
		mInitialPos[i].setX(positions[i].getX()*mSpriteSize + offset);
		mInitialPos[i].setY(positions[i].getY()*mSpriteSize);
		mBlockVector[i].mPos.setX(positions[i].getX() * mSpriteSize + offset);
		mBlockVector[i].mPos.setY(positions[i].getY() * mSpriteSize);
		mBlockVector[i].mSprite = blockSpr;
		mBlockVector[i].mID = spriteID;
	}
}

void BlockObject::reInitBlockObject()
{
	mTimePassed = 0;
	mCanUpdate = true;

	for (int i = 0; i < mNumBlocks; i++)
	{
		mBlockVector[i].mPos.setX(mInitialPos[i].getX());
		mBlockVector[i].mPos.setY(mInitialPos[i].getY());
	}
}

void BlockObject::update(double timeElapsed, BlockPile &blockPile, bool canMoveY)
{
	mTimePassed += (float)timeElapsed;
	//go down one bit per however long
	if (mTimePassed > mBlockSpeed)
	{
		if (canMoveY)
			moveBlockY();

		mTimePassed = 0;
	}
	
	for (int i = 0; i < mNumBlocks; i++)
	{	 
		if (blockPile.getBlockBitOccupy(mBlockVector[i].mPos.getX(), mBlockVector[i].mPos.getY()) == true)
		{ 
			//TODO: Make this a for loop, it really should be.
			//Also, the lose consdition procs, but does nothing. Make it do something

			//if ((mBlockVector[0].mPos.getY()) == 0 || (mBlockVector[1].mPos.getY()) == 0 || (mBlockVector[2].mPos.getY()) == 0 || (mBlockVector[3].mPos.getY()) == 0)
			//{
			//	cout << "GAME OVER " << endl;
			//	break;
			//}

			mBlockVector[0].mPos.setY(mBlockVector[0].mPos.getY() - mSpriteSize);
			mBlockVector[1].mPos.setY(mBlockVector[1].mPos.getY() - mSpriteSize);
			mBlockVector[2].mPos.setY(mBlockVector[2].mPos.getY() - mSpriteSize);
			mBlockVector[3].mPos.setY(mBlockVector[3].mPos.getY() - mSpriteSize);

			blockPile.addBlockToPile(mBlockVector[0].mPos, mBlockVector[0].mSprite, mBlockVector[0].mID);
			blockPile.addBlockToPile(mBlockVector[1].mPos, mBlockVector[1].mSprite, mBlockVector[1].mID);
			blockPile.addBlockToPile(mBlockVector[2].mPos, mBlockVector[2].mSprite, mBlockVector[2].mID);
			blockPile.addBlockToPile(mBlockVector[3].mPos, mBlockVector[3].mSprite, mBlockVector[3].mID);
		
			mCanUpdate = false;
			break;
		}
	}
}

void BlockObject::draw(GraphicsSystem *graphicsSystem)
{
	for (int i = 0; i < mNumBlocks; i++)
	{ 
		graphicsSystem->draw((int)(mBlockVector[i].mPos.getX() + 300), (int)mBlockVector[i].mPos.getY(), mBlockVector[i].mSprite, 1.0, 1.0);
	}
}

void BlockObject::drawNext(GraphicsSystem *graphicsSystem)
{
	for (int i = 0; i < mNumBlocks; i++)
	{
		graphicsSystem->draw((int)(mBlockVector[i].mPos.getX() + 500), (int)mBlockVector[i].mPos.getY()+150, mBlockVector[i].mSprite, 1.0, 1.0);
	}
}

void BlockObject::moveBlockX(int direction, BlockPile &blockPile)
{
	for (int i = 0; i < mNumBlocks; i++)
	{
		if (blockPile.getBlockBitOccupy(mBlockVector[i].mPos.getX()+ mSpriteSize * direction, mBlockVector[i].mPos.getY()) == true)
		{
			direction = 0;
		}
	}

	for (int i = 0; i < mNumBlocks; i++)
	{
		mBlockVector[i].mPos.setX(mBlockVector[i].mPos.getX() + mSpriteSize*direction);
	}
}

void BlockObject::moveBlockY()
{
	for (int i = 0; i < mNumBlocks; i++)
	{
		mBlockVector[i].mPos.setY(mBlockVector[i].mPos.getY() + mSpriteSize);
	}
}

void BlockObject::rotateBlock(BlockPile &blockPile)
{
	//rotation shouldn't place the block in a spot where it could clip into the ground.
	//rotation shouldn't allow the player to rotate if it would go into other blocks.
	int tempX;
	bool alreadyOccupied = false;
	int normPosX = (int)mBlockVector[0].mPos.getX();
	int normPosY = (int)mBlockVector[0].mPos.getY();

	for (int i = 0; i < mNumBlocks; i++)
	{
		mBlockVector[i].mPos.setX(mBlockVector[i].mPos.getX() - normPosX);
		mBlockVector[i].mPos.setY(mBlockVector[i].mPos.getY() - normPosY);

		tempX = (int)mBlockVector[i].mPos.getX();
		mBlockVector[i].mPos.setX(mBlockVector[i].mPos.getY()*-1);
		mBlockVector[i].mPos.setY((float)tempX);

		if (blockPile.getBlockBitOccupy((mBlockVector[i].mPos.getX() + normPosX), (mBlockVector[i].mPos.getY() + normPosY)) == true)
		{
			alreadyOccupied = true;
		}

		//these were OG, the rest of if can change
		mBlockVector[i].mPos.setX(mBlockVector[i].mPos.getX() + normPosX);
		mBlockVector[i].mPos.setY(mBlockVector[i].mPos.getY() + normPosY);
	}

	if (alreadyOccupied)
	{
		for (int i = 0; i < mNumBlocks; i++)
		{
			mBlockVector[i].mPos.setX(mBlockVector[i].mPos.getX() - normPosX);
			mBlockVector[i].mPos.setY(mBlockVector[i].mPos.getY() - normPosY);

			tempX = (int)mBlockVector[i].mPos.getY();
			mBlockVector[i].mPos.setY(mBlockVector[i].mPos.getX()*-1);
			mBlockVector[i].mPos.setX((float)tempX);

			mBlockVector[i].mPos.setX(mBlockVector[i].mPos.getX() + normPosX);
			mBlockVector[i].mPos.setY(mBlockVector[i].mPos.getY() + normPosY);
		}
	
	}

}

void BlockObject::setSpeed(int speed)
{
	mBlockSpeed = speed;
}

void BlockObject::setBlockX(int index, int xPos)
{
	mBlockVector[index].mPos.setX((float)xPos);
}

void BlockObject::setBlockY(int index, int yPos)
{
	mBlockVector[index].mPos.setY((float)yPos);
}


int BlockObject::getBlockX(int index)
{
	return (int)mBlockVector[index].mPos.getX();
}

int BlockObject::getBlockY(int index)
{
	return (int)mBlockVector[index].mPos.getY();
}