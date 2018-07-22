#include "Game.h"

Game* Game::mGameInstance = NULL;



//creating exe
//https://discourse.libsdl.org/t/creating-an-easily-distributable-executable-file/24413


//collisions
//https://www.gamedev.net/articles/programming/general-and-gameplay-programming/intelligent-2d-collision-and-pixel-perfect-precision-r3311/

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
	mLoadingScreen.initGraphicsBuffer(mSystem.getGraphicsSystem()->getBackbuffer(), mLevelWidth, mLevelHeight, mLOCAL_ASSET_PATH + mLOADING_IMG);
	mBufferManager.addGraphicsBuffer(mLOAD_ID, &mLoadingScreen);

	mLoadingSprite.initSprite(mBufferManager.getGraphicsBuffer(mLOAD_ID), 0, 0, mBufferManager.getGraphicsBuffer(mLOAD_ID)->getBitmapWidth(),
		mBufferManager.getGraphicsBuffer(mLOAD_ID)->getBitmapHeight());

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
	if (!mSystem.initSystem(GAME_TITLE, mDisplayWidth, mDisplayHeight, mGameView.getCamera()))
		return false;

	cout << "*******Initialized system*******" << endl;

	displayLoadingScreen();

	//Initialize game-side input translation from event system
	mInputTranslator.initInputTranslator();
	
	cout << "*******Initialized input translator*******" << endl;
	cout << "~~| Applying duct tape and elbow grease |~~" << endl;

	installListeners();
	loadBackgrounds();
	initUI();
	loadScenes();
	initAudio();

	
	mPlayerAnim.addSpriteSheet(mBufferManager.getGraphicsBuffer(mPLAYER_ID), 1, 2, 16, 16);
		
	mPlayer.init(mLevelWidth, mLevelHeight, mDisplayWidth, mDisplayHeight);
	mPlayer.setAnimation(mPlayerAnim);
	mPlayer.shouldAnimate(false);
	mPlayer.setCollider(PLAYER_COL_TAG);
	mPlayer.setLoc(450,200);

	mRoninAnim.addSpriteSheet(mBufferManager.getGraphicsBuffer(mRONIN_ID), 1, 1, 32, 32);

	//mRonin.setAnimation(mRoninAnim);
	//mRonin.setCollider(RONIN_COL_TAG);
	//mRonin.setLoc(200, 200);
	srand(unsigned(time(NULL)));

	
	for (int i = 0; i < mNumRonin; i++)
	{

		int randX = rand() % 1500;
		int randY = rand() % 1100;

		mEnemyManager.createAndAddEntity(mEnemyManTag + to_string(i), randX, randY, mRoninAnim);
	}

	//set player bullet sprite
	mBulletAnim.addSpriteSheet(mBufferManager.getGraphicsBuffer(mBULLET_ID), 1, 1, 15, 15);
	mBulletManager.initBulletData(mBulletAnim, mLevelWidth, mLevelHeight, true, BULLET_COL_TAG);

  	mGameView.initView(&mPlayer, mDisplayWidth, mDisplayHeight, mLevelWidth, mLevelHeight);

 	mIsRunning = true;

	pPerformanceTracker->stopTracking(mINIT_TRACKER_NAME);
	cout << endl << "Time to Init:" << pPerformanceTracker->getElapsedTime(mINIT_TRACKER_NAME) << " ms" << endl;

	delete pPerformanceTracker;

	return true;
}


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
	const char * iniNumRonin = ini.GetValue("LEVELDATA", "numRonin", "defualt");
	const char * spriteSize = ini.GetValue("VIEW", "tileSize", "default");


	mDisplayWidth = atoi(iniWidth);
	mDisplayHeight = atoi(iniHeight);
	
	mLevelWidth = atoi(iniLevelWidth);
	mLevelHeight = atoi(iniLevelHeight);

	mNumEnemies = atoi(iniNumEnemies);
	mNumRonin = atoi(iniNumRonin);

	cout << "*******Loaded game data*******" << endl;
}

void Game::initAudio()
{
	
}

