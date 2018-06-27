#ifndef SCENE_H
#define SCENE_H

#include "GUIManager.h"


enum SceneType
{
	SC_UNDEF,
	SC_MAIN,
	SC_GAME,
	SC_PAUSE,
	SC_LOSE,
	SC_OPTIONS,
	SC_CREDITS,
	SC_STATS
};

class Scene : public Trackable
{
private:
	GUIManager *mGuiManager;
	Sprite *mSceneBG;

	bool mIsActiveScene;
	SceneType mSceneType;

public:
	Scene();
	~Scene();

	void initScene(SceneType type, bool isActive, GUIManager *gManager, Sprite *spr);

	void update(double timeElapsed);
	void draw(GraphicsSystem *graphicsSystem);

	bool isSceneActive();
	void setSceneActive(bool active);

	SceneType getSceneType();
	void setSceneType(SceneType sceneType);

	GUIManager* getGuiManager();
};

#endif // !SCENE_H
