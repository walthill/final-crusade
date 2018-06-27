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

	srand(unsigned(time(NULL)));

	loadGameData();
	loadLocalization();

	//Initialize graphics, controls, and event system
	if (!mSystem.initSystem(mDisplayWidth, mDisplayHeight))
		return false;

	cout << "*******Initialized system*******" << endl;

	displayLoadingScreen();

	//Initialize game-side input translation from event system
	mInputTranslator.initInputTranslator();
	
	cout << "*******Initialized input translator*******" << endl;
	cout << "~~| Applying duct tape and elbow grease |~~" << endl;

	initAudio();

	mWhiteText.setColor(255);

	installListeners();
	initUI();
	loadBackgrounds();
	loadScenes();

	//tetris data structures
	mBlockObjectVector.resize(7);
	mBlockBufferVector.resize(8); //7 pieces and the wall
	mBlock_ID.resize(8);

	loadBlockImages();
	initBlockTypes();

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

	const char * startBool = ini.GetValue("STATE", "startAtMenu", "default");
	const char * numBlocksPerTile = ini.GetValue("STATE", "numBlocks", "default");
	const char * wellW = ini.GetValue("STATE", "gridW", "default");
	const char * wellH = ini.GetValue("STATE", "gridH", "default");
	const char * offset = ini.GetValue("STATE", "off", "default");
	const char * lvl = ini.GetValue("STATE", "playerLvl", "default");
	const char * tonextlvl = ini.GetValue("STATE", "tonextlvl", "default");
	//const char * totalscore = ini.GetValue("STATE", "lifetimescore", "default");
	const char * gameSpeed = ini.GetValue("STATE", "speed", "default");
	const char * speed2 = ini.GetValue("STATE", "speed1", "default");
	const char * speed3 = ini.GetValue("STATE", "speed2", "default");

	const char * scores = ini.GetValue("STATE", "lifetimescore", "default");
	const char * clears = ini.GetValue("STATE", "lifetimeclears", "default");
	const char * highLevel = ini.GetValue("STATE", "highestlevel", "default");


	mDisplayWidth = atoi(iniWidth);
	mDisplayHeight = atoi(iniHeight);
	mStartGame = atoi(startBool);
	mNumBlocks = atoi(numBlocksPerTile);
	mSpriteSize = atoi(spriteSize);
	mGridW = atoi(wellW);
	mGridH = atoi(wellH);
	mOffset = atoi(offset);
	
	mSpeed1 = atoi(gameSpeed);
	mBlockSpeed = mSpeed1;
	mSpeed2 = atoi(speed2);
	mSpeed3 = atoi(speed3);

	mPlayerLevelNum = atoi(lvl);
	mExpToNextLevel = atoi(tonextlvl);

	mLifetimeClears = atoi(clears);
	mLifetimeScore = atoi(scores);
	mHighestLevel = atoi(highLevel);

	cout << "*******Loaded game data*******" << endl;
}

void Game::initAudio()
{
	mTest.initSound(false, mLOCAL_ASSET_PATH + mMUSIC_WII, true);
	mTetrisTheme.initSound(false, mLOCAL_ASSET_PATH + mMUSIC_TETRIS, true);
	mTetrisThemeDub.initSound(false, mLOCAL_ASSET_PATH + mMUSIC_TETRISDUB, true);
	mRasputinTheme.initSound(false, mLOCAL_ASSET_PATH + mMUSIC_RASPUTIN, true);

	mButtonMove.initSound(true, mLOCAL_ASSET_PATH + mFX_MOVE, false);
	mButtonSelect.initSound(true, mLOCAL_ASSET_PATH + mFX_SELECT, false);
	mBlockDropSound.initSound(true, mLOCAL_ASSET_PATH + mFX_LOCK, false);
	mLineClearSound.initSound(true, mLOCAL_ASSET_PATH + mFX_CLEAR, false);
}

void Game::loadBackgrounds()
{
	//Initialize bitmaps
	mMenuBuffer.initGraphicsBuffer(mSystem.getGraphicsSystem()->getBackbuffer(), mDisplayWidth, mDisplayHeight, mLOCAL_ASSET_PATH + mSPACE_BASE);
	mSpaceBlue.initGraphicsBuffer(mSystem.getGraphicsSystem()->getBackbuffer(), mDisplayWidth, mDisplayHeight, mLOCAL_ASSET_PATH + mSPACE_BLUE);
	mCredit.initGraphicsBuffer(mSystem.getGraphicsSystem()->getBackbuffer(), mDisplayWidth, mDisplayHeight, mLOCAL_ASSET_PATH + mCREDIT_BG);

	//Add buffers to manager
	mBufferManager.addGraphicsBuffer(mMENU_ID, &mMenuBuffer);
	mBufferManager.addGraphicsBuffer(mSPACE_ID, &mSpaceBlue);
	mBufferManager.addGraphicsBuffer(mCREDIT_ID, &mCredit);

	//Background sprites
	mBackgroundSprite.initSprite(mBufferManager.getGraphicsBuffer(mSPACE_ID), 0, 0, mBufferManager.getGraphicsBuffer(mSPACE_ID)->getBitmapWidth(),
		mBufferManager.getGraphicsBuffer(mSPACE_ID)->getBitmapHeight());

	mMenuSprite.initSprite(mBufferManager.getGraphicsBuffer(mMENU_ID), 0, 0, mBufferManager.getGraphicsBuffer(mMENU_ID)->getBitmapWidth(),
		mBufferManager.getGraphicsBuffer(mMENU_ID)->getBitmapHeight());

	mCreditBG.initSprite(mBufferManager.getGraphicsBuffer(mCREDIT_ID), 0, 0, mBufferManager.getGraphicsBuffer(mCREDIT_ID)->getBitmapWidth(),
		mBufferManager.getGraphicsBuffer(mCREDIT_ID)->getBitmapHeight());

	cout << "*******Initialized buffers*******" << endl;
}

void Game::loadLocalization()
{
	mLocalization.loadLanguage(ENGLISH);
	mLocalization.loadLanguage(SPANISH);
	mLocalization.loadLanguage(SWAHILI);

	mSceneManager.initLanguages(&mLocalization);

	cout << "*******Loaded languages*******" << endl;
}

