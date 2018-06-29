#include "Game.h"

Game* Game::mGameInstance = NULL;

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

	cout << "*******Initialized listeners*******" << endl;
}

void Game::displayLoadingScreen()
{
	//show image while game data & assets load in
	mLoadingScreen.initGraphicsBuffer(mSystem.getGraphicsSystem()->getBackbuffer(), mDisplayWidth, mDisplayHeight, mLOCAL_ASSET_PATH + "spacepurple.bmp");
	mBufferManager.addGraphicsBuffer(mLOAD_ID, &mLoadingScreen);

	mLoadingSprite.initSprite(mBufferManager.getGraphicsBuffer(mLOAD_ID), 0, 0, mBufferManager.getGraphicsBuffer(mLOAD_ID)->getBitmapWidth(),
		mBufferManager.getGraphicsBuffer(mLOAD_ID)->getBitmapHeight());

	mSystem.getGraphicsSystem()->draw(0, 0, mLoadingSprite, 1.0f, 1.0f);
	mSystem.getGraphicsSystem()->flip();
}

bool Game::initGame()
{
	PerformanceTracker* pPerformanceTracker = new PerformanceTracker;
	pPerformanceTracker->startTracking(mINIT_TRACKER_NAME);

	//srand(unsigned(time(NULL)));

	loadGameData();
	loadLocalization();

	//Initialize graphics, controls, and event system
	if (!mSystem.initSystem(mDisplayWidth, mDisplayHeight))
		return false;

	cout << "*******Initialized system*******" << endl;

	//displayLoadingScreen();

	//Initialize game-side input translation from event system
	mInputTranslator.initInputTranslator();
	
	cout << "*******Initialized input translator*******" << endl;
	cout << "~~| Applying duct tape and elbow grease |~~" << endl;

	installListeners();
	loadBackgrounds();
	initUI();
	loadScenes();
	initAudio();

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
	const char * spriteSize = ini.GetValue("VIEW", "tileSize", "default");


	mDisplayWidth = atoi(iniWidth);
	mDisplayHeight = atoi(iniHeight);
	
	cout << "*******Loaded game data*******" << endl;
}

void Game::initAudio()
{
	
}

void Game::loadBackgrounds()
{
	//Initialize bitmaps

	//Add buffers to manager
	
	//Background sprites
	

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
	mMainMenu.initScene(SC_MAIN, true, &mGuiManagerMain, &mLoadingSprite);

	//add to manager
	mSceneManager.addScene("a", &mMainMenu);
	
	mSceneManager.setCurrentScene(SC_MAIN);

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
	}
}


void Game::render()
{
	mSceneManager.draw(mSystem.getGraphicsSystem());

	mSystem.getGraphicsSystem()->flip();
}


void Game::handleEvent(const Event& theEvent)
{
	mEventType = theEvent.getType();

	cout << endl << "Game Listener" << endl;
	cout << "\t" << theEvent.getEventName() << " Received: ";

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

	case MOVE_DOWN:
		cout << endl;
		if (mSceneManager.getCurrentScene() == SC_GAME)
		{
			
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
		cout << endl;
		if (mSceneManager.getCurrentScene() == SC_GAME)
		{
			
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
		cout << endl;
		if (mSceneManager.getCurrentScene() == SC_GAME)
		{

		}
		break;

	case MOVE_RIGHT:
		cout << endl;
		if (mSceneManager.getCurrentScene() == SC_GAME)
		{

		}
		break;
	}
}