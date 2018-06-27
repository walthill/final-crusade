#pragma once
#include "GraphicsSystem.h"
#include "Animation.h"
#include "MemoryTracker.h"
#include "PerformanceTracker.h"
#include "BlockObject.h"
#include <Vector2D.h>
#include <vector>

struct PileBit
{
	Vector2D mPos;
	Sprite mSprite;
	bool mOccupied = false;
	string mSpriteId;
};

class BlockPile : public Trackable
{
private:
	vector<PileBit> mPileVector;
	int mNumPileBits;
	int mSpriteWidth;
	int mGridWidth;
	int mGridHeight;

public:
	BlockPile();
	~BlockPile();

	void initBlockPile(Sprite& blockSprite, int gridWidth, int gridHeight, string spriteId);
	void addBlockToPile(Vector2D blockToAddPos, Sprite &spriteToAdd, string spriteID);
	bool getBlockBitOccupy(float posX, float posY);
	int checkPileFull();
	bool checkLineEmpty(int y);
	void clearLine(int y);
	void draw(GraphicsSystem *graphicsSystem);

	Vector2D getLoc(int index);
	int getSize();

	int getBitX(int index);
	int getBitY(int index);
	bool getIsOccupied(int index);
	string getID(int index);

	void setSize(int size);

	void setBitX(int index, int x);
	void setBitY(int index, int y);
	void setIsOccupied(int index, bool occupied);
	void setSprite(int index, Sprite &sprite);
	void setID(int index, string spriteID);
};