void Game::initUI()
{
	//button image
	mStartButton.initGraphicsBuffer(mSystem.getGraphicsSystem()->getBackbuffer(), 319, 35, mLOCAL_ASSET_PATH + mBUTTON);

	//main menu
	mMainTitle.initGuiElement(mMAINMENU_TAG, 345, 80);
	mMainTitle.initGuiText(mFONT_SIZE, mWhiteText, "TETRIS");

	mStart.initGuiElement(mMAINMENU_TAG, 270, 115);
	mStart.initGuiText(18, mWhiteText, mLocalization.getTranslation("PRESS ENTER TO SELECT"));

	mPlayerLevel.initGuiElement(mMAINMENU_TAG, 10, 35);
	mPlayerLevel.initGuiText(16, mWhiteText, mLocalization.getTranslation("YOUR LEVEL") + ": " + to_string(mPlayerLevelNum));//localization.getTranslation("PRESS ESCAPE TO QUIT"));

	mExpToNextLevelTxt.initGuiElement(mMAINMENU_TAG, 10, 55);
	mExpToNextLevelTxt.initGuiText(16, mWhiteText, mLocalization.getTranslation("EXP TO NEXT LEVEL")+ ": " + to_string(mExpToNextLevel));//localization.getTranslation("PRESS ESCAPE TO QUIT"));

	mFpscounter.initGuiElement(mSTD_UI_TAG, 10, 10);
	mFpscounter.initGuiText(12, mWhiteText, to_string(mFPS));

	//add to manager
	mGuiManagerMain.addGuiElement("mainmenu1", &mMainTitle);
	mGuiManagerMain.addGuiElement("mainmenu2", &mStart);
	mGuiManagerMain.addGuiElement("playerlevel", &mPlayerLevel);
	mGuiManagerMain.addGuiElement("tonextlevel", &mExpToNextLevelTxt);
	mGuiManagerMain.addGuiElement("fps", &mFpscounter);


	//in game

	mLevel.initGuiElement("game", 25, 25);
	mLevel.initGuiText(mUI_SIZE, mWhiteText, mLocalization.getTranslation("LEVEL") + ": " + to_string(mLevelNum));

	mScoreToWin.initGuiElement("game", 25, 60);
	mScoreToWin.initGuiText(mUI_SIZE, mWhiteText, mLocalization.getTranslation("SCORE TO WIN") + ": " + to_string(mScoreToReach));

	mScoreTxt.initGuiElement("game", 25, 80);
	mScoreTxt.initGuiText(mUI_SIZE, mWhiteText, mLocalization.getTranslation("SCORE") + ": " + to_string(mScore));

	mNext.initGuiElement("game", 550, 125);
	mNext.initGuiText(mUI_SIZE, mWhiteText, mLocalization.getTranslation("NEXT BLOCK") + ": " + to_string(mScore));

	//add to manager
	mGuiManagerGame.addGuiElement("fps", &mFpscounter);
	mGuiManagerGame.addGuiElement("score", &mScoreTxt);
	mGuiManagerGame.addGuiElement("reach", &mScoreToWin);
	mGuiManagerGame.addGuiElement("level", &mLevel);
	mGuiManagerGame.addGuiElement("next", &mNext);

	//Main menu buttons
	mGuiManagerMain.setNumButtons(5);

	mButton.initGuiElement(mMAINMENU_TAG, 100, 200);
	mButton.initGuiButton(mStartButton, NEW_GAME, 1, 2, 160, 32, 12, mWhiteText, mLocalization.getTranslation("NEW GAME"));

	mButton2.initGuiElement(mMAINMENU_TAG, 100, 400);
	mButton2.initGuiButton(mStartButton, QUIT, 1, 2, 160, 32, 12, mWhiteText, mLocalization.getTranslation("QUIT"));

	mButton3.initGuiElement(mMAINMENU_TAG, 100, 250);
	mButton3.initGuiButton(mStartButton, LOAD_GAME, 1, 2, 160, 32, 12, mWhiteText, mLocalization.getTranslation("LOAD GAME"));

	mButton4.initGuiElement(mMAINMENU_TAG, 100, 300);
	mButton4.initGuiButton(mStartButton, OPTIONS, 1, 2, 160, 32, 12, mWhiteText, mLocalization.getTranslation("OPTIONS"));

	mCreditsButton.initGuiElement(mMAINMENU_TAG, 100, 350);
	mCreditsButton.initGuiButton(mStartButton, CREDITS, 1, 2, 160, 32, 12, mWhiteText, "Credits");

	//order of key names matters here
	mGuiManagerMain.addGuiElement("0", &mButton); //play
	mGuiManagerMain.addGuiElement("1", &mButton3); //
	mGuiManagerMain.addGuiElement("2", &mButton4); //
	mGuiManagerMain.addGuiElement("3", &mCreditsButton); //credits
	mGuiManagerMain.addGuiElement("4", &mButton2); //quit
	
	//options menu buttons
	mGuiManagerOptions.setNumButtons(5);
	mOptionsTitle.initGuiElement(mOPTIONS_TAG, 300, 75);
	mOptionsTitle.initGuiText(mFONT_SIZE, mWhiteText, mLocalization.getTranslation("OPTIONS"));

	mLanguageButton.initGuiElement(mOPTIONS_TAG, 100, 200);
	mLanguageButton.initGuiButton(mStartButton, LANG_CHANGE, 1, 2, 160, 32, 12, mWhiteText, mLocalization.getTranslation("LANGUAGE"));

	mDifficulty.initGuiElement(mOPTIONS_TAG, 100, 250);
	mDifficulty.initGuiButton(mStartButton, CHANGE_DIFFICULTY, 1, 2, 160, 32, 12, mWhiteText, mLocalization.getTranslation("GAME SPEED"));

	mAudio.initGuiElement(mOPTIONS_TAG, 100, 300);
	mAudio.initGuiButton(mStartButton, CHANGE_AUDIO, 1, 2, 160, 32, 12, mWhiteText, mLocalization.getTranslation("AUDIO"));

	mReturnToMain.initGuiElement(mOPTIONS_TAG, 100, 400);
	mReturnToMain.initGuiButton(mStartButton, RETURN_MAIN, 1, 2, 160, 32, 12, mWhiteText, mLocalization.getTranslation("RETURN"));

	mStatsButton.initGuiElement(mOPTIONS_TAG, 100, 350);
	mStatsButton.initGuiButton(mStartButton, STATS, 1, 2, 160, 32, 12, mWhiteText, mLocalization.getTranslation("STATS"));

	//add to manager
	mGuiManagerOptions.addGuiElement("0", &mOptionsTitle);
	mGuiManagerOptions.addGuiElement("1", &mLanguageButton);
	mGuiManagerOptions.addGuiElement("2", &mDifficulty);
	mGuiManagerOptions.addGuiElement("3", &mAudio);
	mGuiManagerOptions.addGuiElement("4", &mStatsButton);
	mGuiManagerOptions.addGuiElement("5", &mReturnToMain);
	mGuiManagerOptions.addGuiElement("fps", &mFpscounter);

	//pause menu ui
	mPauseLevel.initGuiElement(mPAUSE_TAG, 25, 25);
	mPauseLevel.initGuiText(mUI_SIZE, mWhiteText, mLocalization.getTranslation("LEVEL") + ": " + to_string(mLevelNum));

	mPauseScoreToWin.initGuiElement(mPAUSE_TAG, 25, 60);
	mPauseScoreToWin.initGuiText(mUI_SIZE, mWhiteText, mLocalization.getTranslation("SCORE TO WIN") + ": " + to_string(mScoreToReach));
	
	mPauseScore.initGuiElement(mPAUSE_TAG, 25, 80);
	mPauseScore.initGuiText(mUI_SIZE, mWhiteText, mLocalization.getTranslation("SCORE") + ": " + to_string(mScore));
	
	mPause.initGuiElement(mPAUSE_TAG, 105, 180);
	mPause.initGuiText(mUI_SIZE, mWhiteText, mLocalization.getTranslation("PAUSE GAME"));
	
	mResumeButton.initGuiElement(mPAUSE_TAG, 100, 220);
	mResumeButton.initGuiButton(mStartButton, RESUME, 1, 2, 160, 32, 12, mWhiteText, "RESUME");//localization.getTranslation("PLAY GAME"));

	mReturnButton.initGuiElement(mPAUSE_TAG, 100, 270);
	mReturnButton.initGuiButton(mStartButton, RETURN_MAIN, 1, 2, 160, 32, 12, mWhiteText, mLocalization.getTranslation("SAVE AND QUIT"));

	mPauseNext.initGuiElement(mPAUSE_TAG, 550, 125);
	mPauseNext.initGuiText(mUI_SIZE, mWhiteText, mLocalization.getTranslation("NEXT BLOCK") + ": " + to_string(mScore));

	mGuiManagerPause.setNumButtons(2);
	mGuiManagerPause.addGuiElement("score", &mPauseScore);
	mGuiManagerPause.addGuiElement("reach", &mPauseScoreToWin);
	mGuiManagerPause.addGuiElement("level", &mPauseLevel);
	mGuiManagerPause.addGuiElement("fps", &mFpscounter);
	mGuiManagerPause.addGuiElement("pause", &mPause);
	mGuiManagerPause.addGuiElement("0", &mResumeButton);
	mGuiManagerPause.addGuiElement("1", &mReturnButton);
	mGuiManagerPause.addGuiElement("next", &mPauseNext);

	//lose screen UI
	mLost.initGuiElement(mL_TAG, 325, 205);
	mLost.initGuiText(mUI_SIZE, mWhiteText, mLocalization.getTranslation("YOU LOST"));
	
	mRetryButton.initGuiElement(mL_TAG, 300, 230);
	mRetryButton.initGuiButton(mStartButton, NEW_GAME, 1, 2, 160, 32, 12, mWhiteText, mLocalization.getTranslation("NEW GAME"));

	mQuitLoseButton.initGuiElement(mL_TAG, 300, 270);
	mQuitLoseButton.initGuiButton(mStartButton, QUIT_LOSE, 1, 2, 160, 32, 12, mWhiteText, mLocalization.getTranslation("RETURN"));

	mGuiManagerLose.setNumButtons(2);
	mGuiManagerLose.addGuiElement("fps", &mFpscounter);
	mGuiManagerLose.addGuiElement("lost", &mLost);
	mGuiManagerLose.addGuiElement("0", &mRetryButton);
	mGuiManagerLose.addGuiElement("1", &mQuitLoseButton);

	//credits screen ui
	mReturnFromCreditsButton.initGuiElement(mCREDITS_TAG, 100, 270);
	mReturnFromCreditsButton.initGuiButton(mStartButton, RETURN_MAIN_CREDITS, 1, 2, 160, 32, 12, mWhiteText, mLocalization.getTranslation("CREDITS"));

	mDevelopersTxt.initGuiElement(mCREDITS_TAG, 275, 100);
	mDevelopersTxt.initGuiText(mUI_SIZE, mWhiteText, "Cool Dudes");
	mTetrisArtistTxt.initGuiElement(mCREDITS_TAG, 275, 150);
	mTetrisArtistTxt.initGuiText(mUI_SIZE, mWhiteText, "Tet Song person");
	mTetrisTrapArtistTxt.initGuiElement(mCREDITS_TAG, 275, 200);
	mTetrisTrapArtistTxt.initGuiText(mUI_SIZE, mWhiteText, "Tet Trap Song person");
	mRasputinArtistTxt.initGuiElement(mCREDITS_TAG, 275, 250);
	mRasputinArtistTxt.initGuiText(mUI_SIZE, mWhiteText, "Rasp Song person");
	mTetrisImageTxt.initGuiElement(mCREDITS_TAG, 275, 300);
	mTetrisImageTxt.initGuiText(mUI_SIZE, mWhiteText, "Tetris Image artist");
	mWiiThemeTxt.initGuiElement(mCREDITS_TAG, 275, 350);
	mWiiThemeTxt.initGuiText(mUI_SIZE, mWhiteText, "Wii theme person");

	mGuiManagerCredits.setNumButtons(1);
	mGuiManagerCredits.addGuiElement("0", &mReturnFromCreditsButton);
	mGuiManagerCredits.addGuiElement("fps", &mFpscounter);
	mGuiManagerCredits.addGuiElement("devs", &mDevelopersTxt);
	mGuiManagerCredits.addGuiElement("tetrisArtist", &mTetrisArtistTxt);
	mGuiManagerCredits.addGuiElement("tetrisTrapArtist", &mTetrisTrapArtistTxt);
	mGuiManagerCredits.addGuiElement("raspArtist", &mRasputinArtistTxt);
	mGuiManagerCredits.addGuiElement("tetrisImageArtist", &mTetrisImageTxt);
	mGuiManagerCredits.addGuiElement("wiiThemeArtist", &mWiiThemeTxt);

	//stats screen ui
	mHighestLevelTxt.initGuiElement(mSTATS_TAG, 300, 235);
	mHighestLevelTxt.initGuiText(mUI_SIZE, mWhiteText, mLocalization.getTranslation("HIGHEST LEVEL REACHED")+": " + to_string(mHighestLevel));

	mTotalScoreTxt.initGuiElement(mSTATS_TAG, 300, 270);
	mTotalScoreTxt.initGuiText(mUI_SIZE, mWhiteText, mLocalization.getTranslation("LIFETIME SCORE") + ": " + to_string(mLifetimeScore));

	mTotalClearsTxt.initGuiElement(mSTATS_TAG, 300, 305);
	mTotalClearsTxt.initGuiText(mUI_SIZE, mWhiteText, mLocalization.getTranslation("LIFETIME CLEARS") + ": " + to_string(mLifetimeClears));


	mReturnFromStatsButton.initGuiElement(mSTATS_TAG, 100, 270);
	mReturnFromStatsButton.initGuiButton(mStartButton, RETURN_OPTIONS, 1, 2, 160, 32, 12, mWhiteText, mLocalization.getTranslation("RETURN"));


	mGuiManagerStats.setNumButtons(1);
	mGuiManagerStats.addGuiElement("0", &mReturnFromStatsButton);
	mGuiManagerStats.addGuiElement("a", &mHighestLevelTxt);
	mGuiManagerStats.addGuiElement("b", &mTotalClearsTxt);
	mGuiManagerStats.addGuiElement("c", &mTotalScoreTxt);
	mGuiManagerStats.addGuiElement("fps", &mFpscounter);


	cout << "*******Initialized UI*******" << endl;
}

