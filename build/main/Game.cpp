#include "Game.h" 

Game* Game::mGameInstance = NULL;

//TODO(high): ui for fragment collection - percetage?

//TODO: add in localization code - eng & fr

//TODO: ememy death audio

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
	mLoadingScreen.initGraphicsBuffer(mSystem.getGraphicsSystem()->getBackbuffer(), mLOCAL_ASSET_PATH + mLoadBgAsset);
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
	
	gMemoryTracker.reportAllocations(cout);

	loadGameData();
	loadLocalization();

	//Initialize graphics, controls, and event system
	if (!mSystem.initSystem(mGAME_TITLE, _DisplayWidth, _DisplayHeight, mGameView.getCamera()))
		return false;

	cout << "*******Initialized system*******" << endl;

	//displayLoadingScreen();
	
	//Initialize game-side input translation from event system
	mInputTranslator.initInputTranslator();
	
	cout << "*******Initialized input translator*******" << endl;
	cout << "~~| Applying duct tape and elbow grease |~~" << endl;

	installListeners();
	
	loadBackgroundsAndBuffers();
	initAnimations();

	initUI();
	loadScenes();
	initAudio();

	initPlayer();

	mEnemyBulletManager.initBulletData(200, 4, mEnemyBulletAnim, _LevelWidth, _LevelHeight, ENEMEY_BULLET_COL_TAG);
	initEnemies();
	initFragments();

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

	const char * iniButtonAsset = ini.GetValue("ASSETS", "button", "default");
	const char * iniMainMenuBG = ini.GetValue("ASSETS", "mainmenubg", "default");
	const char * iniLoseBG = ini.GetValue("ASSETS", "losebg", "default");
	const char * iniWinBG = ini.GetValue("ASSETS", "winbg", "default");
	const char * iniGameBG = ini.GetValue("ASSETS", "gamebg", "default");
	const char * iniFontAsset = ini.GetValue("ASSETS", "fnt", "default");
	const char * iniPlayerAsset = ini.GetValue("ASSETS", "player", "default");
	const char * iniRoninAsset = ini.GetValue("ASSETS", "ronin", "default");
	const char * iniMountainAsset = ini.GetValue("ASSETS", "mtn", "default");
	const char * iniBulletAsset = ini.GetValue("ASSETS", "bullet", "default");
	const char * iniLoadBG = ini.GetValue("ASSETS", "loadbg", "default");
	const char * iniFragmentAsset = ini.GetValue("ASSETS", "frgmnt", "default");
	const char * iniHiveAsset = ini.GetValue("ASSETS", "hive", "default");
	const char * iniEnemyBulletAsset = ini.GetValue("ASSETS", "enemybullet", "default");


	const char * iniWidth = ini.GetValue("VIEW", "windowW", "default");
	const char * iniHeight = ini.GetValue("VIEW", "windowH", "default");
	const char * iniLevelWidth = ini.GetValue("VIEW", "levelW", "default");
	const char * iniLevelHeight = ini.GetValue("VIEW", "levelH", "default");
	const char * iniPlayerSpriteSize = ini.GetValue("VIEW", "pTileSize", "default");
	const char * iniRoninSpriteSize = ini.GetValue("VIEW", "rTileSize", "default");
	const char * iniMountainSpriteSize = ini.GetValue("VIEW", "mTileSize", "default");
	const char * iniBulletSpriteSize = ini.GetValue("VIEW", "bTileSize", "default");
	const char * iniFragmentSpriteSize = ini.GetValue("VIEW", "fTileSize", "default");
	const char * iniHiveSpriteSize = ini.GetValue("VIEW", "hTileSize", "default");
	const char * iniEnemyBulletSpriteSize = ini.GetValue("VIEW", "ebTileSize", "default");
	
	const char * iniRoninScoreValue = ini.GetValue("STATE", "roninScoreVal", "default");
	const char * iniMountainScoreValue = ini.GetValue("STATE", "mtnScoreVal", "default");
	const char * iniHiveScoreValue = ini.GetValue("STATE", "hiveScoreVal", "default");


	const char * iniNumEnemies = ini.GetValue("LEVELDATA", "numEnemies", "defualt");
	const char * iniNumMountain = ini.GetValue("LEVELDATA", "numMt", "defualt");
	const char * iniNumRonin = ini.GetValue("LEVELDATA", "numRonin", "defualt");
	const char * inimNumHive = ini.GetValue("LEVELDATA", "numHive", "defualt");
	const char * iniFragmentsToCollect = ini.GetValue("LEVELDATA", "fragmentsToCollect", "default");

	mButtonAsset = iniButtonAsset; 
	mMainBgAsset = iniMainMenuBG;
	mGameBgAsset = iniGameBG;
	mLoseBgAsset = iniLoseBG;
	mWinBgAsset = iniWinBG;
	mLoadBgAsset = iniLoadBG;
	mFontAsset = iniFontAsset;
	mPlayerAsset = iniPlayerAsset;
	mRoninAsset = iniRoninAsset;
	mBulletAsset = iniBulletAsset;
	mEnemyBulletAsset = iniEnemyBulletAsset;
	mMountainAsset = iniMountainAsset;
	mFragmentAsset = iniFragmentAsset;
	mHiveAsset = iniHiveAsset;

	_DisplayWidth = atoi(iniWidth);
	_DisplayHeight = atoi(iniHeight);

	//using higher value than the bg image causes cool stretching
	_LevelWidth = atoi(iniLevelWidth);//2400; 
	_LevelHeight = atoi(iniLevelHeight);//1800;

	mPlayerSpriteSize = atoi(iniPlayerSpriteSize);
	mRoninSpriteSize = atoi(iniRoninSpriteSize);
	mMountainSpriteSize = atoi(iniMountainSpriteSize);
	mBulletSpriteSize = atoi(iniBulletSpriteSize);
	mEnemyBulletSpriteSize = atoi(iniEnemyBulletSpriteSize);
	mFragmentSpriteSize = atoi(iniFragmentSpriteSize);
	mHiveSpriteSize = atoi(iniHiveSpriteSize);

	mRoninScoreValue = atoi(iniRoninScoreValue);
	mMountainScoreValue = atoi(iniMountainScoreValue);
	mHiveScoreValue = atoi(iniHiveScoreValue);

	mNumEnemies = atoi(iniNumEnemies);
	mNumRonin = atoi(iniNumRonin);
	mNumMountain = atoi(iniNumMountain);
	mNumHive = atoi(inimNumHive);
	_FragmentsToCollect = atoi(iniFragmentsToCollect);

	cout << "*******Loaded game data*******" << endl;
}

