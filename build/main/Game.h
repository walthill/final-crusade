#ifndef GAME_H
#define GAME_H

#include <iomanip>
#include "Timer.h"
#include "MemoryTracker.h"
#include "PerformanceTracker.h"

#include "InputTranslator.h"

#include "simpleini-4.17\SimpleIni.h"
#include "GraphicsBufferManager.h"
#include "SceneManager.h"

#include "BlockObject.h"


/*
File I/O performed using brofield's SimpleIni
Link: https://github.com/brofield/simpleini
*/


/*
Object headers include Trackable in order
to use time and memory managment tool
*/

class Game : EventListener
{
	private:
		static Game* mGameInstance; 
		
		System mSystem; //Gives access to window and graphics
		GraphicsBufferManager mBufferManager; 
		InputTranslator mInputTranslator;
		
		//each scene has a manager
		GUIManager mGuiManagerMain, mGuiManagerGame, mGuiManagerOptions, mGuiManagerStats,
				   mGuiManagerPause, mGuiManagerLose, mGuiManagerCredits;
		
		vector<BlockObject> mBlockObjectVector;
		vector<Vector2D> mBlockBitPositions;
		BlockPile mBlockPile;

		Scene mMainMenu, mGame, mOptions, mPauseScene, mLoseScene, mStatsScene, mCreditScene;
		SceneManager mSceneManager;

		Language mLocalization;

		Sound mTest, mButtonMove, mButtonSelect, mTetrisTheme, 
			  mTetrisThemeDub, mLineClearSound, mRasputinTheme,
				mBlockDropSound;

		Gui mMainTitle, mStart, mQuit, mPause, mLost,
			mScoreTxt, mLevel, mNext, mPauseNext, mFpscounter, mScoreToWin,
			mPauseScore, mPauseLevel, mPauseScoreToWin,
			mPlayerLevel, mExpToNextLevelTxt, mQuitLoseButton,
			mButton, mButton2, mButton3, mButton4, mCreditsButton,
			mLanguageButton, mOptionsTitle, mRetryButton, mStatsButton,
			mReturnToMain, mAudio, mDifficulty, mReturnFromCreditsButton,
			mResumeButton, mReturnButton, mOptionsGame, mReturnFromStatsButton,
			mTotalScoreTxt, mTotalClearsTxt, mHighestLevelTxt, mDevelopersTxt,
			mRasputinArtistTxt, mTetrisArtistTxt, mTetrisTrapArtistTxt, mTetrisImageTxt, mWiiThemeTxt;

		int mPlayerLevelNum = 1, mExpToNextLevel = 100, mClearPoints;
		int mLifetimeClears, mLifetimeScore, mHighestLevel;

		GraphicsBuffer mMenuBuffer;
		GraphicsBuffer mSpaceBlue, mLoadingScreen;
		GraphicsBuffer mCredit;
		vector<GraphicsBuffer> mBlockBufferVector;
		GraphicsBuffer mStartButton;

		Sprite mBackgroundSprite, mMenuSprite, mCreditBG, mLoadingSprite;
		Sprite mBlock1Sprite, mBlock2Sprite, mBlock3Sprite,
			   mBlock4Sprite, mBlock5Sprite, mBlock6Sprite,
			   mBlock7Sprite, mBlock8Sprite;
		Sprite mLoadBlock;

		//External assets
		
	//	#ifdef _DEBUG
		const string mLOCAL_ASSET_PATH = "assets\\";
	//	#else
		//const string mLOCAL_ASSET_PATH = "..\\assets\\";
	//	#endif


		const string mCOUR_FONT = "cour.ttf";
		const string mINI_FILE = "data.ini";
		const string mSPACE_BASE = "tetrisBG5.bmp";
		const string mSPACE_BLUE = "tetrisBG6.bmp";
		const string mCREDIT_BG = "tetrisBG7.bmp";
		const string mSPACE_PURPLE = "spacepurple.bmp";
		