void Game::loadScenes()
{
	mMainMenu.initScene(SC_MAIN, true, &mGuiManagerMain, &mMenuSprite);
	mGame.initScene(SC_GAME, false, &mGuiManagerGame, &mBackgroundSprite);
	mOptions.initScene(SC_OPTIONS, false, &mGuiManagerOptions, &mMenuSprite);
	mPauseScene.initScene(SC_PAUSE, false, &mGuiManagerPause, &mBackgroundSprite);
	mLoseScene.initScene(SC_LOSE, false, &mGuiManagerLose, &mBackgroundSprite);
	mCreditScene.initScene(SC_CREDITS, false, &mGuiManagerCredits, &mCreditBG);
	mStatsScene.initScene(SC_STATS, false, &mGuiManagerStats, &mMenuSprite);

	//TODO: make snakey boi in credits screen

	//add to manager
	mSceneManager.addScene("a", &mMainMenu);
	mSceneManager.addScene("b", &mGame);
	mSceneManager.addScene("c", &mOptions);
	mSceneManager.addScene("d", &mPauseScene);
	mSceneManager.addScene("e", &mLoseScene);
	mSceneManager.addScene("f", &mCreditScene);
	mSceneManager.addScene("g", &mStatsScene);

	mSceneManager.addAudio("mii", &mTest);
	mSceneManager.addAudio("tetris", &mTetrisTheme);
	mSceneManager.addAudio("tetrisdub", &mTetrisThemeDub);
	mSceneManager.addAudio("rasputin", &mRasputinTheme);
	mSceneManager.setCurrentMusic("tetris");

	mSceneManager.addAudio("buttonmove", &mButtonMove);
	mSceneManager.addAudio("buttonselect", &mButtonSelect);
	mSceneManager.addAudio("blocklock", &mBlockDropSound);
	mSceneManager.addAudio("lineclear", &mLineClearSound);

	mSceneManager.setCurrentScene(SC_MAIN);

	cout << "~~| Twisting loose screws |~~" << endl;
	cout << "*******Scenes Loaded*******" << endl;
}