void Game::loadLocalization()
{
	//load in translations from file, store in memory, and assign to manager

	mLocalization.loadLanguage(ENGLISH);


	mSceneManager.initLanguages(&mLocalization);

	cout << "*******Loaded languages*******" << endl;
}

void Game::loadBackgroundsAndBuffers()
{
	//Initialize bitmaps using initGraphicsBuffer()
	mPlayerBuffer.initGraphicsBuffer(mSystem.getGraphicsSystem()->getBackbuffer(), mLOCAL_ASSET_PATH + mPlayerAsset);
	mBulletBuffer.initGraphicsBuffer(mSystem.getGraphicsSystem()->getBackbuffer(), mLOCAL_ASSET_PATH + mBulletAsset);
	mEnemyBulletBuffer.initGraphicsBuffer(mSystem.getGraphicsSystem()->getBackbuffer(), mLOCAL_ASSET_PATH + mEnemyBulletAsset);
	mRoninBuffer.initGraphicsBuffer(mSystem.getGraphicsSystem()->getBackbuffer(), mLOCAL_ASSET_PATH + mRoninAsset);
	mMountainBuffer.initGraphicsBuffer(mSystem.getGraphicsSystem()->getBackbuffer(), mLOCAL_ASSET_PATH + mMountainAsset);
	mFragmentBuffer.initGraphicsBuffer(mSystem.getGraphicsSystem()->getBackbuffer(), mLOCAL_ASSET_PATH + mFragmentAsset);
	mHiveBuffer.initGraphicsBuffer(mSystem.getGraphicsSystem()->getBackbuffer(), mLOCAL_ASSET_PATH + mHiveAsset);

	mGameScreenBuffer.initGraphicsBuffer(mSystem.getGraphicsSystem()->getBackbuffer(), mLOCAL_ASSET_PATH + mGameBgAsset);
	mMenuBuffer.initGraphicsBuffer(mSystem.getGraphicsSystem()->getBackbuffer(), mLOCAL_ASSET_PATH + mMainBgAsset);
	mLoseScreenBuffer.initGraphicsBuffer(mSystem.getGraphicsSystem()->getBackbuffer(), mLOCAL_ASSET_PATH + mLoseBgAsset);
	mWinScreenBuffer.initGraphicsBuffer(mSystem.getGraphicsSystem()->getBackbuffer(), mLOCAL_ASSET_PATH + mWinBgAsset);

	//Add buffers to buffer manager
	mBufferManager.addGraphicsBuffer(mPLAYER_ID, &mPlayerBuffer);
	mBufferManager.addGraphicsBuffer(mBULLET_ID, &mBulletBuffer);
	mBufferManager.addGraphicsBuffer(mENEMY_BULLET_ID, &mEnemyBulletBuffer);
	mBufferManager.addGraphicsBuffer(mRONIN_ID, &mRoninBuffer);
	mBufferManager.addGraphicsBuffer(mMOUNTAIN_ID, &mMountainBuffer);
	mBufferManager.addGraphicsBuffer(mFRAGMENT_ID, &mFragmentBuffer);
	mBufferManager.addGraphicsBuffer(mHIVE_ID, &mHiveBuffer);

	mBufferManager.addGraphicsBuffer(mMAINMENU_BUFFER_ID, &mMenuBuffer);
	mBufferManager.addGraphicsBuffer(mLOSE_SCREEN_ID, &mLoseScreenBuffer);
	mBufferManager.addGraphicsBuffer(mWIN_SCREEN_ID, &mWinScreenBuffer);
	mBufferManager.addGraphicsBuffer(mGAME_ID, &mGameScreenBuffer);

	//init Background sprites using initSprite()
	mGameScreenSprite.initSprite(&mGameScreenBuffer, 0, 0, mGameScreenBuffer.getBitmapWidth(), mGameScreenBuffer.getBitmapHeight());
	mMenuSprite.initSprite(&mMenuBuffer, 0, 0, mMenuBuffer.getBitmapWidth(), mMenuBuffer.getBitmapHeight());
	mWinScreenSprite.initSprite(&mWinScreenBuffer, 0, 0, mWinScreenBuffer.getBitmapWidth(), mWinScreenBuffer.getBitmapHeight());
	mLoseScreenSprite.initSprite(&mLoseScreenBuffer, 0, 0, mLoseScreenBuffer.getBitmapWidth(), mLoseScreenBuffer.getBitmapHeight());

	cout << "*******Initialized buffers*******" << endl;
}

