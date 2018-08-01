#ifndef GAME_H
#define GAME_H

#include <random>
#include <chrono>
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
#include "MountainManager.h"

/*
File I/O performed using brofield's SimpleIni
Link: https://github.com/brofield/simpleini
*/

/*
Object headers include Trackable in order
to use time and memory managment tool
*/

typedef string ColliderTag, BufferTag, AssetString;

//typedef string UITag;

class Game : EventListener
{
	private:
		static Game* mGameInstance; 
		const string GAME_TITLE = "Final Crusade";
		
		System mSystem; //Gives access to window and graphics
		GraphicsBufferManager mBufferManager; 
		InputTranslator mInputTranslator;
		Language mLocalization;

		View mGameView;
		
		//each scene has a manager
		GUIManager mGuiManagerMain, mGuiManagerGame, mGuiManagerOptions, mGuiManagerStats,
				   mGuiManagerPause, mGuiManagerLose, mGuiManagerCredits;

		Scene mMainMenuScene, mGameScene, mOptionsScene, mPauseScene, mLoseScene, mStatsScene, mCreditScene;
		SceneManager mSceneManager;

		Gui mFpscounter, 
			mMainStart, mMainQuit, mMainTitle,
			mGameScore, mGameCombo, mGameTime;

		//Visual Assets
		GraphicsBuffer mButtonBuffer; // ui button
		GraphicsBuffer mMenuBuffer, mLoadingScreen;
		GraphicsBuffer mPlayerBuffer, mBulletBuffer, mRoninBuffer, mMountainBuffer;

		Sprite mMenuSprite, mLoadingSprite;
		Animation mPlayerAnim, mBulletAnim, mRoninAnim, mMountainAnim;

		const int mFONT_SIZE = 25;
		const int mUI_SIZE = 20;
		const int mUI_TXT_SIZE = 12;
		const int mBUTTON_SPRSHEET_ROWS = 1, mBUTTON_SPRSHEET_COLS = 2;
		Font *mFont, *mGameUI;
		Color mWhiteText;


		//Managers
		RoninManager mRoninManager;
		MountainManager mMountainManager;
		BulletPool mBulletManager;
		vector<Collider*> mColliderCollection;

		//Game Entities
		Ronin mRonin;
		Player mPlayer;

		unsigned int mEventType;
		bool mStartGame;

		int mSpriteSize;
		int mGridW, mGridH;

		int mDisplayWidth, mDisplayHeight, mLevelWidth, mLevelHeight;

		int mNumEnemies, mNumRonin, mNumMountain;

		int mouseX, mouseY;
		int camX, camY;
		int mPlayerSpriteSize, mRoninSpriteSize, mMountainSpriteSize, mBulletSpriteSize;
		int bulletSpawnX, bulletSpawnY;


		//Enemy Manager Tags
		string mRoninManTag = "r";
		string mMountainManTag = "m";

		//Collider tags
		const ColliderTag PLAYER_COL_TAG = "player";
		const ColliderTag RONIN_COL_TAG = "ronin";
		const ColliderTag BULLET_COL_TAG = "stdBullet";
		const ColliderTag MOUNTAIN_COL_TAG = "mountain";

		//External assets
		const AssetString mLOCAL_ASSET_PATH = "assets\\";
		const AssetString mBUTTON_ASSET = "button.bmp";
		const AssetString mMAINMENU_BG_ASSET = "spaceblue.png";
		const AssetString mLOADING_IMG = "bigspace.png";
		const AssetString mCOUR_FONT = "cour.ttf";
		const AssetString mINI_FILE = "data.ini";
		const AssetString mSPACE_PURPLE = "spacepurple.bmp";
		const AssetString mPLAYER_ASSET = "phplayer.png";
		const AssetString mRONIN_ASSET = "phronin.png";
		const AssetString mBULLET_ASSET = "bullet.png";
		const AssetString mMOUNTAIN_ASSET = "phmountain.png";

		//GraphicsBuffer tags
		const BufferTag mPLAYER_ID = "player";
		const BufferTag mRONIN_ID = "ronin";
		const BufferTag mMOUNTAIN_ID = "mountain";
		const BufferTag mBULLET_ID = "bullet";
		const BufferTag mMAINMENU_BUFFER_ID = "spaceblue";
		const BufferTag mSPACE_ID = "spacebase";
		const BufferTag mCREDIT_ID, mLOAD_ID = "loading";

		//UI tags
		const string mGEN_TAG = "general";
		const string mMAINMENU_TAG = "mainmenu";
		const string mOPTIONS_TAG = "options";
		const string mPAUSE_TAG = "pause";
		const string mL_TAG = "lose";
		const string mCREDITS_TAG = "credits";
		const string mSTATS_TAG = "stats";
	
		Game();
		~Game();

		//Performance tracker & titles
		const string mINIT_TRACKER_NAME = "init";
		const string mDRAW_TRACKER_NAME = "draw";
		const double mFRAME_TIME_60FPS = 16.7;
		
		//screenshot var
		bool takeScreenshot;
		
		Timer *survivalTimer;

		double mFPS;
		bool mIsRunning;
		int dtTime;	int sec = 0, millisec, min = 0;
		const int mCOMBO_WINDOW = 2500;
		const int mCOMBO_NUM_TO_REGEN = 3;

		
	public:

		//gloabl vars
		int _Score, _ComboCount, _TimeSurvived;
		bool _CanCombo;

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
		
		void tickSurvivalTimer();
		void comboUpdate(double timeElapsed);
		void endCombo();

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