void Game::loadBlockImages()
{
	//Block buffers
	//L Block
	mBlockBufferVector[0].initGraphicsBuffer(mSystem.getGraphicsSystem()->getBackbuffer(), mSpriteSize, mSpriteSize, mLOCAL_ASSET_PATH + mL_BLOCK);
	mBlock_ID[0] = mBLOCK_STR + to_string(0);
	mBufferManager.addGraphicsBuffer(mBlock_ID[0], &mBlockBufferVector[0]);

	//I Block
	mBlockBufferVector[1].initGraphicsBuffer(mSystem.getGraphicsSystem()->getBackbuffer(), mSpriteSize, mSpriteSize, mLOCAL_ASSET_PATH + mI_BLOCK);
	mBlock_ID[1] = mBLOCK_STR + to_string(1);
	mBufferManager.addGraphicsBuffer(mBlock_ID[1], &mBlockBufferVector[1]);

	//Z Block
	mBlockBufferVector[2].initGraphicsBuffer(mSystem.getGraphicsSystem()->getBackbuffer(), mSpriteSize, mSpriteSize, mLOCAL_ASSET_PATH + mZ_BLOCK);
	mBlock_ID[2] = mBLOCK_STR + to_string(2);
	mBufferManager.addGraphicsBuffer(mBlock_ID[2], &mBlockBufferVector[2]);

	//S Block
	mBlockBufferVector[3].initGraphicsBuffer(mSystem.getGraphicsSystem()->getBackbuffer(), mSpriteSize, mSpriteSize, mLOCAL_ASSET_PATH + mS_BLOCK);
	mBlock_ID[3] = mBLOCK_STR + to_string(3);
	mBufferManager.addGraphicsBuffer(mBlock_ID[3], &mBlockBufferVector[3]);

	//O Block
	mBlockBufferVector[4].initGraphicsBuffer(mSystem.getGraphicsSystem()->getBackbuffer(), mSpriteSize, mSpriteSize, mLOCAL_ASSET_PATH + mO_BLOCK);
	mBlock_ID[4] = mBLOCK_STR + to_string(4);
	mBufferManager.addGraphicsBuffer(mBlock_ID[4], &mBlockBufferVector[4]);

	//T Block
	mBlockBufferVector[5].initGraphicsBuffer(mSystem.getGraphicsSystem()->getBackbuffer(), mSpriteSize, mSpriteSize, mLOCAL_ASSET_PATH + mT_BLOCK);
	mBlock_ID[5] = mBLOCK_STR + to_string(5);
	mBufferManager.addGraphicsBuffer(mBlock_ID[5], &mBlockBufferVector[5]);

	//J Block
	mBlockBufferVector[6].initGraphicsBuffer(mSystem.getGraphicsSystem()->getBackbuffer(), mSpriteSize, mSpriteSize, mLOCAL_ASSET_PATH + mJ_BLOCK);
	mBlock_ID[6] = mBLOCK_STR + to_string(6);
	mBufferManager.addGraphicsBuffer(mBlock_ID[6], &mBlockBufferVector[6]);

	//Wall Block
	mBlockBufferVector[7].initGraphicsBuffer(mSystem.getGraphicsSystem()->getBackbuffer(), mSpriteSize, mSpriteSize, mLOCAL_ASSET_PATH + mWALLBLOCK);
	mBlock_ID[7] = mBLOCK_STR + to_string(7);
	mBufferManager.addGraphicsBuffer(mBlock_ID[7], &mBlockBufferVector[7]);

	cout << "*******Blocks loaded to buffer*******" << endl;

	//init block sprites
	mBlock1Sprite.initSprite(mBufferManager.getGraphicsBuffer(mBlock_ID[0]), 0, 0, mBufferManager.getGraphicsBuffer(mBlock_ID[0])->getBitmapWidth(),
		mBufferManager.getGraphicsBuffer(mBlock_ID[0])->getBitmapHeight());

	mBlock2Sprite.initSprite(mBufferManager.getGraphicsBuffer(mBlock_ID[1]), 0, 0, mBufferManager.getGraphicsBuffer(mBlock_ID[1])->getBitmapWidth(),
		mBufferManager.getGraphicsBuffer(mBlock_ID[1])->getBitmapHeight());

	mBlock3Sprite.initSprite(mBufferManager.getGraphicsBuffer(mBlock_ID[2]), 0, 0, mBufferManager.getGraphicsBuffer(mBlock_ID[2])->getBitmapWidth(),
		mBufferManager.getGraphicsBuffer(mBlock_ID[2])->getBitmapHeight());

	mBlock4Sprite.initSprite(mBufferManager.getGraphicsBuffer(mBlock_ID[3]), 0, 0, mBufferManager.getGraphicsBuffer(mBlock_ID[3])->getBitmapWidth(),
		mBufferManager.getGraphicsBuffer(mBlock_ID[3])->getBitmapHeight());

	mBlock5Sprite.initSprite(mBufferManager.getGraphicsBuffer(mBlock_ID[4]), 0, 0, mBufferManager.getGraphicsBuffer(mBlock_ID[4])->getBitmapWidth(),
		mBufferManager.getGraphicsBuffer(mBlock_ID[4])->getBitmapHeight());

	mBlock6Sprite.initSprite(mBufferManager.getGraphicsBuffer(mBlock_ID[5]), 0, 0, mBufferManager.getGraphicsBuffer(mBlock_ID[5])->getBitmapWidth(),
		mBufferManager.getGraphicsBuffer(mBlock_ID[5])->getBitmapHeight());

	mBlock7Sprite.initSprite(mBufferManager.getGraphicsBuffer(mBlock_ID[6]), 0, 0, mBufferManager.getGraphicsBuffer(mBlock_ID[6])->getBitmapWidth(),
		mBufferManager.getGraphicsBuffer(mBlock_ID[6])->getBitmapHeight());

	mBlock8Sprite.initSprite(mBufferManager.getGraphicsBuffer(mBlock_ID[7]), 0, 0, mBufferManager.getGraphicsBuffer(mBlock_ID[7])->getBitmapWidth(),
		mBufferManager.getGraphicsBuffer(mBlock_ID[7])->getBitmapHeight());

	cout << "~~| Hammering nails |~~" << endl;
	cout << "*******Block sprites created*******" << endl;
}

