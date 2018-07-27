#ifndef GUI_MAN_H
#define GUI_MAN_H

#include <map>
#include "GUI.h"

typedef string GuiID;

class GUIManager : public Trackable
{
	private:
		map<GuiID, Gui*> mGuiMap;
		int mCurrentButton;
		int mNumButtons;
		 
		//elements with the given tag or with the global tag "general"
		
	public:

		bool buttonPressed, isCursorMovingUp;
		GUIManager();
		~GUIManager();

		//Explicit destructor
		void clearManager();

		//Create object and add to collection
//		void createAndAddGuiElement(GuiID key, int x, int y, int fontSize, Color color, string msg);

		void updateText(double timeElapsed, string strUpdate);
		void updateButtons(double timeElapsed);

		bool getButtonEventPressed(EventType type);

		//selectNextButton() //increment currentbutton value

		//draw all ui 
		void draw(GraphicsSystem *graphicsSystem); // , string guiTag);

		//Add buffer to collection
		void addToManager(GuiID key, Gui *objToAdd);

		//Remove from collection by object or key
		void removeFromManager(GuiID key);
		void removeFromManager(Gui *objToRemove);

		Gui* getGuiObject(GuiID key);
		void setNumButtons(int amount);

};

#endif // !GUI_MAN_H
