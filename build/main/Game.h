#ifndef GAME_H
#define GAME_H

#include <iomanip>
#include "Timer.h"
#include "MemoryTracker.h"
#include "PerformanceTracker.h"

#include "InputTranslator.h"
#include "Player.h"
#include "simpleini-4.17\SimpleIni.h"
#include "GraphicsBufferManager.h"
#include "SceneManager.h"
#include "View.h"
#include "BulletPool.h"
#include "RoninManager.h"

/*
File I/O performed using brofield's SimpleIni
Link: https://github.com/brofield/simpleini
*/

/*
Object headers include Trackable in order
to use time and memory managment tool
*/

typedef string ColliderTag;
typedef string AssetString;
//typedef string UITag;

class Game : EventListener
{
	private:
		static Game* mGameInstance; 
		
		const string GAME_TITLE = "Final Crusade";

		unsigned int mEventType;

		System mSystem; //Gives access to window and graphics
		GraphicsBufferManager mBufferManager; 
		InputTranslator mInputTranslator;

		View mGameView;
		
		//each scene has a manager
		GUIManager mGuiManagerMain, mGuiManagerGame, mGuiManagerOptions, mGuiManagerStats,
				   mGuiManagerPause, mGuiManagerLose, mGuiManagerCredits;

		Scene mMainMenu, mGame, mOptions, mPauseScene, mLoseScene, mStatsScene, mCreditScene;
		SceneManager mSceneManager;

		Gui mFpscounter;

		Language mLocalization;

		//Assets
		GraphicsBuffer mMenuBuffer;
		GraphicsBuffer mSpaceBlue, mLoadingScreen;
		GraphicsBuffer mCredit;
		vector<GraphicsBuffer> mBlockBufferVector;
		GraphicsBuffer mStartButton;
		GraphicsBuffer mPlayerBuffer, mBulletBuffer, mRoninBuffer;

		Sprite mBackgroundSprite, mMenuSprite, mCreditBG, mLoadingSprite;
		Animation mPlayerAnim, mBulletAnim, mRoninAnim;

		RoninManager mEnemyManager;

		int mNumEnemies, mNumRonin;

		Ronin mRonin;
		Player mPlayer;
		BulletPool mBulletManager;

		const ColliderTag PLAYER_COL_TAG = "player";
		const ColliderTag RONIN_COL_TAG = "ronin";
		const ColliderTag BULLET_COL_TAG = "stdBullet";

		//External assets
		const AssetString mLOCAL_ASSET_PATH = "assets\\";
		const AssetString mLOADING_IMG = "bigspace.png";
		const AssetString mCOUR_FONT = "cour.ttf";
		const AssetString mINI_FILE = "data.ini";
		const AssetString mSPACE_PURPLE = "spacepurple.bmp";
		const AssetString mPLAYER_ASSET = "phplayer.png";
		const AssetString mRONIN_ASSET = "phronin.png";
		const AssetString mBULLET_ASSET = "bullet.png";


		//Buffer tags
		const string mPLAYER_ID = "player";
		const string mRONIN_ID = "ronin";
		const string mBULLET_ID = "bullet";
		const string mMENU_ID = "spacebase";
		const string mSPACE_ID = "spaceblue";
		const string mCREDIT_ID, mLOAD_ID = "loading";

		string mEnemyManTag = "e";

		//UI tags
		const string mGEN_TAG = "general";
		const string mMAINMENU_TAG = "mainmenu";
		const string mOPTIONS_TAG = "options";
		const string mPAUSE_TAG = "pause";
		const string mL_TAG = "lose";
		const string mCREDITS_TAG = "credits";
		const string mSTATS_TAG = "stats";

		bool mStartGame;
		bool mIsRunning;
		
		int mSpriteSize;
		int mGridW, mGridH;

		int mDisplayWidth, mDisplayHeight, mLevelWidth, mLevelHeight;

		Font *mFont, *mGameUI;
		const int mFONT_SIZE = 25;
		const int mUI_SIZE = 20;
		Color mWhiteText;

		int mouseX, mouseY;

		int bulletSpawnX, bulletSpawnY;


		Game();
		~Game();

		//Performance tracker & titles
		const string mINIT_TRACKER_NAME = "init";
		const string mDRAW_TRACKER_NAME = "draw";

		const double mFRAME_TIME_60FPS = 16.7;
		int mCapNum=0;
		const string mScreencapFilename = "capture";
		bool takeScreenshot;
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

		//Explicit destructor
		void cleanupGame();

		//Calls input, update, and render
		bool runGameLoop();

		//Update game objects
		void update(double timeElapsed);

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