void Game::initBlockTypes()
{
	//setup wall sprite
	mBlockPile.initBlockPile(mBlock8Sprite, mGridW, mGridH, mBlock_ID[7]);

	//Z Block
	mBlockBitPositions.resize(mNumBlocks);
	mBlockBitPositions[0].setX(1);
	mBlockBitPositions[0].setY(0);
	mBlockBitPositions[1].setX(0);
	mBlockBitPositions[1].setY(0);
	mBlockBitPositions[2].setX(1);
	mBlockBitPositions[2].setY(1);
	mBlockBitPositions[3].setX(2);
	mBlockBitPositions[3].setY(1);
	mBlockObjectVector[0].initBlockObject(mNumBlocks, mBlockBitPositions, mBlock1Sprite,
										  mBlockSpeedMultiplier*mBlockSpeed, mOffset, mBlock_ID[0]);
	//I Block
	mBlockBitPositions[0].setX(1);
	mBlockBitPositions[0].setY(0);
	mBlockBitPositions[1].setX(0);
	mBlockBitPositions[1].setY(0);
	mBlockBitPositions[2].setX(2);
	mBlockBitPositions[2].setY(0);
	mBlockBitPositions[3].setX(3);
	mBlockBitPositions[3].setY(0);
	mBlockObjectVector[1].initBlockObject(mNumBlocks, mBlockBitPositions, mBlock2Sprite, 
										  mBlockSpeedMultiplier*mBlockSpeed, mOffset, mBlock_ID[1]);
	//O Block
	mBlockBitPositions[0].setX(0);
	mBlockBitPositions[0].setY(0);
	mBlockBitPositions[1].setX(1);
	mBlockBitPositions[1].setY(0);
	mBlockBitPositions[2].setX(1);
	mBlockBitPositions[2].setY(1);
	mBlockBitPositions[3].setX(0);
	mBlockBitPositions[3].setY(1);
	mBlockObjectVector[2].initBlockObject(mNumBlocks, mBlockBitPositions, mBlock3Sprite, 
										 mBlockSpeedMultiplier*mBlockSpeed, mOffset, mBlock_ID[2]);
	//S Block
	mBlockBitPositions[0].setX(1);
	mBlockBitPositions[0].setY(0);
	mBlockBitPositions[1].setX(1);
	mBlockBitPositions[1].setY(1);
	mBlockBitPositions[2].setX(0);
	mBlockBitPositions[2].setY(1);
	mBlockBitPositions[3].setX(2);
	mBlockBitPositions[3].setY(0);
	mBlockObjectVector[3].initBlockObject(mNumBlocks, mBlockBitPositions, mBlock4Sprite, 
										  mBlockSpeedMultiplier*mBlockSpeed, mOffset, mBlock_ID[3]);
	//J Block
	mBlockBitPositions[0].setX(1);
	mBlockBitPositions[0].setY(1);
	mBlockBitPositions[1].setX(2);
	mBlockBitPositions[1].setY(1);
	mBlockBitPositions[2].setX(2);
	mBlockBitPositions[2].setY(0);
	mBlockBitPositions[3].setX(0);
	mBlockBitPositions[3].setY(1);
	mBlockObjectVector[4].initBlockObject(mNumBlocks, mBlockBitPositions, mBlock5Sprite, 
										  mBlockSpeedMultiplier*mBlockSpeed, mOffset, mBlock_ID[4]);
	//L Block
	mBlockBitPositions[0].setX(1);
	mBlockBitPositions[0].setY(1);
	mBlockBitPositions[1].setX(0);
	mBlockBitPositions[1].setY(1);
	mBlockBitPositions[2].setX(0);
	mBlockBitPositions[2].setY(0);
	mBlockBitPositions[3].setX(2);
	mBlockBitPositions[3].setY(1);
	mBlockObjectVector[5].initBlockObject(mNumBlocks, mBlockBitPositions, mBlock6Sprite, 
										  mBlockSpeedMultiplier*mBlockSpeed, mOffset, mBlock_ID[5]);
	//T Block
	mBlockBitPositions[0].setX(1);
	mBlockBitPositions[0].setY(1);
	mBlockBitPositions[1].setX(0);
	mBlockBitPositions[1].setY(1);
	mBlockBitPositions[2].setX(1);
	mBlockBitPositions[2].setY(0);
	mBlockBitPositions[3].setX(2);
	mBlockBitPositions[3].setY(1);
	mBlockObjectVector[6].initBlockObject(mNumBlocks, mBlockBitPositions, mBlock7Sprite, 
										  mBlockSpeedMultiplier*mBlockSpeed, mOffset, mBlock_ID[6]);

	cout << "*******Blocks built*******" << endl;
}



