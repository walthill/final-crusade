#include "Game.h"

Game* Game::mGameInstance = NULL;

//TODO: lose screen - work on resetting game

//TODO: add in localization code - eng & fr

//TODO: ememy death audio

//Including in .h vs .cpp https://stackoverflow.com/questions/3002110/include-in-h-or-c-cpp 

//creating exe
//https://discourse.libsdl.org/t/creating-an-easily-distributable-executable-file/24413

Game::Game()
	:EventListener(nullptr) //Null because Event System is static
{
}


Game::~Game()
{
	cleanupGame();
}

void Game::installListeners()
{
	//Initialize event system listeners
	EventSystem::getInstance()->addListener(PAUSE_GAME, this);
	EventSystem::getInstance()->addListener(BUTTON_SELECT, this);
	EventSystem::getInstance()->addListener(MOVE_LEFT, this);
	EventSystem::getInstance()->addListener(MOVE_RIGHT, this);
	EventSystem::getInstance()->addListener(MOVE_DOWN, this);
	EventSystem::getInstance()->addListener(MOVE_UP, this);
	EventSystem::getInstance()->addListener(ROTATION, this);
	EventSystem::getInstance()->addListener(STOP_LEFT, this);
	EventSystem::getInstance()->addListener(STOP_RIGHT, this);
	EventSystem::getInstance()->addListener(STOP_DOWN, this);
	EventSystem::getInstance()->addListener(STOP_UP, this);
	EventSystem::getInstance()->addListener(SHOOT, this);
	EventSystem::getInstance()->addListener(SCREENCAP, this);

	cout << "*******Initialized listeners*******" << endl;
}

void Game::displayLoadingScreen()
{
	//show image while game data & assets load in
	mLoadingScreen.initGraphicsBuffer(mSystem.getGraphicsSystem()->getBackbuffer(), mLOCAL_ASSET_PATH + mLOADING_IMG);
	mBufferManager.addGraphicsBuffer(mLOAD_ID, &mLoadingScreen);

	mLoadingSprite.initSprite(&mLoadingScreen, 0, 0, mLoadingScreen.getBitmapWidth(),
							  mLoadingScreen.getBitmapHeight());

	//mSystem.getGraphicsSystem()->draw(0, 0, mLoadingSprite, 1.0f, 1.0f);
	//mSystem.getGraphicsSystem()->flip();
}

bool Game::initGame()
{
	PerformanceTracker* pPerformanceTracker = new PerformanceTracker;
	pPerformanceTracker->startTracking(mINIT_TRACKER_NAME);

	loadGameData();
	loadLocalization();

	//Initialize graphics, controls, and event system
	if (!mSystem.initSystem(GAME_TITLE, _DisplayWidth, _DisplayHeight, mGameView.getCamera()))
		return false;

	cout << "*******Initialized system*******" << endl;

	displayLoadingScreen();

	//Initialize game-side input translation from event system
	mInputTranslator.initInputTranslator();
	
	cout << "*******Initialized input translator*******" << endl;
	cout << "~~| Applying duct tape and elbow grease |~~" << endl;

	installListeners();
	
	loadBackgrounds();
	initAnimations();
	
	initUI();
	loadScenes();
	initAudio();
	
	initPlayer();
	initEnemies();

	//set player bullet sprite
	mBulletAnim.addSpriteSheet(mBufferManager.getGraphicsBuffer(mBULLET_ID), 1, 1, 15, 15);
	mBulletManager.initBulletData(mBulletAnim, _LevelWidth, _LevelHeight, true, BULLET_COL_TAG);

  	mGameView.initView(&mPlayer, _DisplayWidth, _DisplayHeight, _LevelWidth, _LevelHeight);

 	mIsRunning = true;

	pPerformanceTracker->stopTracking(mINIT_TRACKER_NAME);
	cout << endl << "Time to Init:" << pPerformanceTracker->getElapsedTime(mINIT_TRACKER_NAME) << " ms" << endl;

	delete pPerformanceTracker;

	return true;
}

#pragma region Initilizations

