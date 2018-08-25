#include "GUI.h"

Gui::Gui()
{
	mRenderX = 0;
	mRenderY = 0;
	//mTag = "general";
	//mShouldUpdate = false;
	mHasButton = false;

	mTextUI = {};
	//mButtonUI = {};
}


Gui::~Gui()
{
	delete mTextUI.mFontObj;
	mTextUI.mFontObj = NULL;
}


void Gui::initGuiElement(/*string tag, */int x, int y)
{
	mRenderX = x;
	mRenderY = y;
//	mTag = tag;
}


void Gui::addGuiText(int fontSize, string fontFilename, Color fontColor, string msg)
{
	mTextUI.mFontObj = new Font(mLOCAL_ASSET_PATH + fontFilename, fontSize);
	mTextUI.mFontColor = fontColor;
	mTextUI.mText = msg;
}


void Gui::initGuiElementWithText(/*string tag, */int x, int y, int fontSize, string fontFilename, Color fontColor, string msg)
{
	mRenderX = x;
	mRenderY = y;
	//mTag = tag;

	mTextUI.mFontObj = new Font(mLOCAL_ASSET_PATH + fontFilename, fontSize);
	mTextUI.mFontColor = fontColor;
	mTextUI.mText = msg;
}

void Gui::addGuiButton(GraphicsBuffer buttonBuffer, EventType type, int sheetRows, int sheetCols,
					   int spriteW, int spriteH, int fontSize, string fontFilename, Color fontColor, string msg)
{
	mHasButton = true;
	mButtonUI.mButtonType = type;

	//set text within button
	mButtonUI.mButtonTextX = mRenderX + 10;
	mButtonUI.mButtonTextY = mRenderY + 10;

	addGuiText(fontSize, fontFilename, fontColor, msg);

	mButtonUI.mButtonBuffer = buttonBuffer;

	//buttons should be loaded with the button image and the button selected image	
	mButtonUI.mButtonAnim.addSpriteSheet(&mButtonUI.mButtonBuffer,
										 sheetRows, sheetCols, 
										 spriteW, spriteH);

	if (mButtonUI.mButtonType == NEW_GAME || mButtonUI.mButtonType == RETURN_STATS || mButtonUI.mButtonType == RETURN_CREDITS || 
		mButtonUI.mButtonType == SHOW_CONTROLS || mButtonUI.mButtonType == RESUME)
		
		mButtonUI.mButtonAnim.setSpriteIndex(1);
}



void Gui::updateText(double timeElapsed, string strUpdate)
{
//	if (mShouldUpdate)
		mTextUI.mText = strUpdate;
}

void Gui::updateButton(double timeElapsed)
{
	if (mButtonUI.mButtonAnim.getSpriteIndex() == 0)
		mButtonUI.mButtonAnim.setSpriteIndex(1); 
	
	else //selected
		mButtonUI.mButtonAnim.setSpriteIndex(0);

}

void Gui::draw(GraphicsSystem *graphicsSystem)
{
	//draw button
	if (mHasButton)
	{
		Sprite curSpr = mButtonUI.mButtonAnim.getCurrentSprite();
		graphicsSystem->draw(mRenderX, mRenderY, curSpr, 1.0, 1.0);

		//draw text
		graphicsSystem->writeTextToBackbuffer(mButtonUI.mButtonTextX, mButtonUI.mButtonTextY,
											 *mTextUI.mFontObj, mTextUI.mFontColor,
											  mTextUI.mText);
	}
	else //draw element w/o button
	{
		//draw text
		graphicsSystem->writeTextToBackbuffer(mRenderX, mRenderY,
											 *mTextUI.mFontObj, mTextUI.mFontColor,
											  mTextUI.mText);
	}
		
}

void Gui::setGuiToUpdate(bool shouldUpdate)
{
	mShouldUpdate = shouldUpdate;
}

bool Gui::shouldTextUpdate()
{
	return mShouldUpdate;
}

void Gui::setButton(bool selected)
{
	if(selected)
		mButtonUI.mButtonAnim.setSpriteIndex(1);
}

bool Gui::hasButton()
{
	return mHasButton;
}

bool Gui::isButtonSelected()
{
	bool result = false;

	if (mButtonUI.mButtonAnim.getSpriteIndex() == 1)
		result = true;
	
	return result;
}

EventType Gui::getButtonType()
{
	return mButtonUI.mButtonType;
}

/*string Gui::getTag()
{
	//return mTag;
}*/

int Gui::getX()
{
	return mRenderX;
}


int Gui::getY()
{
	return mRenderY;
}

void Gui::setTextColor(Color textColor)
{
	mTextUI.mFontColor = textColor;
}