bool Game::saveGame()
{
	int i;
	bool result = false;
	string occupied = "b",
		   x = "x", y = "y", tag = "t",
			blockX = "fallblockx", blockY = "fallblocky";
		   
	CSimpleIniA ini;
	ini.LoadFile(mINI_FILE.c_str());
	
	// overwrite last save - deleting an entire section and all keys in it
	ini.Delete("GAME", NULL);

	for (i = 0; i < mBlockPile.getSize(); i++)
	{
		//save value for each block's position, boolean
		ini.SetLongValue("GAME", x.c_str(), mBlockPile.getBitX(i));
		ini.SetLongValue("GAME", y.c_str(), mBlockPile.getBitY(i));

		ini.SetBoolValue("GAME", occupied.c_str(), mBlockPile.getIsOccupied(i));

		//save sprite data
		// - save graphics buffer id
		ini.SetValue("GAME", tag.c_str(), mBlockPile.getID(i).c_str());

		//increment keys
		x = "x" + to_string(i);
		y = "y" + to_string(i);
		occupied = "b" + to_string(i);
		tag = "t" + to_string(i);
	}
	
	ini.SetLongValue("GAME", "score", mScore);
	ini.SetLongValue("GAME", "scoretoget",mScoreToReach);
	ini.SetLongValue("GAME", "lvl", mLevelNum);

	ini.SetLongValue("STATE", "playerLvl", mPlayerLevelNum);
	ini.SetLongValue("STATE", "tonextlvl", mExpToNextLevel);

	ini.SetLongValue("STATE", "lifetimescore", mLifetimeScore);
	ini.SetLongValue("STATE", "lifetimeclears", mLifetimeClears);
	ini.SetLongValue("STATE", "highestlevel", mHighestLevel);

	//save falling block type
	ini.SetLongValue("GAME", "currblock", mCurrBlock);
	ini.SetLongValue("GAME", "nextblock", mNextBlock);

	for (int i = 0; i < mNumBlocks; i++)
	{
		ini.SetLongValue("GAME", ("currBlockX" + to_string(i)).c_str(), mBlockObjectVector[mCurrBlock].getBlockX(i));
		ini.SetLongValue("GAME", ("currBlockY" + to_string(i)).c_str(), mBlockObjectVector[mCurrBlock].getBlockY(i));
	}

	ini.SetLongValue("GAME", "size", mBlockPile.getSize());

	ini.SaveFile(mINI_FILE.c_str());

	cout << "saved" << endl;

	return result;
}

void Game::loadLastSave()
{
	int i;
	string occupied = "b",
		x = "x", y = "y", tag = "t",
		blockX = "fallblockx", blockY = "fallblocky";

	int xPos, yPos;
	bool isOccupied;

	CSimpleIniA ini;
	ini.LoadFile(mINI_FILE.c_str());
	
	//load level stats
	mScore = ini.GetLongValue("GAME", "score", -1);
	mScoreToReach = ini.GetLongValue("GAME", "scoretoget", -1);
	mLevelNum = ini.GetLongValue("GAME", "lvl", -1);

	//load falling blocks
	mCurrBlock = ini.GetLongValue("GAME", "currblock", -1);
	mNextBlock = ini.GetLongValue("Game", "nextBlock", -1);

	for (int i = 0; i < mNumBlocks; i++)
	{
		mBlockObjectVector[mCurrBlock].setBlockX(i, ini.GetLongValue("GAME", ("currBlockX" + to_string(i)).c_str(), -1));
		mBlockObjectVector[mCurrBlock].setBlockY(i, ini.GetLongValue("GAME", ("currBlockY" + to_string(i)).c_str(), -1));
	}

	int size = ini.GetLongValue("GAME", "size", -1);

	mBlockPile.setSize(size);

	//load in block pile
	for (i = 0; i < size; i++)
	{
		xPos = ini.GetLongValue("GAME", x.c_str(), -1);
		yPos = ini.GetLongValue("GAME", y.c_str(), -1);

		isOccupied = ini.GetBoolValue("GAME", occupied.c_str(), false);

		//save sprite data
		// - save graphics buffer id
		const char * id = ini.GetValue("GAME", tag.c_str(), "default");

		if (isOccupied == true)
		{ 
			mLoadBlock.initSprite(mBufferManager.getGraphicsBuffer(id), 0, 0, mBufferManager.getGraphicsBuffer(id)->getBitmapWidth(),
				mBufferManager.getGraphicsBuffer(id)->getBitmapHeight());
			mBlockPile.setBitX(i, xPos);
			mBlockPile.setBitY(i, yPos);
			mBlockPile.setIsOccupied(i, isOccupied);
			mBlockPile.setSprite(i, mLoadBlock);
			mBlockPile.setID(i, std::string(id));
		}
		
		//increment keys
		x = "x" + to_string(i);
		y = "y" + to_string(i);
		occupied = "b" + to_string(i);
		tag = "t" + to_string(i);

	}

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

	saveGame();

	delete pPerformanceTracker;
	delete frameTimer;

	return false;
}