void Game::loadGameData()
{
	//"section", "key",	default, "filename"
	CSimpleIniA ini;
	ini.SetUnicode();
	ini.LoadFile(mINI_FILE.c_str());

	const char * iniWidth = ini.GetValue("VIEW", "windowW", "default");
	const char * iniHeight = ini.GetValue("VIEW", "windowH", "default");
	const char * iniLevelWidth = ini.GetValue("VIEW", "levelW", "default");
	const char * iniLevelHeight = ini.GetValue("VIEW", "levelH", "default");

	const char * iniNumEnemies = ini.GetValue("LEVELDATA", "numEnemies", "defualt");
	const char * iniNumMountain = ini.GetValue("LEVELDATA", "numMt", "defualt");
	const char * iniNumRonin = ini.GetValue("LEVELDATA", "numRonin", "defualt");

	const char * iniPlayerSpriteSize = ini.GetValue("VIEW", "pTileSize", "default");
	const char * iniRoninSpriteSize = ini.GetValue("VIEW", "rTileSize", "default");
	const char * iniMountainSpriteSize = ini.GetValue("VIEW", "mTileSize", "default");
	const char * iniBulletSpriteSize = ini.GetValue("VIEW", "bTileSize", "default");

	_DisplayWidth = atoi(iniWidth);
	_DisplayHeight = atoi(iniHeight);

	//using higher value than the bg image causes cool stretching
	_LevelWidth = atoi(iniLevelWidth);//2400; 
	_LevelHeight = atoi(iniLevelHeight);//1800;

	mPlayerSpriteSize = atoi(iniPlayerSpriteSize);
	mRoninSpriteSize = atoi(iniRoninSpriteSize);
	mMountainSpriteSize = atoi(iniMountainSpriteSize);
	mBulletSpriteSize = atoi(iniBulletSpriteSize);

	mNumEnemies = atoi(iniNumEnemies);
	mNumRonin = atoi(iniNumRonin);
	mNumMountain = atoi(iniNumMountain);

	cout << "*******Loaded game data*******" << endl;
}

void Game::loadLocalization()
{
	//load in translations from file, store in memory, and assign to manager

	mLocalization.loadLanguage(ENGLISH);


	mSceneManager.initLanguages(&mLocalization);

	cout << "*******Loaded languages*******" << endl;
}

void Game::loadBackgrounds()
{
	//Initialize bitmaps using initGraphicsBuffer()
	mPlayerBuffer.initGraphicsBuffer(mSystem.getGraphicsSystem()->getBackbuffer(), mLOCAL_ASSET_PATH + mPLAYER_ASSET);
	mBulletBuffer.initGraphicsBuffer(mSystem.getGraphicsSystem()->getBackbuffer(), mLOCAL_ASSET_PATH + mBULLET_ASSET);
	mRoninBuffer.initGraphicsBuffer(mSystem.getGraphicsSystem()->getBackbuffer(), mLOCAL_ASSET_PATH + mRONIN_ASSET);
	mMountainBuffer.initGraphicsBuffer(mSystem.getGraphicsSystem()->getBackbuffer(), mLOCAL_ASSET_PATH + mMOUNTAIN_ASSET);

	mMenuBuffer.initGraphicsBuffer(mSystem.getGraphicsSystem()->getBackbuffer(), mLOCAL_ASSET_PATH + mMAINMENU_BG_ASSET);

	//Add buffers to buffer manager
	mBufferManager.addGraphicsBuffer(mPLAYER_ID, &mPlayerBuffer);
	mBufferManager.addGraphicsBuffer(mBULLET_ID, &mBulletBuffer);
	mBufferManager.addGraphicsBuffer(mRONIN_ID, &mRoninBuffer);
	mBufferManager.addGraphicsBuffer(mMOUNTAIN_ID, &mMountainBuffer);

	mBufferManager.addGraphicsBuffer(mMAINMENU_BUFFER_ID, &mMenuBuffer);

	//init Background sprites using initSprite()
	mMenuSprite.initSprite(&mMenuBuffer, 0, 0, mMenuBuffer.getBitmapWidth(), mMenuBuffer.getBitmapHeight());

	cout << "*******Initialized buffers*******" << endl;
}

void Game::initAnimations()
{
	mPlayerAnim.addSpriteSheet(mBufferManager.getGraphicsBuffer(mPLAYER_ID), 1, 2, 16, 16);
	mRoninAnim.addSpriteSheet(mBufferManager.getGraphicsBuffer(mRONIN_ID), 1, 1, 32, 32);
	mMountainAnim.addSpriteSheet(mBufferManager.getGraphicsBuffer(mMOUNTAIN_ID), 1, 1, 24, 24);

}

void Game::initPlayer()
{
	mPlayer.init(_LevelWidth, _LevelHeight);
	mPlayer.setAnimation(mPlayerAnim);
	mPlayer.shouldAnimate(false);
	mPlayer.setCollider(PLAYER_COL_TAG);
	mPlayer.setLoc(450, 200);

}