void Game::initAnimations()
{
	mPlayerAnim.addSpriteSheet(mBufferManager.getGraphicsBuffer(mPLAYER_ID), 1, 2, mPlayerSpriteSize, mPlayerSpriteSize);
	mRoninAnim.addSpriteSheet(mBufferManager.getGraphicsBuffer(mRONIN_ID), 1, 1, mRoninSpriteSize, mRoninSpriteSize);
	mMountainAnim.addSpriteSheet(mBufferManager.getGraphicsBuffer(mMOUNTAIN_ID), 1, 2, mMountainSpriteSize, mMountainSpriteSize);
	mHiveAnim.addSpriteSheet(mBufferManager.getGraphicsBuffer(mHIVE_ID), 1, 1, mHiveSpriteSize, mHiveSpriteSize);
	
	mFragmentAnim.addSpriteSheet(mBufferManager.getGraphicsBuffer(mFRAGMENT_ID), 1, 4, mFragmentSpriteSize, mFragmentSpriteSize);
	//mFragmentAnim.setLooping(true);

	//set player bullet sprite
	mBulletAnim.addSpriteSheet(mBufferManager.getGraphicsBuffer(mBULLET_ID), 1, 1, mBulletSpriteSize, mBulletSpriteSize);
	mEnemyBulletAnim.addSpriteSheet(mBufferManager.getGraphicsBuffer(mENEMY_BULLET_ID), 1, 1, mEnemyBulletSpriteSize, mEnemyBulletSpriteSize);
}