		const string mWALLBLOCK = "wallBlock.bmp";		
		const string mL_BLOCK = "lBlock.bmp";
		const string mI_BLOCK = "iBlock.bmp";
		const string mZ_BLOCK = "zBlock.bmp";
		const string mS_BLOCK = "sBlock.bmp";
		const string mO_BLOCK = "oBlock.bmp";
		const string mT_BLOCK = "tBlock.bmp";
		const string mJ_BLOCK = "jBlock.bmp";
		
		const string mBUTTON = "button.bmp";
		const string mMUSIC_WII = "miimusic.wav";
		const string mMUSIC_TETRIS = "tetristheme.wav";
		const string mMUSIC_TETRISDUB = "tetrisdub.wav";
		const string mMUSIC_RASPUTIN = "rasputinMusic.wav";
		const string mFX_MOVE = "sof.wav";
		const string mFX_SELECT = "close.wav";
		const string mFX_LOCK = "blockLock.wav";
		const string mFX_CLEAR = "lineComplete.wav";

		//Buffer tags
		const string mMENU_ID = "spacebase";
		const string mSPACE_ID = "spaceblue";
		const string mCREDIT_ID, mLOAD_ID = "loading";
		const string mBLOCK_STR = "block";
		vector<string> mBlock_ID;

		//UI tags
		const string mSTD_UI_TAG = "general";
		const string mMAINMENU_TAG = "mainmenu";
		const string mOPTIONS_TAG = "options";
		const string mPAUSE_TAG = "pause";
		const string mL_TAG = "lose";
		const string mCREDITS_TAG = "credits";
		const string mSTATS_TAG = "stats";

		bool mStartGame;
		bool mIsRunning;
		bool mCanMoveBlockY = true;
		int mAudioCounter = 1;
	
		int mScore = 0; 
		string mScoreStr, mLevelStr;
		int mLevelNum = 1;
		int mCurrBlock = 2;
		int mNextBlock = 1;
		int mSpriteSize;

		int mNumBlocks;
		int mOffset, mGridW, mGridH;

		int mDisplayWidth, mDisplayHeight;

		Font *mFont, *mGameUI;
		const int mFONT_SIZE = 25;
		const int mUI_SIZE = 20;
		Color mWhiteText;

		const int mCLEAR_PTS = 1100;
		int mScoreToReach = 1000;
			
		float mBlockSpeedMultiplier = 1.0f;
		int mSpeed1, mSpeed2, mSpeed3, mBlockSpeed;

		unsigned int mEventType;
		
		Game();
		~Game();

		//Performance tracker & titles
		const string mINIT_TRACKER_NAME = "init";
		const string mDRAW_TRACKER_NAME = "draw";

		const double mFRAME_TIME_60FPS = 16.7;

		double mFPS;

	public:
		//Static class functions
		static Game* getInstance()
		{
			assert(mGameInstance != NULL);
			return mGameInstance;
		}


		static void initInstance()
		{
			mGameInstance = new Game;
		}


		static void cleanupInstance()
		{
			if (mGameInstance != NULL)
			{
				delete mGameInstance;
				mGameInstance = NULL;
			}	
		}

		//Open window and init graphics
		bool initGame();
		void displayLoadingScreen();

		void loadLocalization();
		void initAudio();
		void loadBackgrounds();
		void initUI();
		void loadScenes();

		void loadBlockImages();
		void initBlockTypes();

		//Explicit destructor
		void cleanupGame();

		//Calls input, update, and render
		bool runGameLoop();

		//Update game objects
		void update(double timeElapsed);
		void updateTetris();
		void updateScore();

		//Draw to the screen
		void render();
		
		void loadGameData();
		void installListeners();

		bool saveGame();
		void loadLastSave();

		//Handles input
		void handleEvent(const Event& theEvent);

};

#endif // !GAME_H