void Game::update(double timeElapsed)
{
	mCanMoveBlockY = true;

	//Input
	mSystem.getInputSystem()->update(timeElapsed);

	//update displays
	mSceneManager.update(timeElapsed, mScore, mLevelNum, mPlayerLevelNum, 
						 mExpToNextLevel, mScoreToReach, (int)mBlockSpeedMultiplier, 
						 mAudioCounter, mHighestLevel, mLifetimeScore, 
						 mLifetimeClears, mFPS);
	
	if (mSceneManager.getCurrentScene() == SC_GAME)
	{
		updateTetris();
		updateScore();
	}
}


void Game::updateTetris()
{
	if (mBlockObjectVector[mCurrBlock].checkUpdate() == true)
	{
		mBlockObjectVector[mCurrBlock].update(mFRAME_TIME_60FPS, mBlockPile, mCanMoveBlockY);
		mCanMoveBlockY = false;
	}
	else
	{
		//score based on difficulty - hard=1650, med=1100, easy=550
		mClearPoints = mBlockPile.checkPileFull();
		if (mClearPoints != -1)
		{
			if (mClearPoints > 0)
				mSceneManager.playSfx("lineclear");

			mSceneManager.playSfx("blocklock");
			mScore += (int)((mBlockSpeedMultiplier / 2) * mCLEAR_PTS * mClearPoints);
			mLifetimeClears += mClearPoints;
			mLifetimeScore += mScore;
		}
		else if (mClearPoints == -1)
			mSceneManager.setCurrentScene(SC_LOSE);

		mCurrBlock = mNextBlock;

		while(mNextBlock == mCurrBlock)
			mNextBlock = rand() % 7;

		mBlockObjectVector[mNextBlock].reInitBlockObject();
	}
}


void Game::updateScore()
{	
	//check score
	if (mScore > mScoreToReach)
	{
		mLevelNum++;
		
		if (mLevelNum > mHighestLevel)
			mHighestLevel = mLevelNum;
		
		mScoreToReach += 10000; 

		mExpToNextLevel -= 100 * (int)mBlockSpeedMultiplier;
		if (mExpToNextLevel <= 0)
		{
			mPlayerLevelNum++;
			mExpToNextLevel = 300 * mPlayerLevelNum;
		}
	}
}

