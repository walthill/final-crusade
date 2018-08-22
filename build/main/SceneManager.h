#ifndef SCENE_MAN_H
#define SCENE_MAN_H

#include <map>
#include <iomanip>
#include <string>
#include "Scene.h"
#include "Sound.h"
#include "Language.h"

typedef string SceneID;
typedef string SoundID;


class SceneManager
{
	private:
		//text id's
		const string LVL = "level";
		const string SCORE = "score";
		const string FPS_COUNT = "mainmenu4";

		string timeStr;

		map<SceneID, Scene*> mSceneCollection;
		map<SoundID, Sound*> mSoundList;

		SoundID mCurrentSound;
		SceneType mCurrentScene;

		Language *mLocalization;
	public:
		SceneManager();
		~SceneManager();

		void initLanguages(Language *localization);

		//Explicit destructor
		void clearManager();

		//Take in values for UI info
		void update(double timeElapsed, int &combo, int &score, int &fragmentsCollectedPercentage, int const &minutes, int const &seconds, double &fps);
		void update(double timeElapsed, int &combo, int &score, int &fragmentsCollectedPercentage, int const &minutes, int const &seconds);

		void draw(GraphicsSystem *graphicsSystem);
		void drawGUI(GraphicsSystem *graphicsSystem);


		void moveCursorDown(SceneType scene, SoundID buttonMoveSound = "");
		void moveCursorUp(SceneType scene, SoundID buttonMoveSound = "");

		//Add buffer to collection
		void addScene(SceneID key, Scene *objToAdd);
		void addAudio(SoundID key, Sound *audioToAdd);

		void playSound(SoundID key);
		void stopSound(SoundID key);

		void playCurrentMusic();
		void stopCurrentMusic();

		void playSfx(SoundID id);
		void setCurrentMusic(SoundID id);

		//Remove from collection by object or key
		void removeScene(SceneID key);
		void removeScene(Scene *objToRemove);

		SceneType getCurrentScene();
		void setCurrentScene(SceneType type);

		Scene* getScene(SceneType type);

		GUIManager* getGuiManager(SceneType type);


};

#endif // !SCENE_MAN_H