void Game::initPlayer()
{
	mPlayer.init(_LevelWidth, _LevelHeight);
	mPlayer.setAnimation(mPlayerAnim);
	mPlayer.shouldAnimate(false);
	mPlayer.setCollider(PLAYER_COL_TAG);
	mPlayer.setLoc(450, 200);

	//player bullet init
	mBulletManager.initBulletData(75, 5, mBulletAnim, _LevelWidth, _LevelHeight, BULLET_COL_TAG);

}

void Game::initFragments()
{
	mFragmentList.clearManager();

	random_device rd1, rd2;
	uniform_int_distribution<int> randGenX(1, _LevelWidth - 100);
	uniform_int_distribution<int> randGenY(1, _LevelHeight - 100);

	int randX = 0, randY = 0;

	for (int i = 0; i < _FragmentsToCollect; i++)
	{
		randX = randGenX(rd1);
		randY = randGenY(rd2);

		mFragmentList.createAndAddEntity(mFragmentManTag + to_string(i), randX, randY, mFragmentAnim);

		//	mColliderCollection.push_back(mFragmentList.getColliderList().at(i));
	}

}

void Game::initEnemies()
{
	mRoninManager.clearManager();
	mMountainManager.clearManager();
	mHiveManager.clearManager();

	mColliderCollection.clear();
	//initialize enemy values, locations, and colliders

	random_device rd1, rd2;
	uniform_int_distribution<int> randGenX(1, _LevelWidth - 100);
	uniform_int_distribution<int> randGenY(1, _LevelHeight - 100);
	int randX, randY;

	for (int i = 0; i < mNumRonin; i++)
	{
		randX = randGenX(rd1);
		randY = randGenY(rd2);

		mRoninManager.createAndAddEntity(mRoninManTag + to_string(i), randX, randY, mRoninAnim);
		mRoninManager.getEntity(i)->init(mRoninScoreValue);

		mColliderCollection.push_back(mRoninManager.getColliderList().at(i));
	}

	for (int i = 0; i < mNumMountain; i++)
	{
		randX = randGenX(rd1);
		randY = randGenY(rd2);

		mMountainManager.createAndAddEntity(mMountainManTag + to_string(i), randX, randY, mMountainAnim);
		mMountainManager.getEntity(i)->init(mMountainScoreValue);

		mColliderCollection.push_back(mMountainManager.getColliderList().at(i));
	}

	for (int i = 0; i < mNumHive; i++)
	{
		randX = randGenX(rd1);
		randY = randGenY(rd2);

		mHiveManager.createAndAddEntity(mHiveManTag + to_string(i), randX, randY, mHiveAnim);
		mHiveManager.getEntity(i)->init(&mEnemyBulletManager, mHiveScoreValue);

		mColliderCollection.push_back(mHiveManager.getColliderList().at(i));
	} 

	for (int i = 0; i < mEnemyBulletManager.getPoolSize(); i++)
	{
		mColliderCollection.push_back(mEnemyBulletManager.getBulletCollider(i));
	}

	mColliderCollection.push_back(mPlayer.getCollider());
}

