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

	//TODO: Hunt for memory leaks
	//delete mLocalization;
}


void SceneManager::update(double timeElapsed, int score, int level, int playerLevel, int expToNextLevel, int scoreToReach, int blockSpeedMultiplier, int audioCounter, int highestLevel, int lifetimeScore, int lifetimeClears, double fps)
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
				iter->second->getGuiManager()->getGuiElement("playerlevel")->updateText(timeElapsed, mLocalization->getTranslation("YOUR LEVEL") + ": " + to_string(playerLevel));
				iter->second->getGuiManager()->getGuiElement("tonextlevel")->updateText(timeElapsed, mLocalization->getTranslation("EXP TO NEXT LEVEL") + ": " + to_string(expToNextLevel));

				iter->second->getGuiManager()->getGuiElement("mainmenu2")->updateText(timeElapsed, mLocalization->getTranslation("PRESS ENTER TO SELECT"));

				iter->second->getGuiManager()->getGuiElement("0")->updateText(timeElapsed, mLocalization->getTranslation("NEW GAME"));
				iter->second->getGuiManager()->getGuiElement("1")->updateText(timeElapsed, mLocalization->getTranslation("LOAD GAME"));
				iter->second->getGuiManager()->getGuiElement("2")->updateText(timeElapsed, mLocalization->getTranslation("OPTIONS"));
				iter->second->getGuiManager()->getGuiElement("4")->updateText(timeElapsed, mLocalization->getTranslation("QUIT"));
				iter->second->getGuiManager()->getGuiElement("3")->updateText(timeElapsed, mLocalization->getTranslation("CREDITS"));

				iter->second->getGuiManager()->getGuiElement("fps")->updateText(timeElapsed, to_string(fps));
			}
			else if (iter->second->getSceneType() == SC_GAME)
			{
				iter->second->getGuiManager()->getGuiElement("reach")->updateText(timeElapsed, mLocalization->getTranslation("SCORE TO WIN") + ": " + to_string(scoreToReach));
				iter->second->getGuiManager()->getGuiElement("score")->updateText(timeElapsed, mLocalization->getTranslation("SCORE") + ": " + to_string(score));
				iter->second->getGuiManager()->getGuiElement("level")->updateText(timeElapsed, mLocalization->getTranslation("LEVEL") + ": " + to_string(level));
				iter->second->getGuiManager()->getGuiElement("next")->updateText(timeElapsed, mLocalization->getTranslation("NEXT"));
				iter->second->getGuiManager()->getGuiElement("fps")->updateText(timeElapsed, to_string(fps));
			}
			else if (iter->second->getSceneType() == SC_OPTIONS)
			{
				iter->second->getGuiManager()->getGuiElement("0")->updateText(timeElapsed, mLocalization->getTranslation("OPTIONS"));
				iter->second->getGuiManager()->getGuiElement("1")->updateText(timeElapsed, mLocalization->getTranslation("LANGUAGE"));
				iter->second->getGuiManager()->getGuiElement("2")->updateText(timeElapsed, mLocalization->getTranslation("GAME SPEED") + ": " + to_string(blockSpeedMultiplier));
				iter->second->getGuiManager()->getGuiElement("3")->updateText(timeElapsed, mLocalization->getTranslation("AUDIO") + ": " + to_string(audioCounter));
				iter->second->getGuiManager()->getGuiElement("4")->updateText(timeElapsed, mLocalization->getTranslation("STATS"));
				iter->second->getGuiManager()->getGuiElement("5")->updateText(timeElapsed, mLocalization->getTranslation("RETURN"));

				iter->second->getGuiManager()->getGuiElement("fps")->updateText(timeElapsed, to_string(fps));
			}
			else if (iter->second->getSceneType() == SC_PAUSE)
			{
				iter->second->getGuiManager()->getGuiElement("0")->updateText(timeElapsed, mLocalization->getTranslation("PLAY GAME"));
				iter->second->getGuiManager()->getGuiElement("1")->updateText(timeElapsed, mLocalization->getTranslation("SAVE AND QUIT"));

				iter->second->getGuiManager()->getGuiElement("pause")->updateText(timeElapsed, mLocalization->getTranslation("PAUSED GAME"));
				iter->second->getGuiManager()->getGuiElement("reach")->updateText(timeElapsed, mLocalization->getTranslation("SCORE TO WIN") + ": " + to_string(scoreToReach));
				iter->second->getGuiManager()->getGuiElement("score")->updateText(timeElapsed, mLocalization->getTranslation("SCORE") + ": " + to_string(score));
				iter->second->getGuiManager()->getGuiElement("level")->updateText(timeElapsed, mLocalization->getTranslation("LEVEL") + ": " + to_string(level));
				iter->second->getGuiManager()->getGuiElement("next")->updateText(timeElapsed, mLocalization->getTranslation("NEXT"));

				iter->second->getGuiManager()->getGuiElement("fps")->updateText(timeElapsed, to_string(fps));
			}
			else if (iter->second->getSceneType() == SC_LOSE)
			{
				iter->second->getGuiManager()->getGuiElement("lost")->updateText(timeElapsed, mLocalization->getTranslation("YOU LOST"));
				iter->second->getGuiManager()->getGuiElement("0")->updateText(timeElapsed, mLocalization->getTranslation("NEW GAME"));
				iter->second->getGuiManager()->getGuiElement("1")->updateText(timeElapsed, mLocalization->getTranslation("RETURN"));

				iter->second->getGuiManager()->getGuiElement("fps")->updateText(timeElapsed, to_string(fps));
			}
			else if (iter->second->getSceneType() == SC_CREDITS)
			{
				iter->second->getGuiManager()->getGuiElement("0")->updateText(timeElapsed, mLocalization->getTranslation("RETURN"));

				//TODO: add ui text to this section
				//---------------
				iter->second->getGuiManager()->getGuiElement("devs")->updateText(timeElapsed, mLocalization->getTranslation("DEVELOPERS"));
				iter->second->getGuiManager()->getGuiElement("tetrisArtist")->updateText(timeElapsed, mLocalization->getTranslation("TETRIS SONGWRITER"));
				iter->second->getGuiManager()->getGuiElement("tetrisTrapArtist")->updateText(timeElapsed, mLocalization->getTranslation("TETRIS TRAP WRITER"));
				iter->second->getGuiManager()->getGuiElement("raspArtist")->updateText(timeElapsed, mLocalization->getTranslation("RASPUTIN SONGWRITER"));
				iter->second->getGuiManager()->getGuiElement("tetrisImageArtist")->updateText(timeElapsed, mLocalization->getTranslation("TETRIS IMAGE ARTIST"));
				iter->second->getGuiManager()->getGuiElement("wiiThemeArtist")->updateText(timeElapsed, mLocalization->getTranslation("WII THEME ARTIST"));

				//---------------

				iter->second->getGuiManager()->getGuiElement("fps")->updateText(timeElapsed, to_string(fps));

			}
			else if (iter->second->getSceneType() == SC_STATS)
			{
				iter->second->getGuiManager()->getGuiElement("0")->updateText(timeElapsed, mLocalization->getTranslation("RETURN"));
				iter->second->getGuiManager()->getGuiElement("a")->updateText(timeElapsed, mLocalization->getTranslation("HIGHEST LEVEL REACHED") + ": " + to_string(highestLevel));
				iter->second->getGuiManager()->getGuiElement("b")->updateText(timeElapsed, mLocalization->getTranslation("LIFETIME CLEARS") + ": " + to_string(lifetimeClears));
				iter->second->getGuiManager()->getGuiElement("c")->updateText(timeElapsed, mLocalization->getTranslation("LIFETIME SCORE") + ": " + to_string(lifetimeScore));


				iter->second->getGuiManager()->getGuiElement("fps")->updateText(timeElapsed, to_string(fps));

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