void Game::initEnemies()
{
	random_device rd1, rd2;
	uniform_int_distribution<int> randGenX(1, _LevelWidth - 100);
	uniform_int_distribution<int> randGenY(1, _LevelHeight - 100);
	int randX, randY;

	for (int i = 0; i < 0; i++)
	{
		randX = randGenX(rd1);
		randY = randGenY(rd2);

		mRoninManager.createAndAddEntity(mRoninManTag + to_string(i), randX, randY, mRoninAnim);
		mRoninManager.getEntity(i)->init();

		mColliderCollection.push_back(mRoninManager.getColliderList().at(i));
	}

	for (int i = 0; i < 1; i++)
	{
		randX = randGenX(rd1);
		randY = randGenY(rd2);

		mMountainManager.createAndAddEntity(mMountainManTag + to_string(i), randX, randY, mMountainAnim);
		mMountainManager.getEntity(i)->init();

		mColliderCollection.push_back(mMountainManager.getColliderList().at(i));
	}
}

void Game::initUI()
{
	mWhiteText.setColor(255);
	mButtonBuffer.initGraphicsBuffer(mSystem.getGraphicsSystem()->getBackbuffer(), mLOCAL_ASSET_PATH + mBUTTON_ASSET);

	//FPS UI
	mFpscounter.initGuiElement(10, 10);
	mFpscounter.addGuiText(12, mWhiteText, to_string(mFPS));

	//MAIN MENU UI
	mMainTitle.initGuiElementWithText(345, 80, mFONT_SIZE, mWhiteText, "Final Crusade");

	mMainStart.initGuiElement(270, 115);
	mMainStart.addGuiButton(mButtonBuffer, NEW_GAME, mBUTTON_SPRSHEET_ROWS, mBUTTON_SPRSHEET_COLS, 160, 32, mUI_TXT_SIZE, mWhiteText, "Play");

	mMainQuit.initGuiElement(270, 155);
	mMainQuit.addGuiButton(mButtonBuffer, QUIT, mBUTTON_SPRSHEET_ROWS, mBUTTON_SPRSHEET_COLS, 160, 32, mUI_TXT_SIZE, mWhiteText, "Quit");

	//IN GAME UI
	mGameCombo.initGuiElementWithText(745, 550, mUI_SIZE, mWhiteText, to_string(_ComboCount));
	mGameScore.initGuiElementWithText(745, 30, mUI_SIZE, mWhiteText, to_string(_Score));
	mGameTime.initGuiElementWithText(745, 5, mUI_SIZE, mWhiteText, to_string(_TimeSurvived));


	/*** add ui objects to manager ***/


	//MAIN MENU UI MANAGER
	mGuiManagerMain.setNumButtons(2);
	mGuiManagerMain.addToManager("title", &mMainTitle);
	mGuiManagerMain.addToManager("play", &mMainStart);
	mGuiManagerMain.addToManager("quit", &mMainQuit);
	mGuiManagerMain.addToManager("fps", &mFpscounter);

	//GAME UI MANAGER
	mGuiManagerGame.addToManager("combo", &mGameCombo);
	mGuiManagerGame.addToManager("score", &mGameScore);
	mGuiManagerGame.addToManager("time", &mGameTime);

	mGuiManagerGame.addToManager("fps", &mFpscounter);

	cout << "*******Initialized UI*******" << endl;
}

void Game::loadScenes()
{
	mMainMenuScene.initScene(SC_MAIN, &mGuiManagerMain, &mMenuSprite, true);
	mGameScene.initScene(SC_GAME, &mGuiManagerGame, &mLoadingSprite);

	//add to manager
	mSceneManager.addScene("a", &mMainMenuScene);
	mSceneManager.addScene("b", &mGameScene);

	mSceneManager.setCurrentScene(SC_MAIN);

	cout << "~~| Twisting loose screws |~~" << endl;
	cout << "*******Scenes Loaded*******" << endl;
}

void Game::initAudio()
{

}
#pragma endregion

bool Game::saveGame()
{
	bool result = false;
		   
	CSimpleIniA ini;
	ini.LoadFile(mINI_FILE.c_str());
	
	// overwrite last save - deleting an entire section and all keys in it
	ini.Delete("GAME", NULL);

	ini.SaveFile(mINI_FILE.c_str());

	cout << "saved" << endl;

	return result;
}

void Game::loadLastSave()
{

	CSimpleIniA ini;
	ini.LoadFile(mINI_FILE.c_str());


	mSceneManager.setCurrentScene(SC_GAME);
}

