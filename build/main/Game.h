#ifndef GAME_H
#define GAME_H

#include <random>
#include "MemoryTracker.h"
#include "PerformanceTracker.h"

#include "InputTranslator.h"
#include "simpleini-4.17\SimpleIni.h"
#include "View.h"
#include "GraphicsBufferManager.h"
#include "SceneManager.h"
#include "FragmentManager.h"
#include "BulletPool.h"
#include "RoninManager.h"
#include "MountainManager.h"
#include "HiveManager.h"

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
		const string mGAME_TITLE = "Final Crusade";
		
		System mSystem; //Gives access to window and graphics
		GraphicsBufferManager mBufferManager; 
		InputTranslator mInputTranslator;
		Language mLocalization;

		View mGameView;
		
		//each scene has a manager
		GUIManager mGuiManagerMain, mGuiManagerGame, mGuiManagerOptions, mGuiManagerStats,
				   mGuiManagerPause, mGuiManagerLose, mGuiManagerWin, mGuiManagerCredits;

		Scene mMainMenuScene, mGameScene, mOptionsScene, mPauseScene, 
			  mLoseScene, mWinScene, mStatsScene, mCreditScene;
		
		SceneManager mSceneManager;
		

		Gui mFpscounter, mLoseRetry, mLoseQuit, mLoseText,
			mWinTitle, mWinPlayAgain, mWinQuit,
			mMainStart, mMainQuit, mMainTitle,
			mGameScore, mGameCombo, mGameTime;

		//Visual Assets
		GraphicsBuffer mButtonBuffer; // ui button
		GraphicsBuffer mMenuBuffer, mLoadingScreen, mGameScreenBuffer, mLoseScreenBuffer, 
					   mWinScreenBuffer; //backgrounds
		GraphicsBuffer mPlayerBuffer, mBulletBuffer, mEnemyBulletBuffer, mFragmentBuffer,
					   mRoninBuffer, mMountainBuffer, mHiveBuffer;

		Sprite mMenuSprite, mGameScreenSprite, mLoadingSprite, mLoseScreenSprite, mWinScreenSprite;
		Animation mPlayerAnim, mBulletAnim, mEnemyBulletAnim, mFragmentAnim, mHiveAnim, 
				  mRoninAnim, mMountainAnim;

		const int mFONT_SIZE = 25;
		const int mUI_SIZE = 20;
		const int mUI_TXT_SIZE = 12;
		const int mBUTTON_SPRSHEET_ROWS = 1, mBUTTON_SPRSHEET_COLS = 2;
		Font *mFont, *mGameUI;
		Color mWhiteText;

		//Managers
		RoninManager mRoninManager;
		MountainManager mMountainManager;
		HiveManager mHiveManager;
		BulletPool mBulletManager;
		BulletPool mEnemyBulletManager;
		FragmentManager mFragmentList;

		vector<Collider*> mColliderCollection;

		//Game Entities
		//Ronin mRonin;
		Player mPlayer;

		unsigned int mEventType;
		bool mStartGame;

		int mSpriteSize;
		int mGridW, mGridH;

		int mNumEnemies, mNumRonin, mNumMountain, mNumHive;

		int mouseX, mouseY;
		int camX, camY;
		int mPlayerSpriteSize, mRoninSpriteSize, mEnemyBulletSpriteSize, mFragmentSpriteSize,
			mMountainSpriteSize, mBulletSpriteSize, mHiveSpriteSize;
		int mRoninScoreValue, mHiveScoreValue, mMountainScoreValue;
		float bulletSpawnX, bulletSpawnY;

		//gameplay variables
		Timer *survivalTimer;
		double dtTime;	int sec = 0, millisec, min = 0;
		const int mCOMBO_WINDOW = 2500;
		const int mCOMBO_NUM_TO_REGEN = 3;

		//Enemy Manager Tags
		string mRoninManTag = "r";
		string mMountainManTag = "m";
		string mFragmentManTag = "f";
		string mHiveManTag = "h";

		//Collider tags
		const ColliderTag PLAYER_COL_TAG = "player";
		const ColliderTag BULLET_COL_TAG = "stdBullet";
		const ColliderTag ENEMEY_BULLET_COL_TAG = "enemyBullet";

		//External assets
		const AssetString mLOCAL_ASSET_PATH = "assets\\";
		const AssetString mINI_FILE = "data.ini";

		AssetString mButtonAsset, mMainBgAsset, mLoseBgAsset, mGameBgAsset, mWinBgAsset,
		mLoadBgAsset, mFontAsset, mPlayerAsset, mEnemyBulletAsset, mRoninAsset, mBulletAsset,
		mMountainAsset, mFragmentAsset, mHiveAsset;

		//GraphicsBuffer tags
		const BufferTag mPLAYER_ID = "player";
		const BufferTag mRONIN_ID = "ronin";
		const BufferTag mMOUNTAIN_ID = "mountain";
		const BufferTag mLOSE_SCREEN_ID = "lose";
		const BufferTag mWIN_SCREEN_ID = "win";
		const BufferTag mBULLET_ID = "bullet";
		const BufferTag mENEMY_BULLET_ID = "ebullet";
		const BufferTag mMAINMENU_BUFFER_ID = "spaceblue";
		const BufferTag mSPACE_ID = "spacebase";
		const BufferTag mCREDIT_ID, mLOAD_ID = "loading";
		const BufferTag mGAME_ID = "game";
		const BufferTag mFRAGMENT_ID = "fragmnt";
		const BufferTag mHIVE_ID = "hive";

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
		
		double mFPS;
		bool mIsRunning;
	public:

		//gloabl vars
		int _DisplayWidth, _DisplayHeight, _LevelWidth, _LevelHeight;
		int _Score, _ComboCount, _TimeSurvived, _NumFragments, _FragmentsToCollect;
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
		void initEnemies();
		void initAnimations();
		void initPlayer();
		void loadLocalization();
		void initAudio();
		void loadBackgroundsAndBuffers();
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
