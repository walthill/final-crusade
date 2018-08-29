/*****************

RELEASE BUILD NOTES - VS2017
	--> To compile Release build, add GraphicsLib.lib to ../Release (relative path)
	--> 
*****************/

#include "Game.h" 

Game* Game::mGameInstance = NULL;

//TODO: tweak player sprite --> bigger and make direction more visible

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
	EventSystem::getInstance()->addListener(MOVE_LEFT_CONTROLLER, this);
	EventSystem::getInstance()->addListener(MOVE_RIGHT_CONTROLLER, this);
	EventSystem::getInstance()->addListener(MOVE_DOWN_CONTROLLER, this);
	EventSystem::getInstance()->addListener(MOVE_UP_CONTROLLER, this);

	EventSystem::getInstance()->addListener(ROTATION, this);
	EventSystem::getInstance()->addListener(STOP_LEFT, this);
	EventSystem::getInstance()->addListener(STOP_RIGHT, this);
	EventSystem::getInstance()->addListener(STOP_DOWN, this);
	EventSystem::getInstance()->addListener(STOP_UP, this);
	EventSystem::getInstance()->addListener(STOP_LEFT_CONTROLLER, this);
	EventSystem::getInstance()->addListener(STOP_RIGHT_CONTROLLER, this);
	EventSystem::getInstance()->addListener(STOP_DOWN_CONTROLLER, this);
	EventSystem::getInstance()->addListener(STOP_UP_CONTROLLER, this);
	EventSystem::getInstance()->addListener(SHOOT, this);
	EventSystem::getInstance()->addListener(SCREENCAP, this);
	EventSystem::getInstance()->addListener(TOGGLE_CONTROLLER, this);

	cout << "*******Initialized listeners*******" << endl;
}

void Game::displayLoadingScreen()
{
	//show image while game data & assets load in
	mLoadingScreen.initGraphicsBuffer(mSystem.getGraphicsSystem()->getBackbuffer(), mLOCAL_ASSET_PATH + mLoadBgAsset);
	mBufferManager.addGraphicsBuffer(mLOAD_ID, &mLoadingScreen);

	mLoadingSprite.initSprite(&mLoadingScreen, 0, 0, mLoadingScreen.getBitmapWidth(),
							  mLoadingScreen.getBitmapHeight());

	mSystem.getGraphicsSystem()->draw(0, 0, mLoadingSprite, 1.0f, 1.0f);
	mSystem.getGraphicsSystem()->flip();

	Sleep(2000);
}

bool Game::initGame()
{
	PerformanceTracker* pPerformanceTracker = new PerformanceTracker;
	pPerformanceTracker->startTracking(mINIT_TRACKER_NAME);

	firstPlay = true;
	controllerInUse = false; 
	shouldPlayMusic = true;
	bestMin = 0;
	bestSec = 0;
	musicValue = 1;
	srand(unsigned(time(NULL)));

	loadGameData();
	
	//set up best time stat
	if (mBestTime != "")
	{
		string secStr = mBestTime.substr(mBestTime.find(":") + 1);
		bestSec = atoi(secStr.c_str());

		string minStr = mBestTime.substr(0, mBestTime.find(":"));
		bestMin = atoi(minStr.c_str());
	}
	
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

	mSceneManager.playSound(MENU_MUSIC);

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
	const char * iniCreditsBG = ini.GetValue("ASSETS", "creditsbg", "default");
	const char * iniStatsBG = ini.GetValue("ASSETS", "statsbg", "default");
	const char * iniOptionsBG = ini.GetValue("ASSETS", "optionsbg", "default");

	const char * iniFontAsset = ini.GetValue("ASSETS", "fnt", "default");
	const char * iniPlayerAsset = ini.GetValue("ASSETS", "player", "default");
	const char * iniRoninAsset = ini.GetValue("ASSETS", "ronin", "default");
	const char * iniMountainAsset = ini.GetValue("ASSETS", "mtn", "default");
	const char * iniBulletAsset = ini.GetValue("ASSETS", "bullet", "default");
	const char * iniLoadBG = ini.GetValue("ASSETS", "loadbg", "default");
	const char * iniFragmentAsset = ini.GetValue("ASSETS", "frgmnt", "default");
	const char * iniHiveAsset = ini.GetValue("ASSETS", "hive", "default");
	const char * iniEnemyBulletAsset = ini.GetValue("ASSETS", "enemybullet", "default");

	const char * iniPlayerShootSound = ini.GetValue("ASSETS", "playershoot", "default");
	const char * iniEnemyShootSound = ini.GetValue("ASSETS", "enemyshoot", "default");
	const char * iniPlayerHitSound = ini.GetValue("ASSETS", "playerhit", "default");
	const char * iniEnemyHitSound = ini.GetValue("ASSETS", "enemyhit", "default");
	const char * iniPlayerDiedSound = ini.GetValue("ASSETS", "playerdied", "default");
	const char * iniFragmentPickupSound = ini.GetValue("ASSETS", "fragmentpickup", "default");
	const char * iniButtonMoveSound = ini.GetValue("ASSETS", "buttonmove", "default");
	const char * iniButtonSelectSound = ini.GetValue("ASSETS", "buttonselect", "default");
	const char * iniMenuMusic= ini.GetValue("ASSETS", "menumusic", "default");
	const char * iniGameMusic = ini.GetValue("ASSETS", "gamemusic", "default");
	const char * iniWinMusic = ini.GetValue("ASSETS", "winmusic", "default");


	const char * iniLifetimeScore = ini.GetValue("STATS", "lifetimescore", "default");
	const char * iniHoursPlayed = ini.GetValue("STATS", "hoursplayed", "default");
	const char * iniMinutesPlayed = ini.GetValue("STATS", "minutesplayed", "default");
	const char * iniSecondsPlayed = ini.GetValue("STATS", "secondsplayed", "default");
	const char * iniBestTime = ini.GetValue("STATS", "besttime", "default");
	const char * iniHighScore = ini.GetValue("STATS", "highscore", "default");
	const char * iniHighCombo = ini.GetValue("STATS", "highcombo", "default");
	const char * iniFileCaptured= ini.GetValue("STATS", "filescaptured", "default");


	const char * iniWidth = ini.GetValue("VIEW", "windowW", "default");
	const char * iniHeight = ini.GetValue("VIEW", "windowH", "default");
	const char * iniLevelWidth = ini.GetValue("VIEW", "levelW", "default");
	const char * iniLevelHeight = ini.GetValue("VIEW", "levelH", "default");
	const char * iniPlayerSpriteSize = ini.GetValue("VIEW", "playerTileSize", "default");
	const char * iniRoninSpriteSize = ini.GetValue("VIEW", "roninTileSize", "default");
	const char * iniMountainSpriteSize = ini.GetValue("VIEW", "mtnTileSize", "default");
	const char * iniBulletSpriteSize = ini.GetValue("VIEW", "bulletTileSize", "default");
	const char * iniFragmentSpriteSize = ini.GetValue("VIEW", "fragmentTileSize", "default");
	const char * iniHiveSpriteSize = ini.GetValue("VIEW", "hiveTileSize", "default");
	const char * iniEnemyBulletSpriteSize = ini.GetValue("VIEW", "enemybulletTileSize", "default");
	
	const char * iniRoninScoreValue = ini.GetValue("STATE", "roninScoreVal", "default");
	const char * iniMountainScoreValue = ini.GetValue("STATE", "mtnScoreVal", "default");
	const char * iniHiveScoreValue = ini.GetValue("STATE", "hiveScoreVal", "default");

//	const char * iniNumEnemies = ini.GetValue("LEVELDATA", "numEnemies", "defualt");
	const char * iniNumMountainMin = ini.GetValue("LEVELDATA", "numMtMin", "defualt");
	const char * iniNumRoninMin = ini.GetValue("LEVELDATA", "numRoninMin", "defualt");
	const char * inimNumHiveMin = ini.GetValue("LEVELDATA", "numHiveMin", "defualt");
	const char * iniNumMountainMax = ini.GetValue("LEVELDATA", "numMtMax", "defualt");
	const char * iniNumRoninMax = ini.GetValue("LEVELDATA", "numRoninMax", "defualt");
	const char * inimNumHiveMax = ini.GetValue("LEVELDATA", "numHiveMax", "defualt");
	const char * iniFragmentsToCollect = ini.GetValue("LEVELDATA", "fragmentsToCollect", "default");

	mButtonAsset = iniButtonAsset; 
	mMainBgAsset = iniMainMenuBG;
	mGameBgAsset = iniGameBG;
	mLoseBgAsset = iniLoseBG;
	mStatsBgAsset = iniStatsBG;
	mOptionsBgAsset = iniOptionsBG;
	mCreditsBgAsset = iniCreditsBG;
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

	mPlayerHitSound = iniPlayerHitSound;
	mPlayerShootSound = iniPlayerShootSound;
	mPlayerLoseSound = iniPlayerDiedSound;
	mEnemyHitSound = iniEnemyHitSound;
	mEnemyShootSound = iniEnemyShootSound;
	mButtonSelectSound = iniButtonSelectSound;
	mButtonMoveSound = iniButtonMoveSound;
	mFragmentPickupSound = iniFragmentPickupSound;
	mMenuSound = iniMenuMusic;
	mGameSound = iniGameMusic;
	mWinSound = iniWinMusic;

	_DisplayWidth = atoi(iniWidth);
	_DisplayHeight = atoi(iniHeight);

	//cool note - using higher value than the bg image causes cool stretching
	
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

//	mNumEnemies = atoi(iniNumEnemies);
	
	mNumRoninMin = atoi(iniNumRoninMin);
	mNumMountainMin = atoi(iniNumMountainMin);
	mNumHiveMin = atoi(inimNumHiveMin);
	mNumRoninMax = atoi(iniNumRoninMax);
	mNumMountainMax = atoi(iniNumMountainMax);
	mNumHiveMax = atoi(inimNumHiveMax);

	mHighCombo = atoi(iniHighCombo);
	mHighScore = atoi(iniHighScore);
	mFilesCaptured = atoi(iniFileCaptured);
	mLifetimeScore = atoi(iniLifetimeScore);
	mBestTime = iniBestTime;
	hoursPlayed = atoi(iniHoursPlayed);
	minutesPlayed = atoi(iniMinutesPlayed);
	secondsPlayed = atoi(iniSecondsPlayed);

//	bestSec

	_FragmentsToCollect = atoi(iniFragmentsToCollect);

	cout << "*******Loaded game data*******" << endl;
}