void Game::render()
{
	mSceneManager.draw(mSystem.getGraphicsSystem());

	if (mSceneManager.getCurrentScene() == SC_GAME || mSceneManager.getCurrentScene() == SC_PAUSE)
	{
		mBlockObjectVector[mCurrBlock].draw(mSystem.getGraphicsSystem());
		mBlockObjectVector[mNextBlock].drawNext(mSystem.getGraphicsSystem());
		mBlockPile.draw(mSystem.getGraphicsSystem());
	}

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
			mSceneManager.playSfx("buttonselect");
			if (mSceneManager.getGuiManager(SC_MAIN)->getButtonEventPressed(NEW_GAME))
			{ 
				//Reload blockTypes to reset the board
				initBlockTypes();
				mLevelNum = 1;
				mScore = 0;
				mScoreToReach = 1000;
				mSceneManager.setCurrentScene(SC_GAME);
			}
			else if (mSceneManager.getGuiManager(SC_MAIN)->getButtonEventPressed(QUIT))
				mIsRunning = false;
			else if (mSceneManager.getGuiManager(SC_MAIN)->getButtonEventPressed(LOAD_GAME))
				loadLastSave();
			else if (mSceneManager.getGuiManager(SC_MAIN)->getButtonEventPressed(OPTIONS))
				mSceneManager.setCurrentScene(SC_OPTIONS);
			else if (mSceneManager.getGuiManager(SC_MAIN)->getButtonEventPressed(CREDITS))
				mSceneManager.setCurrentScene(SC_CREDITS);

		}
		else if (mSceneManager.getCurrentScene() == SC_OPTIONS)
		{
			mSceneManager.playSfx("buttonselect");
			if (mSceneManager.getGuiManager(SC_OPTIONS)->getButtonEventPressed(RETURN_MAIN))
			{
				//set new block speed
				for (unsigned int i = 0; i < mBlockObjectVector.size(); i++)
				{
					mBlockObjectVector[i].setSpeed(mBlockSpeed);
				}

				mSceneManager.setCurrentScene(SC_MAIN);

			}
			else if (mSceneManager.getGuiManager(SC_OPTIONS)->getButtonEventPressed(LANG_CHANGE))
			{
				if (mLocalization.getLanguage() == ENGLISH)
				{
					mLocalization.setLangauge(SPANISH);
				}
				else if (mLocalization.getLanguage() == SPANISH)
				{
					mLocalization.setLangauge(SWAHILI);
				}
				else
				{
					mLocalization.setLangauge(ENGLISH);
				}
			}
			else if (mSceneManager.getGuiManager(SC_OPTIONS)->getButtonEventPressed(CHANGE_DIFFICULTY))
			{
				if (mBlockSpeedMultiplier == 1)
				{
					mBlockSpeedMultiplier++;
					mBlockSpeed = mSpeed2;
				}
				else if (mBlockSpeedMultiplier == 2)
				{
					mBlockSpeedMultiplier++;
					mBlockSpeed = mSpeed3;
				}
				else if (mBlockSpeedMultiplier == 3)
				{
					mBlockSpeedMultiplier = 1;
					mBlockSpeed = 1000;
				}
			}
			else if (mSceneManager.getGuiManager(SC_OPTIONS)->getButtonEventPressed(CHANGE_AUDIO))
			{
				if (mAudioCounter == 1)
				{
					mAudioCounter = 2;
					mSceneManager.stopCurrentMusic();
					mSceneManager.setCurrentMusic("tetrisdub"); //2
 				}
				else if (mAudioCounter == 2)
				{
					if (mPlayerLevelNum > 2)
					{
						mAudioCounter = 3;
						mSceneManager.stopCurrentMusic();
						mSceneManager.setCurrentMusic("mii"); //3
					}
					else
					{
						mAudioCounter = 0;
						mSceneManager.stopCurrentMusic();
						mSceneManager.setCurrentMusic("");
					}
				}
				else if (mAudioCounter == 3)
				{
					if (mPlayerLevelNum > 4)
					{
						mAudioCounter = 4;
						mSceneManager.stopCurrentMusic();
						mSceneManager.setCurrentMusic("rasputin"); //4
					}
					else
					{
						mAudioCounter = 0;
						mSceneManager.stopCurrentMusic();
						mSceneManager.setCurrentMusic("");
					}
				}
				else if (mAudioCounter == 4)
				{
					mAudioCounter = 0;
					mSceneManager.stopCurrentMusic(); //0
					mSceneManager.setCurrentMusic("");
				}
				else if (mAudioCounter == 0) //no music
				{
					mAudioCounter = 1;
					mSceneManager.setCurrentMusic("tetris"); //1
				}
			}
			if (mSceneManager.getGuiManager(SC_OPTIONS)->getButtonEventPressed(STATS))
			{
				mSceneManager.setCurrentScene(SC_STATS);
			}
		}
		else if (mSceneManager.getCurrentScene() == SC_PAUSE)
		{
			mSceneManager.playSfx("buttonselect");
			if (mSceneManager.getGuiManager(SC_PAUSE)->getButtonEventPressed(RESUME))
			{
				mSceneManager.setCurrentScene(SC_GAME);
			}
			else if (mSceneManager.getGuiManager(SC_PAUSE)->getButtonEventPressed(RETURN_MAIN))
			{
				saveGame();
				mSceneManager.setCurrentScene(SC_MAIN);
			}
		}
		else if (mSceneManager.getCurrentScene() == SC_LOSE)
		{
			mSceneManager.playSfx("buttonselect");

			if (mSceneManager.getGuiManager(SC_LOSE)->getButtonEventPressed(NEW_GAME))
			{
				initBlockTypes();
				mLevelNum = 1;
				mScore = 0;
				mScoreToReach = 1000;
				mSceneManager.setCurrentScene(SC_GAME);
			}
			if (mSceneManager.getGuiManager(SC_LOSE)->getButtonEventPressed(QUIT_LOSE))
			{
				mSceneManager.setCurrentScene(SC_MAIN);
			}
		}
		else if (mSceneManager.getCurrentScene() == SC_CREDITS)
		{
			if (mSceneManager.getGuiManager(SC_CREDITS)->getButtonEventPressed(RETURN_MAIN_CREDITS))
			{
				mSceneManager.setCurrentScene(SC_MAIN);
			}
		}
		else if (mSceneManager.getCurrentScene() == SC_STATS)
		{
			if (mSceneManager.getGuiManager(SC_STATS)->getButtonEventPressed(RETURN_OPTIONS))
			{
				mSceneManager.setCurrentScene(SC_OPTIONS);
			}
		}
		break;

	case MOVE_DOWN:
		cout << endl;
		if (mSceneManager.getCurrentScene() == SC_GAME && mCanMoveBlockY == true)
		{
			mBlockObjectVector[mCurrBlock].moveBlockY();
			mCanMoveBlockY = false;
		}
		else if (mSceneManager.getCurrentScene() == SC_MAIN)
		{
			mSceneManager.getGuiManager(SC_MAIN)->buttonPressed = true;//select right button
			mSceneManager.getGuiManager(SC_MAIN)->left = false;
			mSceneManager.playSfx("buttonmove");

		}
		else if (mSceneManager.getCurrentScene() == SC_OPTIONS)
		{
			mSceneManager.getGuiManager(SC_OPTIONS)->buttonPressed = true;//select left button
			mSceneManager.getGuiManager(SC_OPTIONS)->left = false;
			mSceneManager.playSfx("buttonmove");

		}
		else if (mSceneManager.getCurrentScene() == SC_PAUSE)
		{
			mSceneManager.getGuiManager(SC_PAUSE)->buttonPressed = true;//select left button
			mSceneManager.getGuiManager(SC_PAUSE)->left = false;
			mSceneManager.playSfx("buttonmove");

		}
		else if (mSceneManager.getCurrentScene() == SC_LOSE)
		{
			mSceneManager.getGuiManager(SC_LOSE)->buttonPressed = true;//select left button
			mSceneManager.getGuiManager(SC_LOSE)->left = false;
			mSceneManager.playSfx("buttonmove");

		}

		break;

	case MOVE_UP:
		cout << endl;
		if (mSceneManager.getCurrentScene() == SC_GAME)
		{
			mBlockObjectVector[mCurrBlock].rotateBlock(mBlockPile);
		}
		else if (mSceneManager.getCurrentScene() == SC_MAIN)
		{
			mSceneManager.getGuiManager(SC_MAIN)->buttonPressed = true;//select left button
			mSceneManager.getGuiManager(SC_MAIN)->left = true;

			mSceneManager.playSfx("buttonmove");
		}
		else if (mSceneManager.getCurrentScene() == SC_OPTIONS)
		{
			mSceneManager.getGuiManager(SC_OPTIONS)->buttonPressed = true;//select left button
			mSceneManager.getGuiManager(SC_OPTIONS)->left = true;
			mSceneManager.playSfx("buttonmove");
		}
		else if (mSceneManager.getCurrentScene() == SC_PAUSE)
		{
			mSceneManager.getGuiManager(SC_PAUSE)->buttonPressed = true;//select left button
			mSceneManager.getGuiManager(SC_PAUSE)->left = true;
			mSceneManager.playSfx("buttonmove");
		}
		else if (mSceneManager.getCurrentScene() == SC_LOSE)
		{
			mSceneManager.getGuiManager(SC_LOSE)->buttonPressed = true;//select left button
			mSceneManager.getGuiManager(SC_LOSE)->left = true;
			mSceneManager.playSfx("buttonmove");
		}
		break;


	case MOVE_LEFT:
		cout << endl;
		if (mSceneManager.getCurrentScene() == SC_GAME)
		{
			mBlockObjectVector[mCurrBlock].moveBlockX(-1, mBlockPile);
		}
		break;

	case MOVE_RIGHT:
		cout << endl;
		if (mSceneManager.getCurrentScene() == SC_GAME)
		{
			mBlockObjectVector[mCurrBlock].moveBlockX(1, mBlockPile);
		}
		break;
	}
}
