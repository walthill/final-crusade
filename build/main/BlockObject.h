#pragma once
#include "GraphicsSystem.h"
#include "Animation.h"
#include "MemoryTracker.h"
#include "PerformanceTracker.h"
#include <Vector2D.h>
#include <vector>
#include "BlockPile.h"

class BlockPile;

struct block
{
	Vector2D mPos;
	Sprite mSprite;
	string mID;
};

class BlockObject : public Trackable
{
private:
	//names can be subject to change
	vector<block> mBlockVector;
	vector<Vector2D> mInitialPos;

	int mNumBlocks;
	int mBlockSpeed;
	float mTimePassed;
	float mSpriteSize;

	bool mCanUpdate;
public:
	BlockObject();
	~BlockObject();

	void initBlockObject(int numBlocks, vector<Vector2D> positions, Sprite& blockSpr, float speed, int offset, string spriteID);
	void update(double timeElapsed, BlockPile &blockPile, bool canMoveY);
	void draw(GraphicsSystem *graphicsSystem);
	void drawNext(GraphicsSystem *graphicsSystem);
	void moveBlockX(int direction, BlockPile &blockPile);
	void moveBlockY();
	void rotateBlock(BlockPile &blockPile);
	void reInitBlockObject();

	bool checkUpdate(){return mCanUpdate;};
	void setSpeed(int speed);

	void setBlockX(int index, int yPos);
	void setBlockY(int index, int yPos);
	int getBlockX(int index);
	int getBlockY(int index);

};