void Game::loadBackgrounds()
{
	//Initialize bitmaps using initGraphicsBuffer()
	mPlayerBuffer.initGraphicsBuffer(mSystem.getGraphicsSystem()->getBackbuffer(), mDisplayWidth, mDisplayHeight, mLOCAL_ASSET_PATH + mPLAYER_ASSET);
	mBulletBuffer.initGraphicsBuffer(mSystem.getGraphicsSystem()->getBackbuffer(), mDisplayWidth, mDisplayHeight, mLOCAL_ASSET_PATH + mBULLET_ASSET);
	mRoninBuffer.initGraphicsBuffer(mSystem.getGraphicsSystem()->getBackbuffer(), mDisplayWidth, mDisplayHeight, mLOCAL_ASSET_PATH + mRONIN_ASSET);

	//Add buffers to buffer manager
	mBufferManager.addGraphicsBuffer(mPLAYER_ID, &mPlayerBuffer);
	mBufferManager.addGraphicsBuffer(mBULLET_ID, &mBulletBuffer);
	mBufferManager.addGraphicsBuffer(mRONIN_ID, &mRoninBuffer);


	//init Background sprites using initSprite()
	

	cout << "*******Initialized buffers*******" << endl;
}

void Game::loadLocalization()
{
	mLocalization.loadLanguage(ENGLISH);
	

	mSceneManager.initLanguages(&mLocalization);

	cout << "*******Loaded languages*******" << endl;
}

void Game::initUI()
{
	mWhiteText.setColor(255);

	mFpscounter.initGuiElement(mGEN_TAG, 10, 10);
	mFpscounter.initGuiText(12, mWhiteText, to_string(mFPS));

	//add to manager
	mGuiManagerMain.addGuiElement("fps", &mFpscounter);

	cout << "*******Initialized UI*******" << endl;
}

void Game::loadScenes()
{
	mMainMenu.initScene(SC_GAME, true, &mGuiManagerMain, &mLoadingSprite);

	//add to manager
	mSceneManager.addScene("a", &mMainMenu);
	
	mSceneManager.setCurrentScene(SC_GAME);

	cout << "~~| Twisting loose screws |~~" << endl;
	cout << "*******Scenes Loaded*******" << endl;
}




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
	delete mFont;
	mFont = NULL;

	delete mGameUI;
	mGameUI = NULL;

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
	mSceneManager.update(timeElapsed, mFPS);
	
	if (mSceneManager.getCurrentScene() == SC_GAME)
	{
		mEnemyManager.update(timeElapsed);
		//TODO: check collisions on all enemies
		mBulletManager.update(timeElapsed, mEnemyManager.getEntity("e0")->getCollider());
		mPlayer.update(timeElapsed, mEnemyManager.getEntity("e0")->getCollider(), mouseX, mouseY, mGameView.getCamera()->getX(), mGameView.getCamera()->getY());
		mGameView.update(timeElapsed);
	}
}


void Game::render()
{
 	mSceneManager.draw(mSystem.getGraphicsSystem());

	mPlayer.draw(mSystem.getGraphicsSystem(), mGameView.getCamera()->getX(), mGameView.getCamera()->getY());
	mBulletManager.draw(mSystem.getGraphicsSystem(), mGameView.getCamera()->getX(), mGameView.getCamera()->getY());

	mEnemyManager.draw(mSystem.getGraphicsSystem(), mGameView.getCamera()->getX(), mGameView.getCamera()->getY());
	//mRonin.draw(mSystem.getGraphicsSystem(), mGameView.getCamera()->getX(), mGameView.getCamera()->getY());

	if (takeScreenshot)
	{
		mSystem.getGraphicsSystem()->takeScreenshot(mSystem.getGraphicsSystem()->getBackbuffer(), mScreencapFilename + to_string(mCapNum) + ".png");
		takeScreenshot = false;
		mCapNum++;
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
		
		bulletSpawnX = mPlayer.getX();
		bulletSpawnY = mPlayer.getY();

		mBulletManager.fireProjectile(mFRAME_TIME_60FPS, bulletSpawnX, bulletSpawnY, mPlayer.getRotation());

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
		//mSystem.getGraphicsSystem()->takeScreenshot(mSystem.getGraphicsSystem(), "ss1.png");
		cout << "SCREENSHOT CAPTURED!" << endl;
		break;

	}


}
