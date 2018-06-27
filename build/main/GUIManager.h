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

	public:

		bool buttonPressed, left;
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

		//draw all ui elements with the given tag or with the global tag "general"
		void draw(GraphicsSystem *graphicsSystem, string guiTag);

		//Add buffer to collection
		void addGuiElement(GuiID key, Gui *objToAdd);

		//Remove from collection by object or key
		void removeGuiElement(GuiID key);
		void removeGuiElement(Gui *objToRemove);

		Gui* getGuiElement(GuiID key);
		void setNumButtons(int amount);

};

#endif // !GUI_MAN_H
