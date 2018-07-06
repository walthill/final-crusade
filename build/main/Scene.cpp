#include "Scene.h"

Scene::Scene()
{

}

Scene::~Scene()
{
}


void Scene::initScene(SceneType type, bool isActive, GUIManager *gManager, Sprite *spr)
{
	mGuiManager = gManager;
	mSceneBG = spr;

	mSceneType = type;
	mIsActiveScene = isActive;
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
		case SC_MAIN:
			graphicsSystem->draw(0, 0, *mSceneBG, 1.0, 1.0);
			mGuiManager->draw(graphicsSystem, "mainmenu");
			break;
		case SC_GAME:
			graphicsSystem->viewDraw(0, 0, *mSceneBG);
			mGuiManager->draw(graphicsSystem, "game");
			break;
		case SC_OPTIONS:
			graphicsSystem->draw(0, 0, *mSceneBG, 1.0, 1.0);
			mGuiManager->draw(graphicsSystem, "options");
			break;
		case SC_PAUSE:
			graphicsSystem->draw(0, 0, *mSceneBG, 1.0, 1.0);
			mGuiManager->draw(graphicsSystem, "pause");
			break;
		case SC_CREDITS:
			graphicsSystem->draw(0, 0, *mSceneBG, 1.0, 1.0);
			mGuiManager->draw(graphicsSystem, "credits");
			break;
		case SC_STATS:
			graphicsSystem->draw(0, 0, *mSceneBG, 1.0, 1.0);
			mGuiManager->draw(graphicsSystem, "stats");
		case SC_LOSE:
			graphicsSystem->draw(0, 0, *mSceneBG, 1.0, 1.0);
			mGuiManager->draw(graphicsSystem, "lose");
			break;
		default:
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