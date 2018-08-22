#include "SceneManager.h"


SceneManager::SceneManager()
{
	//setprecision(4);
}

SceneManager::~SceneManager()
{
	clearManager();
}

void SceneManager::initLanguages(Language *localization)
{
	mLocalization = localization;
}

void SceneManager::addAudio(SoundID key, Sound *audioToAdd)
{
	map<SoundID, Sound*>::iterator iter = mSoundList.find(key);

	if (iter == mSoundList.end())
		mSoundList[key] = audioToAdd;
}

void SceneManager::playCurrentMusic()
{
	map<SoundID, Sound*>::iterator iter = mSoundList.find(mCurrentSound);

	if (iter != mSoundList.end())
	{
		iter->second->playSound();
	}
}

void SceneManager::playSound(SoundID key)
{
	map<SoundID, Sound*>::iterator iter = mSoundList.find(key);

	if (iter != mSoundList.end())
	{
		mCurrentSound = key;
		iter->second->playSound();
	}
}

void SceneManager::stopCurrentMusic()
{
	map<SoundID, Sound*>::iterator iter = mSoundList.find(mCurrentSound);

	if (iter != mSoundList.end())
	{
		iter->second->stopSound();
	}
}

void SceneManager::stopSound(SoundID key)
{
	map<SoundID, Sound*>::iterator iter = mSoundList.find(key);

	if (iter != mSoundList.end())
	{
		mCurrentSound = "";
		iter->second->stopSound();
	}

}

void SceneManager::playSfx(SoundID id)
{
	map<SoundID, Sound*>::iterator iter = mSoundList.find(id);

	if (iter != mSoundList.end())
	{
		if (iter->second->isSfx())
		{
			iter->second->playSound();
		}
	}
}


void SceneManager::setCurrentMusic(SoundID id)
{
	mCurrentSound = id;
}

void SceneManager::clearManager()
{
	map<SceneID, Scene*>::iterator iter;

	for (iter = mSceneCollection.begin(); iter != mSceneCollection.end(); iter++)
	{
		Scene* tmp = iter->second;

		if (tmp != NULL)
			tmp->~Scene();
	}

	mSceneCollection.clear();
}


void SceneManager::update(double timeElapsed, int &combo, int &score, int &fragmentsCollectedPercentage, int const &minutes, int const &seconds, double &fps)
{
	map<SceneID, Scene*>::iterator iter;

	for (iter = mSceneCollection.begin(); iter != mSceneCollection.end(); iter++)
	{
		//NOTE: Different scenes use different gui managers
		if (iter->second->isSceneActive())
		{
			//Updating UI - allows for language change, scoring, frame data
			if (iter->second->getSceneType() == SC_MAIN)
			{
				iter->second->getGuiManager()->getGuiObject("fps")->updateText(timeElapsed, to_string(fps));
			}
			else if (iter->second->getSceneType() == SC_GAME)
			{
				if (seconds < 10)
					timeStr = ":0";
				else
					timeStr = ":";

				iter->second->getGuiManager()->getGuiObject("fragments")->updateText(timeElapsed, mLocalization->getTranslation("File")+ "%: " + to_string(fragmentsCollectedPercentage));
				iter->second->getGuiManager()->getGuiObject("time")->updateText(timeElapsed, to_string(minutes) + timeStr + to_string(seconds));
				iter->second->getGuiManager()->getGuiObject("combo")->updateText(timeElapsed, mLocalization->getTranslation("Combo") + ": " + to_string(combo));
				iter->second->getGuiManager()->getGuiObject("score")->updateText(timeElapsed, to_string(score));
				iter->second->getGuiManager()->getGuiObject("fps")->updateText(timeElapsed, to_string(fps));
			}
			else if (iter->second->getSceneType() == SC_OPTIONS)
			{
				
				iter->second->getGuiManager()->getGuiObject("fps")->updateText(timeElapsed, to_string(fps));
			}
			else if (iter->second->getSceneType() == SC_PAUSE)
			{
				

				iter->second->getGuiManager()->getGuiObject("fps")->updateText(timeElapsed, to_string(fps));
			}
			else if (iter->second->getSceneType() == SC_LOSE)
			{
				
				iter->second->getGuiManager()->getGuiObject("fps")->updateText(timeElapsed, to_string(fps));
			}
			else if (iter->second->getSceneType() == SC_WIN)
			{

				iter->second->getGuiManager()->getGuiObject("fps")->updateText(timeElapsed, to_string(fps));
			}
			else if (iter->second->getSceneType() == SC_CREDITS)
			{
				

				iter->second->getGuiManager()->getGuiObject("fps")->updateText(timeElapsed, to_string(fps));

			}
			else if (iter->second->getSceneType() == SC_STATS)
			{
			
				iter->second->getGuiManager()->getGuiObject("fps")->updateText(timeElapsed, to_string(fps));

			}
			else if (iter->second->getSceneType() == SC_OPTIONS)
			{


				iter->second->getGuiManager()->getGuiObject("fps")->updateText(timeElapsed, to_string(fps));

			}
			iter->second->update(timeElapsed);
		}
	}

	//music
	if (mCurrentSound != "")
		playCurrentMusic();
	else
		stopCurrentMusic();
}

