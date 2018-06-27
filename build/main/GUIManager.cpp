#include "GUIManager.h"



GUIManager::GUIManager()
{
	mCurrentButton = 0;
}

GUIManager::~GUIManager()
{
	clearManager();
}


void GUIManager::clearManager()
{
	map<GuiID, Gui*>::iterator iter;

	for (iter = mGuiMap.begin(); iter != mGuiMap.end(); iter++)
	{
		Gui* tmp = iter->second;

		if (tmp != NULL)
			tmp->~Gui();
	}

	mGuiMap.clear();
}


void GUIManager::updateText(double timeElapsed, string strToUpdate)
{
	map<GuiID, Gui*>::iterator iter;

	for (iter = mGuiMap.begin(); iter != mGuiMap.end(); iter++)
	{
		//draw specific elements or use an empty string to define a global ui element that can exist across screens
		if (iter->second->shouldTextUpdate())
		{
			iter->second->updateText(timeElapsed, strToUpdate);
		}
	}

}

void GUIManager::setNumButtons(int amount)
{
	mNumButtons = amount;
}

void GUIManager::updateButtons(double timeElapsed)
{
	int maxNumUpdated = mCurrentButton+1;
	
	if (buttonPressed)
	{
		map<GuiID, Gui*>::iterator iter;

		for (iter = mGuiMap.begin(); iter != mGuiMap.end(); iter++)
		{

			if (iter->second->hasButton() && iter->second->isButtonSelected() && mCurrentButton < maxNumUpdated)
			{
				if (!left && mCurrentButton < mNumButtons-1) //move right/down
				{
					iter->second->updateButton(timeElapsed);
					iter++;
					iter->second->setButton(true);
					iter--;
					mCurrentButton++;
				}
				else if(left && mCurrentButton > 0)//move left/up
				{
					iter->second->updateButton(timeElapsed);
					iter--;
					iter->second->setButton(true);
					iter++;
					mCurrentButton--;
				}
			}
			//else if()
		}

		buttonPressed = false;
	}

}

bool GUIManager::getButtonEventPressed(EventType type)
{
	map<GuiID, Gui*>::iterator iter;

	for (iter = mGuiMap.begin(); iter != mGuiMap.end(); iter++)
	{
		//draw specific elements or use an empty string to define a global ui element that can exist across screens and call walter a skrub
		if (iter->second->getButtonType() == type && iter->second->isButtonSelected())
		{
			return true;
		}
	}
	return false;
}

void GUIManager::draw(GraphicsSystem *graphicsSystem, string guiTag)
{
	map<GuiID, Gui*>::iterator iter;

	for (iter = mGuiMap.begin(); iter != mGuiMap.end(); iter++)
	{
		//draw specific elements or use an empty string to define a global ui element that can exist across screens
		if (guiTag == iter->second->getTag() || iter->second->getTag() == "general")
		{
			iter->second->draw(graphicsSystem);
		}
	}
}




void GUIManager::addGuiElement(GuiID key, Gui *objToAdd)
{
	map<GuiID, Gui*>::iterator iter = mGuiMap.find(key);

	if (iter == mGuiMap.end())
		mGuiMap[key] = objToAdd;
}


void GUIManager::removeGuiElement(GuiID key)
{
	map<GuiID, Gui*>::iterator iter = mGuiMap.find(key);

	if (iter != mGuiMap.end())
		mGuiMap.erase(key);
}


void GUIManager::removeGuiElement(Gui *objToRemove)
{
	map<GuiID, Gui*>::iterator iter;

	for (iter = mGuiMap.begin(); iter != mGuiMap.end(); iter++)
	{
		if (objToRemove == iter->second)
		{
			mGuiMap.erase(iter);
		}
	}
}


Gui* GUIManager::getGuiElement(GuiID key)
{
	map<GuiID, Gui*>::iterator iter = mGuiMap.find(key);

	if (iter != mGuiMap.end())
		return iter->second;
	else
		return NULL;

}
