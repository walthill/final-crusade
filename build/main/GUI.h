#ifndef GUI_H
#define GUI_H

#include "Trackable.h"

#include "GraphicsSystem.h"
#include "Animation.h"
#include "Font.h"
#include "Color.h"
#include "Event.h"

class Gui : public Trackable
{
	private:
		const string mLOCAL_ASSET_PATH = "assets\\"; 
		const string mCOUR_FONT = "cour.ttf"; //see game.h variables

		int mRenderX, mRenderY;
		//string mTag;
		bool mShouldUpdate, mHasButton;
		
		struct TextUI
		{
			Font *mFontObj;
			string mText;
			Color mFontColor;
		}mTextUI;

		struct ButtonUI
		{
			int mButtonTextX, mButtonTextY;

			EventType mButtonType;
			GraphicsBuffer  mButtonBuffer;
			Animation mButtonAnim;

		}mButtonUI;

	public:
		Gui();
		~Gui();

		//Assigns GUI element a tag and a position
		void initGuiElement(/*string tag,*/ int x, int y);

		//Sets text for GUI element
		void addGuiText(int fontSize, Color fontColor, string msg);
		
		//Inits GUI element and creates text
		void initGuiElementWithText(/*string tag,*/ int x, int y, 
								   int fontSize, Color fontColor, string msg);


		//Sets button w/ text for gui element
		void addGuiButton(GraphicsBuffer buttonBuffer, EventType type, 
			              int sheetRows, int sheetCols, 
						  int spriteW, int spriteH, int fontSize, 
						  Color fontColor, string msg);

		//used for scores/timers etc
		void updateText(double timeElapsed, string strUpdate);
		void updateButton(double timeElapsed);


		void draw(GraphicsSystem *graphicsSystem);
		
		void setGuiToUpdate(bool shouldUpdate);
		bool shouldTextUpdate();
		bool hasButton();
		void setButton(bool selected);

		bool isButtonSelected();

		EventType getButtonType();
		//string getTag();
		int getX();
		int getY();
};

#endif // !GUI_H