void Game::cleanupGame()
{
	delete survivalTimer;
	survivalTimer = NULL;

	delete mFont;
	mFont = NULL;

	delete mGameUI;
	mGameUI = NULL;

	mBufferManager.clearManager();
	mSystem.cleanupSystem();
	EventSystem::cleanupInstance();
//	CossinTable::cleanupInstance();
}


bool Game::runGameLoop()
{
	PerformanceTracker* pPerformanceTracker = new PerformanceTracker;

	Timer* frameTimer = new Timer;
	frameTimer->start();

	while (mIsRunning)
	{
		pPerformanceTracker->clearTracker(mDRAW_TRACKER_NAME);
		pPerformanceTracker->startTracking(mDRAW_TRACKER_NAME);
		frameTimer->start();

		update(mFRAME_TIME_60FPS);
		render();

		frameTimer->sleepUntilElapsed(mFRAME_TIME_60FPS);
		pPerformanceTracker->stopTracking(mDRAW_TRACKER_NAME);
		
		mFPS = (1000.0 / pPerformanceTracker->getElapsedTime(mDRAW_TRACKER_NAME));
		cout << "Time to Process Frame:" << pPerformanceTracker->getElapsedTime(mDRAW_TRACKER_NAME) << " ms" << endl << endl;
	}

	//saveGame();

	delete pPerformanceTracker;
	delete frameTimer;

	return false;
}


void Game::update(double timeElapsed)
{
	//Input
	mSystem.getInputSystem()->update(timeElapsed);

	//update displays

	mSceneManager.update(timeElapsed, _ComboCount, _Score, min, sec, mFPS);
	
	if (mSceneManager.getCurrentScene() == SC_GAME)
	{
		tickSurvivalTimer();

		mRoninManager.update(timeElapsed);

		mBulletManager.update(timeElapsed, mColliderCollection);
		mPlayer.update(timeElapsed, mColliderCollection, 
					   mouseX, mouseY, mGameView.getCamera()->getX(), mGameView.getCamera()->getY());
		mMountainManager.update(timeElapsed, &mPlayer);

		comboUpdate(timeElapsed);
		
		mGameView.update(timeElapsed);
	}
}

void Game::tickSurvivalTimer()
{
	millisec = survivalTimer->getElapsedTime();

	sec = millisec / 1000;

	if (sec >= 60)
	{
		sec = 0;
		min += 1;
		survivalTimer->start();
	}
}

void Game::comboUpdate(double timeElapsed)
{
	if (_CanCombo)
	{
		Color gr(0, 255, 0); //tmp
		mGuiManagerGame.getGuiObject("combo")->setTextColor(gr);

		dtTime = mCOMBO_WINDOW; //combo window
		_ComboCount++;

		_CanCombo = false;
	}

	//3 is the number needed to regen 
	if (_ComboCount == mCOMBO_NUM_TO_REGEN && mPlayer.isLastLife())
	{
		mPlayer.getAnimation()->setLooping(false);
		mPlayer.shouldAnimate(false);

		mPlayer.getAnimation()->setSpriteIndex(0);
		mPlayer.setLastLife(false);

		mPlayer.setCollisionDetected(false);
	}

	if (dtTime > 0)
	{
		dtTime -= timeElapsed;
		if (dtTime <= 0)
		{
			endCombo();
		}
	}
}

void Game::endCombo()
{
	_ComboCount = 0;
	mGuiManagerGame.getGuiObject("combo")->setTextColor(mWhiteText);
}

void Game::render()
{
	//draw scene background
	mSceneManager.draw(mSystem.getGraphicsSystem());

	if (mSceneManager.getCurrentScene() == SC_GAME)
	{
		camX = mGameView.getCamera()->getX();
		camY = mGameView.getCamera()->getY();

		mPlayer.draw(mSystem.getGraphicsSystem(), camX, camY);
		mBulletManager.draw(mSystem.getGraphicsSystem(), camX, camY);
		mRoninManager.draw(mSystem.getGraphicsSystem(), camX, camY);
		mMountainManager.draw(mSystem.getGraphicsSystem(), camX, camY);
	}

	//draw gui last so that it exists over the rest of the game
	mSceneManager.drawGUI(mSystem.getGraphicsSystem());
	
	if (takeScreenshot)
	{
		mSystem.getGraphicsSystem()->takeScreenshot(mSystem.getGraphicsSystem()->getBackbuffer());
		takeScreenshot = false;
	}

	mSystem.getGraphicsSystem()->flip();
}