void SceneManager::update(double timeElapsed, int &combo, int &score, int &fragmentsCollectedPercentage, int const &minutes, int const &seconds)
{
	map<SceneID, Scene*>::iterator iter;

	for (iter = mSceneCollection.begin(); iter != mSceneCollection.end(); iter++)
	{
		//NOTE: Different scenes use different gui managers
		if (iter->second->isSceneActive())
		{
			//Updating UI - allows for language change, scoring, frame data
			if (iter->second->getSceneType() == SC_GAME)
			{
				if (seconds < 10)
					timeStr = ":0";
				else
					timeStr = ":";

				iter->second->getGuiManager()->getGuiObject("fragments")->updateText(timeElapsed, mLocalization->getTranslation("File") + "%: " + to_string(fragmentsCollectedPercentage));
				iter->second->getGuiManager()->getGuiObject("time")->updateText(timeElapsed, to_string(minutes) + timeStr + to_string(seconds));
				iter->second->getGuiManager()->getGuiObject("combo")->updateText(timeElapsed, mLocalization->getTranslation("Combo") + ": " + to_string(combo));
				iter->second->getGuiManager()->getGuiObject("score")->updateText(timeElapsed, to_string(score));
			}
			else if (iter->second->getSceneType() == SC_OPTIONS)
			{

			}
			else if (iter->second->getSceneType() == SC_PAUSE)
			{

			}
			else if (iter->second->getSceneType() == SC_LOSE)
			{

			}
			else if (iter->second->getSceneType() == SC_WIN)
			{

			}
			else if (iter->second->getSceneType() == SC_CREDITS)
			{

			}
			else if (iter->second->getSceneType() == SC_STATS)
			{

			}
			else if (iter->second->getSceneType() == SC_OPTIONS)
			{

			}
			iter->second->update(timeElapsed);
		}
	}

	//music
	if (mCurrentSound != "")
		playCurrentMusic();
	else
		stopCurrentMusic();

}

void SceneManager::draw(GraphicsSystem *graphicsSystem)
{
	map<SceneID, Scene*>::iterator iter;

	for (iter = mSceneCollection.begin(); iter != mSceneCollection.end(); iter++)
	{
		if(iter->second->isSceneActive())
			iter->second->draw(graphicsSystem);
	}
}

void SceneManager::drawGUI(GraphicsSystem *graphicsSystem)
{
	getGuiManager(getCurrentScene())->draw(graphicsSystem);
}


void SceneManager::moveCursorDown(SceneType scene, SoundID buttonMoveSound)
{
	getGuiManager(scene)->buttonPressed = true;
	getGuiManager(scene)->isCursorMovingUp = false;

	playSfx(buttonMoveSound);
}


void SceneManager::moveCursorUp(SceneType scene, SoundID buttonMoveSound)
{
	getGuiManager(scene)->buttonPressed = true;
	getGuiManager(scene)->isCursorMovingUp = true;

	playSfx(buttonMoveSound);
}


void SceneManager::addScene(SceneID key, Scene *objToAdd)
{
	map<SceneID, Scene*>::iterator iter = mSceneCollection.find(key);

	if (iter == mSceneCollection.end())
		mSceneCollection[key] = objToAdd;
}


void SceneManager::removeScene(SceneID key)
{
	map<SceneID, Scene*>::iterator iter = mSceneCollection.find(key);

	if (iter != mSceneCollection.end())
		mSceneCollection.erase(key);
}


void SceneManager::removeScene(Scene *objToRemove)
{
	map<SceneID, Scene*>::iterator iter;

	for (iter = mSceneCollection.begin(); iter != mSceneCollection.end(); iter++)
	{
		if (objToRemove == iter->second)
		{
			mSceneCollection.erase(iter);
		}
	}
}

void SceneManager::setCurrentScene(SceneType type)
{
	map<SceneID, Scene*>::iterator iter;

	for (iter = mSceneCollection.begin(); iter != mSceneCollection.end(); iter++)
	{
		if (iter->second->getSceneType() == mCurrentScene)
			iter->second->setSceneActive(false);
	}

	mCurrentScene = type;

	for (iter = mSceneCollection.begin(); iter != mSceneCollection.end(); iter++)
	{
		if (iter->second->getSceneType() == type)
			iter->second->setSceneActive(true);
	}
}

SceneType SceneManager::getCurrentScene()
{
	return mCurrentScene;
}

Scene* SceneManager::getScene(SceneType type)
{
	map<SceneID, Scene*>::iterator iter;

	for (iter = mSceneCollection.begin(); iter != mSceneCollection.end(); iter++)
	{
		if (iter->second->getSceneType() == type)
			return iter->second;
	}

	return NULL;

}


GUIManager* SceneManager::getGuiManager(SceneType type)
{
	map<SceneID, Scene*>::iterator iter;

	for (iter = mSceneCollection.begin(); iter != mSceneCollection.end(); iter++)
	{
		if (iter->second->getSceneType() == type)
			return iter->second->getGuiManager();
	}

	return NULL;
}