#include "Scene.h"

Scene::Scene()
{
	mIsActiveScene = false;
}

Scene::~Scene()
{
}

void Scene::initScene(SceneType type, GUIManager *gManager, Sprite *spr)
{
	mGuiManager = gManager;
	mSceneBG = spr;

	mSceneType = type;
}

void Scene::update(double timeElapsed)
{
	mGuiManager->updateButtons(timeElapsed);
}

void Scene::draw(GraphicsSystem *graphicsSystem)
{

	switch (mSceneType)
	{
		case SC_UNDEF:
			break;
		case SC_GAME:
			graphicsSystem->viewDraw(0, 0, *mSceneBG);
			break;
		case SC_MAIN:
		case SC_OPTIONS:
		case SC_PAUSE:
		case SC_CREDITS:
		case SC_CONTROLS:
		case SC_STATS:
		case SC_WIN:
		case SC_LOSE:
			graphicsSystem->draw(0, 0, *mSceneBG, 1.0, 1.0);
			break;
		default:
			assert(!mIsActiveScene); //add scene enum to draw switch statement
			break;
	}
}

bool Scene::isSceneActive()
{
	return mIsActiveScene;
}

void Scene::setSceneActive(bool active)
{
	mIsActiveScene = active;
}

SceneType Scene::getSceneType()
{
	return mSceneType;
}

void Scene::setSceneType(SceneType sceneType)
{
	mSceneType = sceneType;
}

GUIManager* Scene::getGuiManager()
{
	return mGuiManager;
}