void Game::handleEvent(const Event& theEvent)
{
	mEventType = theEvent.getType();

	switch (mEventType)
	{
	case PAUSE_GAME:
		/*if (mSceneManager.getCurrentScene() == SC_GAME)
			mSceneManager.setCurrentScene(SC_PAUSE);
		else if(mSceneManager.getCurrentScene() == SC_PAUSE)
			mSceneManager.setCurrentScene(SC_GAME);*/
		mIsRunning = false; //temp
		break;

	case BUTTON_SELECT:
		//button actions for each scene
		if (mSceneManager.getCurrentScene() == SC_MAIN)
		{
			if (mGuiManagerMain.getButtonEventPressed(NEW_GAME))
			{
				mSceneManager.setCurrentScene(SC_GAME);
				
				survivalTimer = new Timer;
				survivalTimer->start();
			}
			else if (mGuiManagerMain.getButtonEventPressed(QUIT))
			{
				mIsRunning = false;
			}
		}
		else if (mSceneManager.getCurrentScene() == SC_OPTIONS)
		{

		}
		else if (mSceneManager.getCurrentScene() == SC_PAUSE)
		{

		}
		else if (mSceneManager.getCurrentScene() == SC_LOSE)
		{

		}
		else if (mSceneManager.getCurrentScene() == SC_CREDITS)
		{

		}
		else if (mSceneManager.getCurrentScene() == SC_STATS)
		{

		}
		break;
	case SHOOT:
		
		if (mPlayer.isVisible())
		{
			bulletSpawnX = mPlayer.getX();
			bulletSpawnY = mPlayer.getY();

			mBulletManager.fireProjectile(mFRAME_TIME_60FPS, bulletSpawnX, bulletSpawnY, mPlayer.getRotation());

			mGameView.toggleScreenShake(true);
		}

		//TODO: shooting audio
		//cout << "SHOOT SHOOT SHOOP" << endl;
		break;
	case ROTATION:
	{
		const MouseEvent& mouseEvent = static_cast<const MouseEvent&>(theEvent);

		if (mSceneManager.getCurrentScene() == SC_GAME)
		{
			//cout << "Mouse Y location: " + to_string(mouseEvent.getY()) << endl;

			mouseX = mouseEvent.getX();
			mouseY = mouseEvent.getY();
		}
		break;
	}
	
	case MOVE_DOWN:
		if (mSceneManager.getCurrentScene() == SC_GAME)
		{
			mPlayer.setDown(true);
		}
		else if (mSceneManager.getCurrentScene() == SC_MAIN)
		{
			mSceneManager.moveCursorDown(SC_MAIN);
		}
		else if (mSceneManager.getCurrentScene() == SC_OPTIONS)
		{
			

		}
		else if (mSceneManager.getCurrentScene() == SC_PAUSE)
		{
			

		}
		else if (mSceneManager.getCurrentScene() == SC_LOSE)
		{
			

		}

		break;

	case MOVE_UP:
		if (mSceneManager.getCurrentScene() == SC_GAME)
		{
			mPlayer.setUp(true);
		}
		else if (mSceneManager.getCurrentScene() == SC_MAIN)
		{
			mSceneManager.moveCursorUp(SC_MAIN);
		}
		else if (mSceneManager.getCurrentScene() == SC_OPTIONS)
		{
		}
		else if (mSceneManager.getCurrentScene() == SC_PAUSE)
		{
			
		}
		else if (mSceneManager.getCurrentScene() == SC_LOSE)
		{
		
		}
		break;


	case MOVE_LEFT:
		if (mSceneManager.getCurrentScene() == SC_GAME)
		{
			mPlayer.setLeft(true);		
		}
		break;

	case MOVE_RIGHT:
		if (mSceneManager.getCurrentScene() == SC_GAME)
		{
			mPlayer.setRight(true);
		}
		break;



	case STOP_DOWN:
		if (mSceneManager.getCurrentScene() == SC_GAME)
		{
			mPlayer.setDown(false);
		}

		break;

	case STOP_UP:
		if (mSceneManager.getCurrentScene() == SC_GAME)
		{
			mPlayer.setUp(false);
		}
		break;


	case STOP_LEFT: 
		if (mSceneManager.getCurrentScene() == SC_GAME)
		{
			mPlayer.setLeft(false);		
		}
		break;

	case STOP_RIGHT:
		if (mSceneManager.getCurrentScene() == SC_GAME)
		{
			mPlayer.setRight(false);
		}
		break;

	case SCREENCAP:
		takeScreenshot = true;
		cout << "SCREENSHOT CAPTURED!" << endl;
		break;

	}


}