void Game::loadLocalization()
{
	//load in translations from file, store in memory, and assign to manager

	mLocalization.loadLanguage(ENGLISH);
	mLocalization.loadLanguage(FRENCH);

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
	mStatsScreenBuffer.initGraphicsBuffer(mSystem.getGraphicsSystem()->getBackbuffer(), mLOCAL_ASSET_PATH + mStatsBgAsset);
	mCreditsScreenBuffer.initGraphicsBuffer(mSystem.getGraphicsSystem()->getBackbuffer(), mLOCAL_ASSET_PATH + mCreditsBgAsset);
	mOptionsScreenBuffer.initGraphicsBuffer(mSystem.getGraphicsSystem()->getBackbuffer(), mLOCAL_ASSET_PATH + mOptionsBgAsset);
	
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
	mBufferManager.addGraphicsBuffer(mOPTION_SCREEN_ID, &mOptionsScreenBuffer);
	mBufferManager.addGraphicsBuffer(mCREDITS_SCREEN_ID, &mCreditsScreenBuffer);
	mBufferManager.addGraphicsBuffer(mSTATS_SCREEN_ID, &mStatsScreenBuffer);

	//init Background sprites using initSprite()
	mGameScreenSprite.initSprite(&mGameScreenBuffer, 0, 0, mGameScreenBuffer.getBitmapWidth(), mGameScreenBuffer.getBitmapHeight());
	mMenuSprite.initSprite(&mMenuBuffer, 0, 0, mMenuBuffer.getBitmapWidth(), mMenuBuffer.getBitmapHeight());
	mWinScreenSprite.initSprite(&mWinScreenBuffer, 0, 0, mWinScreenBuffer.getBitmapWidth(), mWinScreenBuffer.getBitmapHeight());
	mLoseScreenSprite.initSprite(&mLoseScreenBuffer, 0, 0, mLoseScreenBuffer.getBitmapWidth(), mLoseScreenBuffer.getBitmapHeight());
	mStatsScreenSprite.initSprite(&mStatsScreenBuffer, 0, 0, mStatsScreenBuffer.getBitmapWidth(), mStatsScreenBuffer.getBitmapHeight());
	mCreditsScreenSprite.initSprite(&mCreditsScreenBuffer, 0, 0, mCreditsScreenBuffer.getBitmapWidth(), mCreditsScreenBuffer.getBitmapHeight());
	mOptionsScreenSprite.initSprite(&mOptionsScreenBuffer, 0, 0, mOptionsScreenBuffer.getBitmapWidth(), mOptionsScreenBuffer.getBitmapHeight());

	cout << "*******Initialized buffers*******" << endl;
}

void Game::initAnimations()
{
	mPlayerAnim.addSpriteSheet(mBufferManager.getGraphicsBuffer(mPLAYER_ID), 1, 2, mPlayerSpriteSize, mPlayerSpriteSize);
	mRoninAnim.addSpriteSheet(mBufferManager.getGraphicsBuffer(mRONIN_ID), 1, 1, mRoninSpriteSize, mRoninSpriteSize);
	mMountainAnim.addSpriteSheet(mBufferManager.getGraphicsBuffer(mMOUNTAIN_ID), 1, 2, mMountainSpriteSize, mMountainSpriteSize);
	mHiveAnim.addSpriteSheet(mBufferManager.getGraphicsBuffer(mHIVE_ID), 1, 1, mHiveSpriteSize, mHiveSpriteSize);
	
	mFragmentAnim.addSpriteSheet(mBufferManager.getGraphicsBuffer(mFRAGMENT_ID), 1, 4, mFragmentSpriteSize, mFragmentSpriteSize);
	
	//set player bullet sprite
	mBulletAnim.addSpriteSheet(mBufferManager.getGraphicsBuffer(mBULLET_ID), 1, 1, mBulletSpriteSize, mBulletSpriteSize);
	mEnemyBulletAnim.addSpriteSheet(mBufferManager.getGraphicsBuffer(mENEMY_BULLET_ID), 1, 1, mEnemyBulletSpriteSize, mEnemyBulletSpriteSize);
}

void Game::initPlayer()
{
	mPlayer.init(_LevelWidth, _LevelHeight, PLAYER_HIT);
	mPlayer.setAnimation(mPlayerAnim);
	mPlayer.shouldAnimate(false);
	mPlayer.setCollider(PLAYER_COL_TAG);

	mPlayer.setLoc(_LevelWidth/2, _LevelHeight/2);

	//player bullet init
	mBulletManager.initBulletData(75, 10, mBulletAnim, _LevelWidth, _LevelHeight, BULLET_COL_TAG);
}

