#include "SceneManager.h"


SceneManager::SceneManager()
{
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


void SceneManager::update(double timeElapsed, int &highScore, int &highCombo, string &bestTime, 
						  int &hoursPlayed, int &minutesPlayed, int &lifetimeScore, int &filesCaptured,
						  int &musicValue, bool &controllerInUse, int &combo, int &score, 
						  int &fragmentsCollectedPercentage, int const &minutes, int const &seconds, 
						  double &fps)
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
				iter->second->getGuiManager()->getGuiObject("title")->updateText(timeElapsed, mLocalization->getTranslation("FINAL CRUSADE"));
				iter->second->getGuiManager()->getGuiObject("1")->updateText(timeElapsed, mLocalization->getTranslation("LAUNCH"));
				iter->second->getGuiManager()->getGuiObject("2")->updateText(timeElapsed, mLocalization->getTranslation("RECORD"));
				iter->second->getGuiManager()->getGuiObject("3")->updateText(timeElapsed, mLocalization->getTranslation("OPTIONS"));
				iter->second->getGuiManager()->getGuiObject("4")->updateText(timeElapsed, mLocalization->getTranslation("CREDITS"));
				iter->second->getGuiManager()->getGuiObject("5")->updateText(timeElapsed, mLocalization->getTranslation("QUIT"));
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
				string controllerStr = "null",
					   musicStr = "null";
				
				if (controllerInUse)
					controllerStr = ": On";
				else
					controllerStr = ": Off";
				
				if (musicValue == 1)
					musicStr = ": On";
				else
					musicStr = ": Off";

				iter->second->getGuiManager()->getGuiObject("title")->updateText(timeElapsed, mLocalization->getTranslation("OPTIONS"));
				iter->second->getGuiManager()->getGuiObject("1")->updateText(timeElapsed, mLocalization->getTranslation("SHOW CONTROLS"));
				iter->second->getGuiManager()->getGuiObject("2")->updateText(timeElapsed, mLocalization->getTranslation("GAMEPAD") + controllerStr);
				iter->second->getGuiManager()->getGuiObject("3")->updateText(timeElapsed, mLocalization->getTranslation("LANGUAGE"));
				iter->second->getGuiManager()->getGuiObject("4")->updateText(timeElapsed, mLocalization->getTranslation("MUSIC") + musicStr);
				iter->second->getGuiManager()->getGuiObject("5")->updateText(timeElapsed, mLocalization->getTranslation("RETURN TO MAIN"));

				iter->second->getGuiManager()->getGuiObject("fps")->updateText(timeElapsed, to_string(fps));
			}
			else if (iter->second->getSceneType() == SC_PAUSE)
			{				
				iter->second->getGuiManager()->getGuiObject("text")->updateText(timeElapsed, mLocalization->getTranslation("PAUSED"));
				iter->second->getGuiManager()->getGuiObject("1")->updateText(timeElapsed, mLocalization->getTranslation("RESUME"));
				iter->second->getGuiManager()->getGuiObject("2")->updateText(timeElapsed, mLocalization->getTranslation("RETURN TO MAIN"));
				iter->second->getGuiManager()->getGuiObject("fps")->updateText(timeElapsed, to_string(fps));
			}
			else if (iter->second->getSceneType() == SC_LOSE)
			{
				iter->second->getGuiManager()->getGuiObject("text")->updateText(timeElapsed, mLocalization->getTranslation("You've Been Caught"));
				iter->second->getGuiManager()->getGuiObject("1")->updateText(timeElapsed, mLocalization->getTranslation("REBOOT"));
				iter->second->getGuiManager()->getGuiObject("2")->updateText(timeElapsed, mLocalization->getTranslation("RETURN TO MAIN"));
				iter->second->getGuiManager()->getGuiObject("fps")->updateText(timeElapsed, to_string(fps));
			}
			else if (iter->second->getSceneType() == SC_WIN)
			{
				iter->second->getGuiManager()->getGuiObject("title")->updateText(timeElapsed, mLocalization->getTranslation("Success: Data Stolen!"));
				iter->second->getGuiManager()->getGuiObject("1")->updateText(timeElapsed, mLocalization->getTranslation("RELAUNCH"));
				iter->second->getGuiManager()->getGuiObject("2")->updateText(timeElapsed, mLocalization->getTranslation("RETURN TO MAIN"));
				iter->second->getGuiManager()->getGuiObject("time")->updateText(timeElapsed, "TIME: " + to_string(minutes) + timeStr + to_string(seconds));
				iter->second->getGuiManager()->getGuiObject("score")->updateText(timeElapsed, "SCORE: "+to_string(score));
				iter->second->getGuiManager()->getGuiObject("fps")->updateText(timeElapsed, to_string(fps));
			}
			else if (iter->second->getSceneType() == SC_CREDITS)
			{
				
				iter->second->getGuiManager()->getGuiObject("title")->updateText(timeElapsed, mLocalization->getTranslation("CREDITS"));
				iter->second->getGuiManager()->getGuiObject("1")->updateText(timeElapsed, mLocalization->getTranslation("RETURN TO MAIN"));
				iter->second->getGuiManager()->getGuiObject("walter")->updateText(timeElapsed, mLocalization->getTranslation("Walter Hill -- Programming, Sprites, Design"));
				iter->second->getGuiManager()->getGuiObject("audio")->updateText(timeElapsed, mLocalization->getTranslation("AUDIO"));
				iter->second->getGuiManager()->getGuiObject("m1")->updateText(timeElapsed, mLocalization->getTranslation("Cabled_mess -- Clack_Minimal UI Sounds"));
				iter->second->getGuiManager()->getGuiObject("m2")->updateText(timeElapsed, mLocalization->getTranslation("Cabled_mess -- Boom_C_03"));
				iter->second->getGuiManager()->getGuiObject("m3")->updateText(timeElapsed, mLocalization->getTranslation("Gravity Sound -- Cloudy"));
				iter->second->getGuiManager()->getGuiObject("m4")->updateText(timeElapsed, mLocalization->getTranslation("Josepharaoh99 -- button pressed"));
				iter->second->getGuiManager()->getGuiObject("m5")->updateText(timeElapsed, mLocalization->getTranslation("Xythe -- Fight for your life loop"));
				iter->second->getGuiManager()->getGuiObject("m6")->updateText(timeElapsed, mLocalization->getTranslation("Xtrgamr -- music game, win or high score"));
				iter->second->getGuiManager()->getGuiObject("fps")->updateText(timeElapsed, to_string(fps));
			}
			else if (iter->second->getSceneType() == SC_STATS)
			{
//				string timePlayedTemp, bestTimeTemp;
			//	if (timePlayed == "")
			//		timePlayed = "0 Hours, 0 Minutes";

				if (bestTime == "")
					bestTime = "0:00";

				iter->second->getGuiManager()->getGuiObject("title")->updateText(timeElapsed, mLocalization->getTranslation("SERVICE RECORD"));
				iter->second->getGuiManager()->getGuiObject("1")->updateText(timeElapsed, mLocalization->getTranslation("RETURN TO MAIN"));

				iter->second->getGuiManager()->getGuiObject("besttime")->updateText(timeElapsed, mLocalization->getTranslation("Fastest Time") + " -- " + bestTime);
				iter->second->getGuiManager()->getGuiObject("files")->updateText(timeElapsed, mLocalization->getTranslation("Files Captured") + " -- " + to_string(filesCaptured));
				iter->second->getGuiManager()->getGuiObject("highcombo")->updateText(timeElapsed, mLocalization->getTranslation("Best Combo") + " -- " + to_string(highCombo));
				iter->second->getGuiManager()->getGuiObject("highscore")->updateText(timeElapsed, mLocalization->getTranslation("Best Score") + " -- " + to_string(highScore));
				iter->second->getGuiManager()->getGuiObject("lifescore")->updateText(timeElapsed, mLocalization->getTranslation("Lifetime Score") + " -- " + to_string(lifetimeScore));
				iter->second->getGuiManager()->getGuiObject("lifeplayed")->updateText(timeElapsed, mLocalization->getTranslation("Time Played") + " -- " + mLocalization->getTranslation("Hours") + " " + to_string(hoursPlayed) + ", " + mLocalization->getTranslation("Minutes") + " " + to_string(minutesPlayed));
				iter->second->getGuiManager()->getGuiObject("fps")->updateText(timeElapsed, to_string(fps));
			}
			else if (iter->second->getSceneType() == SC_CONTROLS)
			{
				iter->second->getGuiManager()->getGuiObject("title")->updateText(timeElapsed, mLocalization->getTranslation("CONTROLS"));
				iter->second->getGuiManager()->getGuiObject("1")->updateText(timeElapsed, mLocalization->getTranslation("RETURN TO MAIN"));

				iter->second->getGuiManager()->getGuiObject("gamepad")->updateText(timeElapsed, mLocalization->getTranslation("GAMEPAD"));
				iter->second->getGuiManager()->getGuiObject("start")->updateText(timeElapsed, mLocalization->getTranslation("START -- Pause Game"));
				iter->second->getGuiManager()->getGuiObject("abutton")->updateText(timeElapsed, mLocalization->getTranslation("A -- Select"));
				iter->second->getGuiManager()->getGuiObject("rs")->updateText(timeElapsed, mLocalization->getTranslation("RIGHT STICK -- Rotate Player"));
				iter->second->getGuiManager()->getGuiObject("ls")->updateText(timeElapsed, mLocalization->getTranslation("LEFT STICK -- Up, Left, Right, Down"));
				iter->second->getGuiManager()->getGuiObject("rt")->updateText(timeElapsed, mLocalization->getTranslation("RT -- Shoot"));
				iter->second->getGuiManager()->getGuiObject("keys")->updateText(timeElapsed, mLocalization->getTranslation("KEYBOARD/MOUSE"));
				iter->second->getGuiManager()->getGuiObject("w")->updateText(timeElapsed, mLocalization->getTranslation("W -- Up"));
				iter->second->getGuiManager()->getGuiObject("a")->updateText(timeElapsed, mLocalization->getTranslation("A -- Left"));
				iter->second->getGuiManager()->getGuiObject("s")->updateText(timeElapsed, mLocalization->getTranslation("S -- Down"));
				iter->second->getGuiManager()->getGuiObject("d")->updateText(timeElapsed, mLocalization->getTranslation("D -- Right"));
				iter->second->getGuiManager()->getGuiObject("enter")->updateText(timeElapsed, mLocalization->getTranslation("ENTER -- Select"));
				iter->second->getGuiManager()->getGuiObject("esc")->updateText(timeElapsed, mLocalization->getTranslation("ESC -- Pause Game"));
				iter->second->getGuiManager()->getGuiObject("lmb")->updateText(timeElapsed, mLocalization->getTranslation("LEFT MOUSE-- Shoot"));
				iter->second->getGuiManager()->getGuiObject("move")->updateText(timeElapsed, mLocalization->getTranslation("MOVE MOUSE -- Rotate Player"));
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

void SceneManager::update(double timeElapsed, int &highScore, int &highCombo, string &bestTime, int &hoursPlayed, int &minutesPlayed, int &lifetimeScore, int &filesCaptured, int &musicValue, bool &controllerInUse, int &combo, int &score, int &fragmentsCollectedPercentage, int const &minutes, int const &seconds)
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