void Game::initUI()
{
	mWhiteText.setColor(255);
	mButtonBuffer.initGraphicsBuffer(mSystem.getGraphicsSystem()->getBackbuffer(), mLOCAL_ASSET_PATH + mButtonAsset);

	//FPS UI
	mFpscounter.initGuiElement(10, 10);
	mFpscounter.addGuiText(12, mWhiteText, to_string(mFPS));

	//MAIN MENU UI
	mMainTitle.initGuiElementWithText(345, 80, mFONT_SIZE, mWhiteText, mGAME_TITLE);

	mMainStart.initGuiElement(270, 115);
	mMainStart.addGuiButton(mButtonBuffer, NEW_GAME, mBUTTON_SPRSHEET_ROWS, mBUTTON_SPRSHEET_COLS, 160, 32, mUI_TXT_SIZE, mWhiteText, "Play");

	mMainQuit.initGuiElement(270, 155);
	mMainQuit.addGuiButton(mButtonBuffer, QUIT, mBUTTON_SPRSHEET_ROWS, mBUTTON_SPRSHEET_COLS, 160, 32, mUI_TXT_SIZE, mWhiteText, "Quit");

	//IN GAME UI
	mGameCombo.initGuiElementWithText(725, 15, mUI_SIZE, mWhiteText, to_string(_ComboCount));
	mGameScore.initGuiElementWithText(740, 30, mUI_SIZE, mWhiteText, to_string(_Score));
	mGameTime.initGuiElementWithText(740, 5, mUI_SIZE, mWhiteText, to_string(_TimeSurvived));
//	mGameFragmentsCollected.initGuiElementWithText(740, 45, mUI_SIZE, mWhiteText, ""to_string(_NumFragments));

	//LOSE SCREEN UI
	mLoseText.initGuiElementWithText(200, 75, mUI_SIZE, mWhiteText, "You've Been Caught");

	mLoseRetry.initGuiElement(270, 115);
	mLoseRetry.addGuiButton(mButtonBuffer, NEW_GAME, mBUTTON_SPRSHEET_ROWS, mBUTTON_SPRSHEET_COLS, 160, 32, mUI_TXT_SIZE, mWhiteText, "Retry");

	mLoseQuit.initGuiElement(270, 155);
	mLoseQuit.addGuiButton(mButtonBuffer, RETURN_MAIN, mBUTTON_SPRSHEET_ROWS, mBUTTON_SPRSHEET_COLS, 160, 32, mUI_TXT_SIZE, mWhiteText, "Return to Main Menu");

	//WIN SCREEN UI
	mWinTitle.initGuiElementWithText(200, 75, mUI_SIZE, mWhiteText, "Success: Data Stolen!");

	mWinPlayAgain.initGuiElement(270, 115);
	mWinPlayAgain.addGuiButton(mButtonBuffer, NEW_GAME, mBUTTON_SPRSHEET_ROWS, mBUTTON_SPRSHEET_COLS, 160, 32, mUI_TXT_SIZE, mWhiteText, "Play Again");

	mWinQuit.initGuiElement(270, 155);
	mWinQuit.addGuiButton(mButtonBuffer, RETURN_MAIN, mBUTTON_SPRSHEET_ROWS, mBUTTON_SPRSHEET_COLS, 160, 32, mUI_TXT_SIZE, mWhiteText, "Return to Main Menu");


	/*** add ui objects to manager ***/
	//NOTE: order matters for buttons in the mangers

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

	//LOSE SCREEN UI MANAGER
	mGuiManagerLose.setNumButtons(2);
	mGuiManagerLose.addToManager("1", &mLoseRetry);
	mGuiManagerLose.addToManager("2", &mLoseQuit);
	mGuiManagerLose.addToManager("text", &mLoseText);
	mGuiManagerLose.addToManager("fps", &mFpscounter);

	//WIN SCREEN UI MANAGER
	mGuiManagerWin.setNumButtons(2);
	mGuiManagerWin.addToManager("1", &mWinPlayAgain);
	mGuiManagerWin.addToManager("2", &mWinQuit);
	mGuiManagerWin.addToManager("title", &mWinTitle);
	mGuiManagerWin.addToManager("fps", &mFpscounter);

	cout << "*******Initialized UI*******" << endl;
}