void Game::initFragments()
{
	mFragmentList.clearManager();

	random_device rd1, rd2;
	uniform_int_distribution<int> randGenX(1, _LevelWidth - 100);
	uniform_int_distribution<int> randGenY(1, _LevelHeight - 100);

	int randX = 0, randY = 0;
	int spawnRegion = 300;

	/***********************
		Fragments region spawn. 3 in upper half of Y. Each w/in a 300 pix region

	|  i = 1	i = 2	i = 3 
	|    
	|
	|  i = 4	i = 5	i = 6
	
	
	************************/

	for (int i = 0; i < _FragmentsToCollect; i++)
	{
		randX = randGenX(rd1);
		randY = randGenY(rd2);

		if (i == 0)
		{
			while (randX < spawnRegion && randY < _LevelHeight / 2)
			{
				randX = randGenX(rd1);
				randY = randGenY(rd2);
			}
		}
		else if (i == 1)
		{
			while (randX > spawnRegion && randX < spawnRegion *2 && randY < _LevelHeight / 2)
			{
				randX = randGenX(rd1);
				randY = randGenY(rd2);
			}
		}
		else if (i == 2)
		{
			while (randX > spawnRegion*2 && randX < spawnRegion * 3 && randY < _LevelHeight / 2)
			{
				randX = randGenX(rd1);
				randY = randGenY(rd2);
			}
		}
		else if (i == 3)
		{
			while (randX < spawnRegion && randY > _LevelHeight / 2)
			{
				randX = randGenX(rd1);
				randY = randGenY(rd2);
			}
		}
		else if (i == 4)
		{
			while (randX > spawnRegion && randY > _LevelHeight / 2)
			{
				randX = randGenX(rd1);
				randY = randGenY(rd2);
			}
		}
		else if (i == 5)
		{
			while (randX > spawnRegion * 2 && randX < spawnRegion * 3 && randY > _LevelHeight / 2)
			{
				randX = randGenX(rd1);
				randY = randGenY(rd2);
			}
		}

		mFragmentList.createAndAddEntity(mFragmentManTag + to_string(i), randX, randY, mFragmentAnim);
		mFragmentList.getEntity(i)->init(FRAG_PICKUP);
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
	random_device rd1, rd2, rd3, rd4, rd5;
	uniform_int_distribution<int> randGenX(32, _LevelWidth - 32); //32 - largest enemy sprite size
	uniform_int_distribution<int> randGenY(32, _LevelHeight - 32);
	uniform_int_distribution<int> randGenRonin(mNumRoninMin, mNumRoninMax);
	uniform_int_distribution<int> randGenMountain(mNumMountainMin, mNumMountainMax);
	uniform_int_distribution<int> randGenHive(mNumHiveMin, mNumHiveMax);

	int randX, randY;
	int numRonin, numMountain, numHive;
	
	numRonin = randGenRonin(rd3);
	numMountain = randGenMountain(rd4);
	numHive = randGenHive(rd5);

	for (int i = 0; i < numRonin; i++)
	{
		randX = randGenX(rd1);
		randY = randGenY(rd2);

		while (randX < mPlayer.getX() + mPlayerSpriteSize + 300 && randX > mPlayer.getX() - mPlayerSpriteSize + 300)
		{
			randX = randGenX(rd1);
		}

		while (randY < mPlayer.getY() + mPlayerSpriteSize + 300 && randY > mPlayer.getY() - mPlayerSpriteSize + 300)
		{
			randY = randGenY(rd2);
		}
		
		mRoninManager.createAndAddEntity(mRoninManTag + to_string(i), randX, randY, mRoninAnim);
		mRoninManager.getEntity(i)->init(mRoninScoreValue, ENEMY_HIT);

		mColliderCollection.push_back(mRoninManager.getColliderList().at(i));
	}

	for (int i = 0; i < numMountain; i++)
	{
		randX = randGenX(rd1);
		randY = randGenY(rd2);
		
		while (randX < mPlayer.getX() + mPlayerSpriteSize + 300 && randX > mPlayer.getX() - mPlayerSpriteSize + 300)
		{
			randX = randGenX(rd1);
		}

		while (randY < mPlayer.getY() + mPlayerSpriteSize + 300 && randY > mPlayer.getY() - mPlayerSpriteSize + 300)
		{
			randY = randGenY(rd2);
		}

		mMountainManager.createAndAddEntity(mMountainManTag + to_string(i), randX, randY, mMountainAnim);
		mMountainManager.getEntity(i)->init(mMountainScoreValue, ENEMY_HIT);

		mColliderCollection.push_back(mMountainManager.getColliderList().at(i));
	}

	for (int i = 0; i < numHive; i++)
	{
		randX = randGenX(rd1);
		randY = randGenY(rd2);

		while (randX < mPlayer.getX() + mPlayerSpriteSize + 300 && randX > mPlayer.getX() - mPlayerSpriteSize + 300)
		{
			randX = randGenX(rd1);
		}

		while (randY < mPlayer.getY() + mPlayerSpriteSize + 300 && randY > mPlayer.getY() - mPlayerSpriteSize + 300)
		{
			randY = randGenY(rd2);
		}

		mHiveManager.createAndAddEntity(mHiveManTag + to_string(i), randX, randY, mHiveAnim);
		mHiveManager.getEntity(i)->init(&mEnemyBulletManager, mHiveScoreValue, ENEMY_HIT, ENEMY_SHOOT);

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
	mFpscounter.addGuiText(12, mFontAsset, mWhiteText, to_string(mFPS));

	//MAIN MENU UI
	mMainTitle.initGuiElementWithText(_DisplayWidth/2 - 120, _DisplayHeight/8, mTITLE_SIZE, mFontAsset, mWhiteText, mLocalization.getTranslation("FINAL CRUSADE"));

	mMainStart.initGuiElement(_DisplayWidth / 2 - 75, 155);
	mMainStart.addGuiButton(mButtonBuffer, NEW_GAME, mBUTTON_SPRSHEET_ROWS, mBUTTON_SPRSHEET_COLS, 160, 32, mUI_TXT_SIZE, mFontAsset, mWhiteText, mLocalization.getTranslation("LAUNCH"));

	//stats
	mMainStats.initGuiElement(_DisplayWidth / 2 - 75, 195);
	mMainStats.addGuiButton(mButtonBuffer, STATS, mBUTTON_SPRSHEET_ROWS, mBUTTON_SPRSHEET_COLS, 160, 32, mUI_TXT_SIZE, mFontAsset, mWhiteText, mLocalization.getTranslation("RECORD"));

	//options
	mMainOptions.initGuiElement(_DisplayWidth / 2 - 75, 235);
	mMainOptions.addGuiButton(mButtonBuffer, OPTIONS, mBUTTON_SPRSHEET_ROWS, mBUTTON_SPRSHEET_COLS, 160, 32, mUI_TXT_SIZE, mFontAsset, mWhiteText, mLocalization.getTranslation("OPTIONS"));

	//credits
	mMainCredits.initGuiElement(_DisplayWidth / 2 - 75, 280);
	mMainCredits.addGuiButton(mButtonBuffer, CREDITS, mBUTTON_SPRSHEET_ROWS, mBUTTON_SPRSHEET_COLS, 160, 32, mUI_TXT_SIZE, mFontAsset, mWhiteText, mLocalization.getTranslation("CREDITS"));

	//quit
	mMainQuit.initGuiElement(_DisplayWidth / 2 - 75, 325);
	mMainQuit.addGuiButton(mButtonBuffer, QUIT, mBUTTON_SPRSHEET_ROWS, mBUTTON_SPRSHEET_COLS, 160, 32, mUI_TXT_SIZE, mFontAsset, mWhiteText, mLocalization.getTranslation("QUIT"));

	//IN GAME UI
	mGameCombo.initGuiElementWithText(_DisplayWidth - 250, 5, mUI_SIZE, mFontAsset, mWhiteText, to_string(_ComboCount));
	mGameScore.initGuiElementWithText(_DisplayWidth - 62, 30, mUI_SIZE, mFontAsset, mWhiteText, to_string(_Score));
	mGameTime.initGuiElementWithText(_DisplayWidth - 62, 5, mUI_SIZE, mFontAsset, mWhiteText, to_string(_TimeSurvived));
	mGameFragmentsCollected.initGuiElementWithText(_DisplayWidth - 250, 30, mUI_SIZE, mFontAsset, mWhiteText, mLocalization.getTranslation("File")+ "%: " + to_string(_NumFragments/_FragmentsToCollect));

	//PAUSE SCREEN UI
	mPauseText.initGuiElementWithText(_DisplayWidth / 2 - 35, _DisplayHeight / 8, mUI_SIZE, mFontAsset, mWhiteText, mLocalization.getTranslation("PAUSED"));

	mPausePlay.initGuiElement(_DisplayWidth / 2 - 65, 155);
	mPausePlay.addGuiButton(mButtonBuffer, RESUME, mBUTTON_SPRSHEET_ROWS, mBUTTON_SPRSHEET_COLS, 160, 32, mUI_TXT_SIZE, mFontAsset, mWhiteText, mLocalization.getTranslation("RESUME"));
	
	mPauseQuit.initGuiElement(_DisplayWidth / 2 - 65, 195);
	mPauseQuit.addGuiButton(mButtonBuffer, RETURN_MAIN, mBUTTON_SPRSHEET_ROWS, mBUTTON_SPRSHEET_COLS, 160, 32, mUI_TXT_SIZE, mFontAsset, mWhiteText, mLocalization.getTranslation("RETURN TO MAIN"));

	//LOSE SCREEN UI
	mLoseText.initGuiElementWithText(_DisplayWidth / 2 - 75, _DisplayHeight / 8, mUI_SIZE, mFontAsset, mWhiteText, mLocalization.getTranslation("You've Been Caught"));

	mLoseRetry.initGuiElement(_DisplayWidth / 2 - 75, 155);
	mLoseRetry.addGuiButton(mButtonBuffer, NEW_GAME, mBUTTON_SPRSHEET_ROWS, mBUTTON_SPRSHEET_COLS, 160, 32, mUI_TXT_SIZE, mFontAsset, mWhiteText, mLocalization.getTranslation("REBOOT"));

	mLoseQuit.initGuiElement(_DisplayWidth / 2 - 75, 195);
	mLoseQuit.addGuiButton(mButtonBuffer, RETURN_MAIN, mBUTTON_SPRSHEET_ROWS, mBUTTON_SPRSHEET_COLS, 160, 32, mUI_TXT_SIZE, mFontAsset, mWhiteText, mLocalization.getTranslation("RETURN TO MAIN"));

	//WIN SCREEN UI
	mWinTitle.initGuiElementWithText(_DisplayWidth / 2 - 90, _DisplayHeight / 8, mUI_SIZE, mFontAsset, mWhiteText, mLocalization.getTranslation("Success: Data Stolen!"));
	mWinScore.initGuiElementWithText(_DisplayWidth / 2 - 150, _DisplayHeight / 4 + 110, mUI_SIZE, mFontAsset, mWhiteText, to_string(_Score));
	mWinTime.initGuiElementWithText(_DisplayWidth / 2 + 85, _DisplayHeight / 4 + 110, mUI_SIZE, mFontAsset, mWhiteText, to_string(_TimeSurvived));

	mWinPlayAgain.initGuiElement(_DisplayWidth / 2 - 75, 155);
	mWinPlayAgain.addGuiButton(mButtonBuffer, NEW_GAME, mBUTTON_SPRSHEET_ROWS, mBUTTON_SPRSHEET_COLS, 160, 32, mUI_TXT_SIZE, mFontAsset,   mWhiteText, mLocalization.getTranslation("RELAUNCH"));

	mWinQuit.initGuiElement(_DisplayWidth / 2 - 75, 195);
	mWinQuit.addGuiButton(mButtonBuffer, RETURN_MAIN, mBUTTON_SPRSHEET_ROWS, mBUTTON_SPRSHEET_COLS, 160, 32, mUI_TXT_SIZE, mFontAsset, mWhiteText, mLocalization.getTranslation("RETURN TO MAIN"));

	//STATS
	mStatsText.initGuiElementWithText(_DisplayWidth / 2 - 90, _DisplayHeight / 8, mUI_SIZE, mFontAsset, mWhiteText, mLocalization.getTranslation("SERVICE RECORD"));

	mStatsTimePlayed.initGuiElementWithText(_DisplayWidth / 2 - 180, 150, mUI_SIZE, mFontAsset, mWhiteText, mLocalization.getTranslation("Time Played"));
	mStatsLifetimeScore.initGuiElementWithText(_DisplayWidth / 2 - 180, 200, mUI_SIZE, mFontAsset, mWhiteText, mLocalization.getTranslation("Lifetime Score"));
	mStatsFilesCaptured.initGuiElementWithText(_DisplayWidth / 2 - 180, 250, mUI_SIZE, mFontAsset, mWhiteText, mLocalization.getTranslation("Files Captured"));
	mStatsFastestTime.initGuiElementWithText(_DisplayWidth / 2 - 180, 300, mUI_SIZE, mFontAsset, mWhiteText, mLocalization.getTranslation("Fastest Time"));
	mStatsHighScore.initGuiElementWithText(_DisplayWidth / 2 - 180, 350, mUI_SIZE, mFontAsset, mWhiteText, mLocalization.getTranslation("Best Score"));
	mStatsHighCombo.initGuiElementWithText(_DisplayWidth / 2 - 180, 400, mUI_SIZE, mFontAsset, mWhiteText, mLocalization.getTranslation("Best Combo"));
	
	mStatsReturn.initGuiElement(_DisplayWidth / 2 - 90, 495);
	mStatsReturn.addGuiButton(mButtonBuffer, RETURN_STATS, mBUTTON_SPRSHEET_ROWS, mBUTTON_SPRSHEET_COLS, 160, 32, mUI_TXT_SIZE, mFontAsset, mWhiteText, mLocalization.getTranslation("RETURN TO MAIN"));

	//OPTIONS
	mOptionsText.initGuiElementWithText(_DisplayWidth / 2 - 50, _DisplayHeight / 8, mUI_SIZE, mFontAsset, mWhiteText, mLocalization.getTranslation("OPTIONS"));

	mOptionsControls.initGuiElement(_DisplayWidth / 2 - 75, 155);
	mOptionsControls.addGuiButton(mButtonBuffer, SHOW_CONTROLS, mBUTTON_SPRSHEET_ROWS, mBUTTON_SPRSHEET_COLS, 160, 32, mUI_TXT_SIZE, mFontAsset, mWhiteText, mLocalization.getTranslation("Show Controls"));

	mOptionsToggleController.initGuiElement(_DisplayWidth / 2 - 75, 200);
	mOptionsToggleController.addGuiButton(mButtonBuffer, CONTROLLER, mBUTTON_SPRSHEET_ROWS, mBUTTON_SPRSHEET_COLS, 160, 32, mUI_TXT_SIZE, mFontAsset, mWhiteText, mLocalization.getTranslation("Gamepad"));

	mOptionsLang.initGuiElement(_DisplayWidth / 2 - 75, 270);
	mOptionsLang.addGuiButton(mButtonBuffer, LANG_CHANGE, mBUTTON_SPRSHEET_ROWS, mBUTTON_SPRSHEET_COLS, 160, 32, mUI_TXT_SIZE, mFontAsset, mWhiteText, mLocalization.getTranslation("Language"));

	mOptionsMusic.initGuiElement(_DisplayWidth / 2 - 75, 315);
	mOptionsMusic.addGuiButton(mButtonBuffer, CHANGE_AUDIO, mBUTTON_SPRSHEET_ROWS, mBUTTON_SPRSHEET_COLS, 160, 32, mUI_TXT_SIZE, mFontAsset, mWhiteText, mLocalization.getTranslation("Music"));

	mOptionsReturn.initGuiElement(_DisplayWidth / 2 - 75, 380);
	mOptionsReturn.addGuiButton(mButtonBuffer, RETURN_OPTIONS, mBUTTON_SPRSHEET_ROWS, mBUTTON_SPRSHEET_COLS, 160, 32, mUI_TXT_SIZE, mFontAsset, mWhiteText, mLocalization.getTranslation("RETURN TO MAIN"));

	//CONTROLS
	mControlsText.initGuiElementWithText(_DisplayWidth / 2 - 75, _DisplayHeight / 8, mUI_SIZE, mFontAsset, mWhiteText, mLocalization.getTranslation("CONTROLS"));
	
	mControlsKeyboard.initGuiElementWithText(_DisplayWidth / 4 - 75, _DisplayHeight / 2 - 200, mUI_SIZE, mFontAsset, mWhiteText, mLocalization.getTranslation("KEYBOARD/MOUSE"));
	mControlsEsc.initGuiElementWithText(_DisplayWidth / 4 - 75, _DisplayHeight / 2 - 150, mUI_SIZE, mFontAsset, mWhiteText, mLocalization.getTranslation("ESC -- Pause Game"));
	mControlsEnter.initGuiElementWithText(_DisplayWidth / 4 - 75, _DisplayHeight / 2 - 100, mUI_SIZE, mFontAsset, mWhiteText, mLocalization.getTranslation("ENTER -- Select"));
	mControlsW.initGuiElementWithText(_DisplayWidth / 4 - 75, _DisplayHeight / 2 - 50, mUI_SIZE, mFontAsset, mWhiteText, mLocalization.getTranslation("W -- Up"));
	mControlsA.initGuiElementWithText(_DisplayWidth / 4 - 75, _DisplayHeight / 2 - 25, mUI_SIZE, mFontAsset, mWhiteText, mLocalization.getTranslation("A -- Left"));
	mControlsS.initGuiElementWithText(_DisplayWidth / 4 - 75, _DisplayHeight / 2, mUI_SIZE, mFontAsset, mWhiteText, mLocalization.getTranslation("S -- Down"));
	mControlsD.initGuiElementWithText(_DisplayWidth / 4 - 75, _DisplayHeight / 2 + 25, mUI_SIZE, mFontAsset, mWhiteText, mLocalization.getTranslation("D -- Right"));
	mControlsLMB.initGuiElementWithText(_DisplayWidth / 4 - 75, _DisplayHeight  /2 + 75, mUI_SIZE, mFontAsset, mWhiteText, mLocalization.getTranslation("MOVE MOUSE -- Rotate Player"));
	mControlsMouseMove.initGuiElementWithText(_DisplayWidth / 4 - 75, _DisplayHeight /2 +125, mUI_SIZE, mFontAsset, mWhiteText, mLocalization.getTranslation("LEFT MOUSE-- Shoot"));
	
	mControlsGamepad.initGuiElementWithText(_DisplayWidth / 2 + 75, _DisplayHeight / 2 - 200, mUI_SIZE, mFontAsset, mWhiteText, mLocalization.getTranslation("GAMEPAD"));
	mControlsStart.initGuiElementWithText(_DisplayWidth / 2 + 75, _DisplayHeight / 2 - 150, mUI_SIZE, mFontAsset, mWhiteText, mLocalization.getTranslation("START -- Pause Game"));
	mControlsAButton.initGuiElementWithText(_DisplayWidth / 2 + 75, _DisplayHeight / 2 - 100, mUI_SIZE, mFontAsset, mWhiteText, mLocalization.getTranslation("A -- Select"));
	mControlsLS.initGuiElementWithText(_DisplayWidth / 2 + 75, _DisplayHeight / 2 - 55, mUI_SIZE, mFontAsset, mWhiteText, mLocalization.getTranslation("LEFT STICK -- Up, Left, Right, Down"));
	mControlsRS.initGuiElementWithText(_DisplayWidth / 2 + 75, _DisplayHeight / 2, mUI_SIZE, mFontAsset, mWhiteText, mLocalization.getTranslation("RIGHT STICK -- Rotate Player"));
	mControlsRT.initGuiElementWithText(_DisplayWidth / 2 + 75, _DisplayHeight / 2 + 50, mUI_SIZE, mFontAsset, mWhiteText, mLocalization.getTranslation("RT -- Shoot"));

	mControlsReturn.initGuiElement(_DisplayWidth / 2 - 75, 595);
	mControlsReturn.addGuiButton(mButtonBuffer, RETURN_TO_OPTIONS, mBUTTON_SPRSHEET_ROWS, mBUTTON_SPRSHEET_COLS, 160, 32, mUI_TXT_SIZE, mFontAsset, mWhiteText, mLocalization.getTranslation("RETURN TO MAIN"));

	//CREDITS
	mCreditsText.initGuiElementWithText(_DisplayWidth / 2 - 90, _DisplayHeight / 8, mUI_SIZE, mFontAsset, mWhiteText, mLocalization.getTranslation("CREDITS"));

	mCreditsWalter.initGuiElementWithText(_DisplayWidth / 2 - 180, 150, mUI_SIZE, mFontAsset, mWhiteText, mLocalization.getTranslation("Walter Hill -- Programming, Sprites, Design"));
	mCreditsAudio.initGuiElementWithText(_DisplayWidth / 2 - 90, 200, mUI_SIZE, mFontAsset, mWhiteText, mLocalization.getTranslation("AUDIO"));
	mCreditsMusic1.initGuiElementWithText(_DisplayWidth / 2 - 180, 250, mUI_SIZE, mFontAsset, mWhiteText, mLocalization.getTranslation("Cabled_mess -- Clack_Minimal UI Sounds"));
	mCreditsMusic2.initGuiElementWithText(_DisplayWidth / 2 - 180, 300, mUI_SIZE, mFontAsset, mWhiteText, mLocalization.getTranslation("Cabled_mess -- Boom_C_03"));
	mCreditsMusic3.initGuiElementWithText(_DisplayWidth / 2 - 180, 350, mUI_SIZE, mFontAsset, mWhiteText, mLocalization.getTranslation("Gravity Sound -- Cloudy"));
	mCreditsMusic4.initGuiElementWithText(_DisplayWidth / 2 - 180, 400, mUI_SIZE, mFontAsset, mWhiteText, mLocalization.getTranslation("Josepharaoh99 -- button pressed"));
	mCreditsMusic5.initGuiElementWithText(_DisplayWidth / 2 - 180, 450, mUI_SIZE, mFontAsset, mWhiteText, mLocalization.getTranslation("Xythe -- Fight for your life loop"));
	mCreditsMusic6.initGuiElementWithText(_DisplayWidth / 2 - 180, 500, mUI_SIZE, mFontAsset, mWhiteText, mLocalization.getTranslation("Xtrgamr -- music game, win or high score"));

	mCreditsReturn.initGuiElement(_DisplayWidth / 2 - 75, 595);
	mCreditsReturn.addGuiButton(mButtonBuffer, RETURN_CREDITS, mBUTTON_SPRSHEET_ROWS, mBUTTON_SPRSHEET_COLS, 160, 32, mUI_TXT_SIZE, mFontAsset, mWhiteText, mLocalization.getTranslation("RETURN TO MAIN"));

	/*** add ui objects to manager ***/
	//NOTE: order matters for buttons in the mangers

	//MAIN MENU UI MANAGER
	mGuiManagerMain.setNumButtons(5);
	mGuiManagerMain.addToManager("title", &mMainTitle);
	mGuiManagerMain.addToManager("1", &mMainStart);
	mGuiManagerMain.addToManager("2", &mMainStats);
	mGuiManagerMain.addToManager("3", &mMainOptions);
	mGuiManagerMain.addToManager("4", &mMainCredits);
	mGuiManagerMain.addToManager("5", &mMainQuit);

	mGuiManagerMain.addToManager("fps", &mFpscounter);

	//GAME UI MANAGER
	mGuiManagerGame.addToManager("combo", &mGameCombo);
	mGuiManagerGame.addToManager("score", &mGameScore);
	mGuiManagerGame.addToManager("time", &mGameTime);
	mGuiManagerGame.addToManager("fragments", &mGameFragmentsCollected);
	mGuiManagerGame.addToManager("fps", &mFpscounter);

	//PAUSE UI MANAGER
	mGuiManagerPause.setNumButtons(2);
	mGuiManagerPause.addToManager("1", &mPausePlay);
	mGuiManagerPause.addToManager("2", &mPauseQuit);
	mGuiManagerPause.addToManager("text", &mPauseText);
	mGuiManagerPause.addToManager("fps", &mFpscounter);

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
	mGuiManagerWin.addToManager("score", &mWinScore);
	mGuiManagerWin.addToManager("time", &mWinTime);
	mGuiManagerWin.addToManager("fps", &mFpscounter);

	//STATS SCREEN UI MANAGER
	mGuiManagerStats.setNumButtons(1);
	mGuiManagerStats.addToManager("1", &mStatsReturn);
	mGuiManagerStats.addToManager("title", &mStatsText);
	mGuiManagerStats.addToManager("besttime", &mStatsFastestTime);
	mGuiManagerStats.addToManager("files", &mStatsFilesCaptured);
	mGuiManagerStats.addToManager("highcombo", &mStatsHighCombo);
	mGuiManagerStats.addToManager("highscore", &mStatsHighScore);
	mGuiManagerStats.addToManager("lifescore", &mStatsLifetimeScore);
	mGuiManagerStats.addToManager("lifeplayed", &mStatsTimePlayed);
	mGuiManagerStats.addToManager("fps", &mFpscounter);


	//CREDITS SCREEN UI MANAGER
	mGuiManagerCredits.setNumButtons(1);
	mGuiManagerCredits.addToManager("1", &mCreditsReturn);
	mGuiManagerCredits.addToManager("title", &mCreditsText);
	mGuiManagerCredits.addToManager("walter", &mCreditsWalter);
	mGuiManagerCredits.addToManager("audio", &mCreditsAudio);
	mGuiManagerCredits.addToManager("m1", &mCreditsMusic1);
	mGuiManagerCredits.addToManager("m2", &mCreditsMusic2);
	mGuiManagerCredits.addToManager("m3", &mCreditsMusic3);
	mGuiManagerCredits.addToManager("m4", &mCreditsMusic4);
	mGuiManagerCredits.addToManager("m5", &mCreditsMusic5);
	mGuiManagerCredits.addToManager("m6", &mCreditsMusic6);
	mGuiManagerCredits.addToManager("fps", &mFpscounter);

	//OPTIONS SCREEN UI MANAGER
	mGuiManagerOptions.setNumButtons(5);
	mGuiManagerOptions.addToManager("1", &mOptionsControls);
	mGuiManagerOptions.addToManager("2", &mOptionsToggleController);
	mGuiManagerOptions.addToManager("3", &mOptionsLang);
	mGuiManagerOptions.addToManager("4", &mOptionsMusic);
	mGuiManagerOptions.addToManager("5", &mOptionsReturn);
	mGuiManagerOptions.addToManager("title", &mOptionsText);
	mGuiManagerOptions.addToManager("fps", &mFpscounter);

	//CONTROLS SCREEN UI MANAGER
	mGuiManagerControls.setNumButtons(1);
	mGuiManagerControls.addToManager("1", &mControlsReturn);
	mGuiManagerControls.addToManager("title", &mControlsText);
	mGuiManagerControls.addToManager("gamepad", &mControlsGamepad);
	mGuiManagerControls.addToManager("start", &mControlsStart);
	mGuiManagerControls.addToManager("abutton", &mControlsAButton);
	mGuiManagerControls.addToManager("rs", &mControlsRS);
	mGuiManagerControls.addToManager("ls", &mControlsLS);
	mGuiManagerControls.addToManager("rt", &mControlsRT);
	mGuiManagerControls.addToManager("keys", &mControlsKeyboard);
	mGuiManagerControls.addToManager("w", &mControlsW);
	mGuiManagerControls.addToManager("a", &mControlsA);
	mGuiManagerControls.addToManager("s", &mControlsS);
	mGuiManagerControls.addToManager("d", &mControlsD);
	mGuiManagerControls.addToManager("enter", &mControlsEnter);
	mGuiManagerControls.addToManager("esc", &mControlsEsc);
	mGuiManagerControls.addToManager("lmb", &mControlsLMB);
	mGuiManagerControls.addToManager("move", &mControlsMouseMove);
	mGuiManagerControls.addToManager("fps", &mFpscounter);

	cout << "*******Initialized UI*******" << endl;
}

void Game::loadScenes()
{
	mMainMenuScene.initScene(SC_MAIN, &mGuiManagerMain, &mMenuSprite);
	mGameScene.initScene(SC_GAME, &mGuiManagerGame, &mGameScreenSprite);
	mLoseScene.initScene(SC_LOSE, &mGuiManagerLose, &mLoseScreenSprite);
	mWinScene.initScene(SC_WIN, &mGuiManagerWin, &mWinScreenSprite);
	mPauseScene.initScene(SC_PAUSE, &mGuiManagerPause, &mGameScreenSprite);
	mStatsScene.initScene(SC_STATS, &mGuiManagerStats, &mStatsScreenSprite);
	mCreditScene.initScene(SC_CREDITS, &mGuiManagerCredits, &mCreditsScreenSprite);
	mOptionsScene.initScene(SC_OPTIONS, &mGuiManagerOptions, &mOptionsScreenSprite);
	mControlsScene.initScene(SC_CONTROLS, &mGuiManagerControls, &mOptionsScreenSprite);

	//add to manager
	mSceneManager.addScene("a", &mMainMenuScene);
	mSceneManager.addScene("b", &mGameScene);
	mSceneManager.addScene("c", &mLoseScene);
	mSceneManager.addScene("d", &mWinScene);
	mSceneManager.addScene("e", &mPauseScene);
	mSceneManager.addScene("f", &mStatsScene);
	mSceneManager.addScene("g", &mCreditScene);
	mSceneManager.addScene("h", &mOptionsScene);
	mSceneManager.addScene("i", &mControlsScene);

	mSceneManager.setCurrentScene(SC_MAIN);

	_Scene = &mSceneManager;

	cout << "~~| Twisting loose screws |~~" << endl;
	cout << "*******Scenes Loaded*******" << endl;
}

void Game::initAudio()
{
	mPlayerShoot.initSound(true, mLOCAL_ASSET_PATH+mPlayerShootSound, false);
	mPlayerHit.initSound(true, mLOCAL_ASSET_PATH + mPlayerHitSound, false);
	mEnemyShoot.initSound(true, mLOCAL_ASSET_PATH + mEnemyShootSound, false);
	mEnemyHit.initSound(true, mLOCAL_ASSET_PATH + mEnemyHitSound, false);
	mButtonMove.initSound(true, mLOCAL_ASSET_PATH + mButtonMoveSound, false);
	mButtonSelect.initSound(true, mLOCAL_ASSET_PATH + mButtonSelectSound, false);
	mFragmentPickup.initSound(true, mLOCAL_ASSET_PATH + mFragmentPickupSound, false);
	mPlayerLose.initSound(true, mLOCAL_ASSET_PATH + mPlayerLoseSound, false);
	mMenuMusic.initSound(false, mLOCAL_ASSET_PATH + mMenuSound, true);
	mGameMusic.initSound(false, mLOCAL_ASSET_PATH + mGameSound, true);
	mWinMusic.initSound(true, mLOCAL_ASSET_PATH + mWinSound, false);

	mSceneManager.addAudio(PLAYER_HIT, &mPlayerHit);
	mSceneManager.addAudio(PLAYER_SHOOT, &mPlayerShoot);
	mSceneManager.addAudio(ENEMY_HIT, &mEnemyHit);
	mSceneManager.addAudio(ENEMY_SHOOT, &mEnemyShoot);
	mSceneManager.addAudio(BUTTON_MOVE, &mButtonMove);
	mSceneManager.addAudio(BUTTON_SEL, &mButtonSelect);
	mSceneManager.addAudio(FRAG_PICKUP, &mFragmentPickup);
	mSceneManager.addAudio(PLAYER_LOSE, &mPlayerLose);
	mSceneManager.addAudio(MENU_MUSIC, &mMenuMusic);
	mSceneManager.addAudio(GAME_MUSIC, &mGameMusic);
	mSceneManager.addAudio(WIN_MUSIC, &mWinMusic);
}
#pragma endregion

bool Game::saveGame(bool saveAllData) //TODO: use to save stats after win/lose
{
	bool result = false;
		   
	CSimpleIniA ini;
	ini.LoadFile(mINI_FILE.c_str());
	
	// overwrite last save - deleting an entire section and all keys in it
	ini.Delete("GAME", NULL);
	
	if (saveAllData)
	{
		ini.SetLongValue("STATS", "lifetimescore", mLifetimeScore);
		ini.SetLongValue("STATS", "hoursplayed", hoursPlayed);
		ini.SetLongValue("STATS", "minutesplayed", minutesPlayed);
		ini.SetLongValue("STATS", "secondsplayed", secondsPlayed);
		ini.SetValue("STATS", "besttime", mBestTime.c_str());
		ini.SetLongValue("STATS", "highscore", mHighScore);
		ini.SetLongValue("STATS", "highcombo", mHighCombo);
		ini.SetLongValue("STATS", "filescaptured", mFilesCaptured);
	}
	else 
	{
		//just save time play
		ini.SetLongValue("STATS", "hoursplayed", hoursPlayed);
		ini.SetLongValue("STATS", "minutesplayed", minutesPlayed);
		ini.SetLongValue("STATS", "secondsplayed", secondsPlayed);
	}

	ini.SaveFile(mINI_FILE.c_str());

	cout << "saved" << endl;

	return result;
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

	delete pPerformanceTracker;
	delete frameTimer;

	return false;
}


void Game::update(double timeElapsed)
{
	//Input
	mSystem.getInputSystem()->update(timeElapsed);

	//update displays

	#ifdef _DEBUG
		mSceneManager.update(timeElapsed, mHighScore, mHighCombo, mBestTime, hoursPlayed, minutesPlayed, mLifetimeScore, mFilesCaptured, musicValue, controllerInUse, _ComboCount, _Score, mCollectedPercentage, min, sec, mFPS);
	#else //for release
		mSceneManager.update(timeElapsed, mHighScore, mHighCombo, mBestTime, hoursPlayed, minutesPlayed, mLifetimeScore, mFilesCaptured, _ComboCount, _Score, mCollectedPercentage, min, sec);
	#endif

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
		mCollectedPercentage = 100*(_NumFragments / _FragmentsToCollect);

		//check lose condition
		if (!mPlayer.isVisible())
		{
			secondsPlayed += sec;
			if (secondsPlayed >= 60)
			{
				secondsPlayed = secondsPlayed - 60;
				minutesPlayed++;
			}
			if (minutesPlayed >= 60 && minutesPlayed % 60 == 0)
			{
				minutesPlayed = 0;
				hoursPlayed += 1;
			}

			mSceneManager.stopSound(GAME_MUSIC);
			mSceneManager.playSfx(PLAYER_LOSE);
			mSceneManager.setCurrentScene(SC_LOSE);
			
			saveGame(false);

		}
		//check win condition
		if (_NumFragments == _FragmentsToCollect)
		{
			mSceneManager.stopSound(GAME_MUSIC);
			mSceneManager.playSfx(WIN_MUSIC);
			mSceneManager.setCurrentScene(SC_WIN);
			
			mFilesCaptured++; //total files
			mLifetimeScore += _Score; //overall score

			if (_Score > mHighScore) //high score
				mHighScore = _Score;
			if (_ComboCount > mHighCombo) //high combo
				mHighCombo = _ComboCount;
			
			//best time
			if (bestMin == 0 && bestSec == 0)
			{
				string timeStr = to_string(min) + ":" + to_string(sec);
				mBestTime = timeStr;
			}
			else if (min <= bestMin && sec <= bestSec)
			{
				string timeStr = to_string(min) + ":" + to_string(sec);
				mBestTime = timeStr;
			}

			secondsPlayed += sec;
			if (secondsPlayed >= 60)
			{
				secondsPlayed = secondsPlayed - 60;
				minutesPlayed++;
			}
			if (minutesPlayed >= 60 && minutesPlayed % 60 == 0)
			{
				minutesPlayed = 0;
				hoursPlayed += 1;
			}
			
			saveGame(true);
		}

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

		minutesPlayed++;
		
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
	mPlayer.getAnimation()->setLooping(false);
	mPlayer.shouldAnimate(false);
	mPlayer.setVisible(true);
	mPlayer.setDown(false);
	mPlayer.setUp(false);
	mPlayer.setLeft(false);
	mPlayer.setRight(false);
	mPlayer.setLastLife(false);
	mPlayer.setLoc(_LevelWidth / 2, _LevelHeight / 2);

	//reset gameplay data
	_ComboCount = 0;
	_Score = 0;
	_NumFragments = 0;
	dtTime = 0;
	min = 0;
	sec = 0;

	delete survivalTimer;
	survivalTimer = new Timer;
	survivalTimer->start();
}

void Game::render()
{
	//draw scene background
	mSceneManager.draw(mSystem.getGraphicsSystem());

	if (mSceneManager.getCurrentScene() == SC_GAME || mSceneManager.getCurrentScene() == SC_PAUSE)
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
		if (mSceneManager.getCurrentScene() == SC_GAME)
			mSceneManager.setCurrentScene(SC_PAUSE);
		else if(mSceneManager.getCurrentScene() == SC_PAUSE)
			mSceneManager.setCurrentScene(SC_GAME);
		break;

	case BUTTON_SELECT:
		//button actions for each scene
		if (mSceneManager.getCurrentScene() == SC_MAIN)
		{
			if (mGuiManagerMain.getButtonEventPressed(NEW_GAME))
			{
				if (!firstPlay)
				{
					if(musicValue == 1)
						mSceneManager.setCurrentMusic(GAME_MUSIC);	
					resetGameState();
				}
				else
				{
					mSceneManager.stopCurrentMusic();
					mSceneManager.playSound(GAME_MUSIC);
					
					firstPlay = false;
					survivalTimer = new Timer;
					survivalTimer->start();
				}

				mSceneManager.setCurrentScene(SC_GAME);				
			}
			else if (mGuiManagerMain.getButtonEventPressed(STATS))
			{
				mSceneManager.setCurrentScene(SC_STATS);
			}
			else if (mGuiManagerMain.getButtonEventPressed(OPTIONS))
			{
				mSceneManager.setCurrentScene(SC_OPTIONS);
			}
			else if (mGuiManagerMain.getButtonEventPressed(CREDITS))
			{
				mSceneManager.setCurrentScene(SC_CREDITS);
			}
			else if (mGuiManagerMain.getButtonEventPressed(QUIT))
			{
				mIsRunning = false;
			}
		}
		else if (mSceneManager.getCurrentScene() == SC_CONTROLS)
		{
			if (mGuiManagerControls.getButtonEventPressed(RETURN_TO_OPTIONS))
			{
				mSceneManager.setCurrentScene(SC_OPTIONS);
			}
		}
		else if (mSceneManager.getCurrentScene() == SC_OPTIONS)
		{
			if (mGuiManagerOptions.getButtonEventPressed(LANG_CHANGE))
			{
				if(mLocalization.getLanguage() == ENGLISH)
					mLocalization.setLangauge(FRENCH);
				else
					mLocalization.setLangauge(ENGLISH);
			}
			else if (mGuiManagerOptions.getButtonEventPressed(CHANGE_AUDIO))
			{
				//toggle music on/off
				if (musicValue == 0) //turn on
				{
					musicValue = 1;
					mSceneManager.playSound(MENU_MUSIC);
				}
				else //turn off
				{
					musicValue = 0;
					mSceneManager.stopSound(MENU_MUSIC);
				}
			}
			else if (mGuiManagerOptions.getButtonEventPressed(SHOW_CONTROLS))
			{
				//go to control scene w/ controls in text
				mSceneManager.setCurrentScene(SC_CONTROLS);
			}
			else if (mGuiManagerOptions.getButtonEventPressed(CONTROLLER))
			{
				if (!controllerInUse)
					controllerInUse = true;
				else
					controllerInUse = false;

				mPlayer.setControllerConnected(controllerInUse);
			}
			else if (mGuiManagerOptions.getButtonEventPressed(RETURN_OPTIONS))
			{
				mSceneManager.setCurrentScene(SC_MAIN);
			}
		}
		else if (mSceneManager.getCurrentScene() == SC_PAUSE)
		{
			if (mGuiManagerPause.getButtonEventPressed(RESUME))
			{
				mSceneManager.setCurrentScene(SC_GAME);
			}
			else if (mGuiManagerPause.getButtonEventPressed(RETURN_MAIN))
			{
				mSceneManager.stopCurrentMusic();
				if (musicValue == 1)
					mSceneManager.playSound(MENU_MUSIC);

				mSceneManager.setCurrentScene(SC_MAIN);
			}
		}
		else if (mSceneManager.getCurrentScene() == SC_LOSE)
		{
			if (mGuiManagerLose.getButtonEventPressed(NEW_GAME))
			{
				resetGameState();
				mSceneManager.stopCurrentMusic();
				if (musicValue == 1)
					mSceneManager.playSound(GAME_MUSIC);
				mSceneManager.setCurrentScene(SC_GAME);
			}
			else if (mGuiManagerLose.getButtonEventPressed(RETURN_MAIN))
			{
				mSceneManager.stopCurrentMusic();
				if (musicValue == 1)
					mSceneManager.playSound(MENU_MUSIC);
				mSceneManager.setCurrentScene(SC_MAIN);
			}
		}
		else if (mSceneManager.getCurrentScene() == SC_WIN)
		{
			if (mGuiManagerWin.getButtonEventPressed(NEW_GAME))
			{
				//restart game & set scene to game
				resetGameState();
				mSceneManager.stopCurrentMusic();
				if (musicValue == 1)
					mSceneManager.playSound(GAME_MUSIC);
				mSceneManager.setCurrentScene(SC_GAME);
			}
			else if (mGuiManagerWin.getButtonEventPressed(RETURN_MAIN))
			{
				mSceneManager.stopSound(WIN_MUSIC);
				if (musicValue == 1)
					mSceneManager.playSound(MENU_MUSIC);
				mSceneManager.setCurrentScene(SC_MAIN);
			}
		}

		else if (mSceneManager.getCurrentScene() == SC_CREDITS)
		{
			if (mGuiManagerCredits.getButtonEventPressed(RETURN_CREDITS))
			{
				mSceneManager.setCurrentScene(SC_MAIN);
			}
		}
		else if (mSceneManager.getCurrentScene() == SC_STATS)
		{
			if (mGuiManagerStats.getButtonEventPressed(RETURN_STATS))
			{
				mSceneManager.setCurrentScene(SC_MAIN);
			}
		}
		mSceneManager.playSfx(BUTTON_SEL);
		break;
	case SHOOT:
		if (mSceneManager.getCurrentScene() == SC_GAME)
		{
			if (mPlayer.isVisible())
			{
				bulletSpawnX = mPlayer.getX();
				bulletSpawnY = mPlayer.getY();

				mBulletManager.fireProjectile(mFRAME_TIME_60FPS, bulletSpawnX, bulletSpawnY, mPlayer.getRotation());
				mSceneManager.playSfx(PLAYER_SHOOT);

				mGameView.toggleScreenShake(true);
			}
		}
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
		if (!controllerInUse)
		{
			if (mSceneManager.getCurrentScene() == SC_GAME)
			{
				mPlayer.setDown(true);
			}
			else if (mSceneManager.getCurrentScene() == SC_MAIN)
			{
				mSceneManager.moveCursorDown(SC_MAIN, BUTTON_MOVE);
			}
			else if (mSceneManager.getCurrentScene() == SC_STATS)
			{
				mSceneManager.moveCursorDown(SC_STATS, BUTTON_MOVE);
			}
			else if (mSceneManager.getCurrentScene() == SC_CREDITS)
			{
				mSceneManager.moveCursorDown(SC_CREDITS, BUTTON_MOVE);
			}
			else if (mSceneManager.getCurrentScene() == SC_OPTIONS)
			{
				mSceneManager.moveCursorDown(SC_OPTIONS, BUTTON_MOVE);
			}
			else if (mSceneManager.getCurrentScene() == SC_PAUSE)
			{
				mSceneManager.moveCursorDown(SC_PAUSE, BUTTON_MOVE);
			}
			else if (mSceneManager.getCurrentScene() == SC_LOSE)
			{
				mSceneManager.moveCursorDown(SC_LOSE, BUTTON_MOVE);
			}
			else if (mSceneManager.getCurrentScene() == SC_WIN)
			{
				mSceneManager.moveCursorDown(SC_WIN, BUTTON_MOVE);
			}
		}
		break;

	case MOVE_UP:
		if (!controllerInUse)
		{
			if (mSceneManager.getCurrentScene() == SC_GAME)
			{
				mPlayer.setUp(true);
			}
			else if (mSceneManager.getCurrentScene() == SC_MAIN)
			{
				mSceneManager.moveCursorUp(SC_MAIN, BUTTON_MOVE);
			}
			else if (mSceneManager.getCurrentScene() == SC_CREDITS)
			{
				mSceneManager.moveCursorUp(SC_CREDITS, BUTTON_MOVE);
			}
			else if (mSceneManager.getCurrentScene() == SC_STATS)
			{
				mSceneManager.moveCursorUp(SC_STATS, BUTTON_MOVE);
			}
			else if (mSceneManager.getCurrentScene() == SC_OPTIONS)
			{
				mSceneManager.moveCursorUp(SC_OPTIONS, BUTTON_MOVE);
			}
			else if (mSceneManager.getCurrentScene() == SC_PAUSE)
			{
				mSceneManager.moveCursorUp(SC_PAUSE, BUTTON_MOVE);
			}
			else if (mSceneManager.getCurrentScene() == SC_LOSE)
			{
				mSceneManager.moveCursorUp(SC_LOSE, BUTTON_MOVE);
			}
			else if (mSceneManager.getCurrentScene() == SC_WIN)
			{
				mSceneManager.moveCursorUp(SC_WIN, BUTTON_MOVE);
			}
		}
		break;

	case MOVE_LEFT:
		if (mSceneManager.getCurrentScene() == SC_GAME)
		{
			if (!controllerInUse)
				mPlayer.setLeft(true);
		}
		break;

	case MOVE_RIGHT:
		if (mSceneManager.getCurrentScene() == SC_GAME)
		{
			if (!controllerInUse)
				mPlayer.setRight(true);
		}
		break;

	case STOP_DOWN:
		if (mSceneManager.getCurrentScene() == SC_GAME)
		{
			if (!controllerInUse)
				mPlayer.setDown(false);
		}

		break;

	case STOP_UP:
		if (mSceneManager.getCurrentScene() == SC_GAME)
		{
			if (!controllerInUse)
				mPlayer.setUp(false);
		}
		break;


	case STOP_LEFT: 
		if (mSceneManager.getCurrentScene() == SC_GAME)
		{
				if(!controllerInUse)
					mPlayer.setLeft(false);		
		}
		break;

	case STOP_RIGHT:
		if (mSceneManager.getCurrentScene() == SC_GAME)
		{
			if(!controllerInUse)
				mPlayer.setRight(false);
		}
		break;

	case MOVE_DOWN_CONTROLLER:
		if (controllerInUse)
		{
			if (mSceneManager.getCurrentScene() == SC_GAME)
			{
				mPlayer.setDown(true);
			}
			else if (mSceneManager.getCurrentScene() == SC_MAIN)
			{
				if (!menuButtonMoved)
				{
					menuButtonMoved = true;
					mSceneManager.moveCursorDown(SC_MAIN, BUTTON_MOVE);
				}
			}
			else if (mSceneManager.getCurrentScene() == SC_STATS)
			{
				mSceneManager.moveCursorDown(SC_STATS, BUTTON_MOVE);
			}
			else if (mSceneManager.getCurrentScene() == SC_CREDITS)
			{
				mSceneManager.moveCursorDown(SC_CREDITS, BUTTON_MOVE);
			}
			else if (mSceneManager.getCurrentScene() == SC_OPTIONS)
			{
				if (!menuButtonMoved)
				{
					menuButtonMoved = true;

					mSceneManager.moveCursorDown(SC_OPTIONS, BUTTON_MOVE);
				}
			}
			else if (mSceneManager.getCurrentScene() == SC_PAUSE)
			{
				if (!menuButtonMoved)
				{
					menuButtonMoved = true;
					mSceneManager.moveCursorDown(SC_PAUSE, BUTTON_MOVE);
				}
			}
			else if (mSceneManager.getCurrentScene() == SC_LOSE)
			{
				if (!menuButtonMoved)
				{
					menuButtonMoved = true; 
					mSceneManager.moveCursorDown(SC_LOSE, BUTTON_MOVE);
				}
			}
			else if (mSceneManager.getCurrentScene() == SC_WIN)
			{
				if (!menuButtonMoved)
				{
					menuButtonMoved = true;
					mSceneManager.moveCursorDown(SC_WIN, BUTTON_MOVE);
				}
			}
		}
		break;

	case MOVE_UP_CONTROLLER:
		if (controllerInUse)
		{
			if (mSceneManager.getCurrentScene() == SC_GAME)
			{
				mPlayer.setUp(true);
			}
			else if (mSceneManager.getCurrentScene() == SC_MAIN)
			{
				if (!menuButtonMoved)
				{
					menuButtonMoved = true;
					mSceneManager.moveCursorUp(SC_MAIN, BUTTON_MOVE);
				}
			}
			else if (mSceneManager.getCurrentScene() == SC_CREDITS)
			{
				mSceneManager.moveCursorUp(SC_CREDITS, BUTTON_MOVE);
			}
			else if (mSceneManager.getCurrentScene() == SC_STATS)
			{
				mSceneManager.moveCursorUp(SC_STATS, BUTTON_MOVE);
			}
			else if (mSceneManager.getCurrentScene() == SC_OPTIONS)
			{
				if (!menuButtonMoved)
				{
					menuButtonMoved = true;
					mSceneManager.moveCursorUp(SC_OPTIONS, BUTTON_MOVE);
				}
			}
			else if (mSceneManager.getCurrentScene() == SC_PAUSE)
			{
				if (!menuButtonMoved)
				{
					menuButtonMoved = true;
					mSceneManager.moveCursorUp(SC_PAUSE, BUTTON_MOVE);
				}
			}
			else if (mSceneManager.getCurrentScene() == SC_LOSE)
			{
				if (!menuButtonMoved)
				{
					menuButtonMoved = true;
					mSceneManager.moveCursorUp(SC_LOSE, BUTTON_MOVE);
				}
			}
			else if (mSceneManager.getCurrentScene() == SC_WIN)
			{
				if (!menuButtonMoved)
				{
					menuButtonMoved = true;	
					mSceneManager.moveCursorUp(SC_WIN, BUTTON_MOVE);
				}
			}
		}
		break;

	case MOVE_LEFT_CONTROLLER:
		if (mSceneManager.getCurrentScene() == SC_GAME)
		{
			if (controllerInUse)
				mPlayer.setLeft(true);
		}
		break;

	case MOVE_RIGHT_CONTROLLER:
		if (mSceneManager.getCurrentScene() == SC_GAME)
		{
			if (controllerInUse)
				mPlayer.setRight(true);
		}
		break;

	case STOP_DOWN_CONTROLLER:
		if (mSceneManager.getCurrentScene() == SC_GAME)
		{
			if (controllerInUse)
				mPlayer.setDown(false);
		}
		else if (mSceneManager.getCurrentScene() == SC_MAIN)
		{
			if (menuButtonMoved)
				menuButtonMoved = false;
		}
		else if (mSceneManager.getCurrentScene() == SC_OPTIONS)
		{
			if (menuButtonMoved)
				menuButtonMoved = false;
		}
		else if (mSceneManager.getCurrentScene() == SC_WIN)
		{
			if (menuButtonMoved)
				menuButtonMoved = false;
		}
		else if (mSceneManager.getCurrentScene() == SC_LOSE)
		{
			if (menuButtonMoved)
				menuButtonMoved = false;
		}
		else if (mSceneManager.getCurrentScene() == SC_PAUSE)
		{
			if (menuButtonMoved)
				menuButtonMoved = false;
		}

		break;

	case STOP_UP_CONTROLLER:
		if (mSceneManager.getCurrentScene() == SC_GAME)
		{
			if (controllerInUse)
				mPlayer.setUp(false);
		}
		else if (mSceneManager.getCurrentScene() == SC_MAIN)
		{
			if (menuButtonMoved)
				menuButtonMoved = false;
		}
		else if (mSceneManager.getCurrentScene() == SC_OPTIONS)
		{
			if (menuButtonMoved)
				menuButtonMoved = false;
		}
		else if (mSceneManager.getCurrentScene() == SC_WIN)
		{
			if (menuButtonMoved)
				menuButtonMoved = false;
		}
		else if (mSceneManager.getCurrentScene() == SC_LOSE)
		{
			if (menuButtonMoved)
				menuButtonMoved = false;
		}
		else if (mSceneManager.getCurrentScene() == SC_PAUSE)
		{
			if (menuButtonMoved)
				menuButtonMoved = false;
		}

		break;

	case STOP_LEFT_CONTROLLER:
		if (mSceneManager.getCurrentScene() == SC_GAME)
		{
			if (controllerInUse)
				mPlayer.setLeft(false);
		}
		break;

	case STOP_RIGHT_CONTROLLER:
		if (mSceneManager.getCurrentScene() == SC_GAME)
		{
			if (controllerInUse)
				mPlayer.setRight(false);
		}
		break;
	case TOGGLE_CONTROLLER:
		if (!controllerInUse)
			controllerInUse = true;
		else
			controllerInUse = false;

		mPlayer.setControllerConnected(controllerInUse);

		break;

	case SCREENCAP: //debug only
		takeScreenshot = true;
		cout << "SCREENSHOT CAPTURED!" << endl;
		break;

	}
}