void Game::loadScenes()
{
	mMainMenuScene.initScene(SC_MAIN, &mGuiManagerMain, &mMenuSprite);
	mGameScene.initScene(SC_GAME, &mGuiManagerGame, &mGameScreenSprite);
	mLoseScene.initScene(SC_LOSE, &mGuiManagerLose, &mLoseScreenSprite);
	mWinScene.initScene(SC_WIN, &mGuiManagerWin, &mWinScreenSprite);

	//add to manager
	mSceneManager.addScene("a", &mMainMenuScene);
	mSceneManager.addScene("b", &mGameScene);
	mSceneManager.addScene("c", &mLoseScene);
	mSceneManager.addScene("d", &mWinScene);

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


	mColliderCollection.clear();
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

		mFragmentList.update(timeElapsed, &mPlayer);
		mRoninManager.update(timeElapsed);
		mMountainManager.update(timeElapsed, &mPlayer);
		mHiveManager.update(timeElapsed, &mPlayer);

		mBulletManager.update(timeElapsed, mColliderCollection);
		mEnemyBulletManager.update(timeElapsed, mColliderCollection);
		
		mPlayer.update(timeElapsed, mColliderCollection, mouseX, mouseY,
					   mGameView.getCamera()->getX(), mGameView.getCamera()->getY());
		
		//check lose condition
		if (!mPlayer.isVisible())
			mSceneManager.setCurrentScene(SC_LOSE);
		
		//check win condition
		if (_NumFragments == _FragmentsToCollect)
			mSceneManager.setCurrentScene(SC_WIN);

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

void Game::resetGameState()
{
	//reset enemies and bullets
	initEnemies();
	initFragments();

	mBulletManager.resetPool();
	mEnemyBulletManager.resetPool();

	//reset player
	mPlayer.getAnimation()->setSpriteIndex(0);
	mPlayer.setVisible(true); 
	mPlayer.shouldAnimate(false);
	mPlayer.setDown(false);
	mPlayer.setUp(false);
	mPlayer.setLeft(false);
	mPlayer.setRight(false);
	mPlayer.setLoc(450, 200);

	//reset gameplay data
	_ComboCount = 0;
	_Score = 0;
	_NumFragments = 0;
	dtTime = 0;

	delete survivalTimer;
	survivalTimer = new Timer;
	survivalTimer->start();
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
		mHiveManager.draw(mSystem.getGraphicsSystem(), camX, camY);
		mEnemyBulletManager.draw(mSystem.getGraphicsSystem(), camX, camY);
		mFragmentList.draw(mSystem.getGraphicsSystem(), camX, camY);
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
			if (mGuiManagerLose.getButtonEventPressed(NEW_GAME))
			{
				resetGameState();
				mSceneManager.setCurrentScene(SC_GAME);
			}
			else if (mGuiManagerLose.getButtonEventPressed(RETURN_MAIN))
			{
				mSceneManager.setCurrentScene(SC_MAIN);
			}
		}
		else if (mSceneManager.getCurrentScene() == SC_WIN)
		{
			if (mGuiManagerWin.getButtonEventPressed(NEW_GAME))
			{
				//restart game & set scene to game
				resetGameState();
				mSceneManager.setCurrentScene(SC_GAME);
			}
			else if (mGuiManagerWin.getButtonEventPressed(RETURN_MAIN))
			{
				mSceneManager.setCurrentScene(SC_MAIN);
			}
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
			mSceneManager.moveCursorDown(SC_LOSE);
		}
		else if (mSceneManager.getCurrentScene() == SC_WIN)
		{
			mSceneManager.moveCursorDown(SC_WIN);
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
			mSceneManager.moveCursorUp(SC_LOSE);
		}
		else if (mSceneManager.getCurrentScene() == SC_WIN)
		{
			mSceneManager.moveCursorUp(SC_WIN);
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
