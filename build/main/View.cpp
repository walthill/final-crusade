#include "View.h"

View::View()
{

}

View::View(Player *player)
{
	mPlayerRef = player;
	centerView();
}


View::~View()
{
}

void View::update(double timeElapsed)
{
	if(mPlayerRef != nullptr)
		checkBounds();
}


void View::render(GraphicsSystem *graphicsSystem)
{

}

void View::checkBounds()
{
	//Keep the camera in bounds
	if (camera.getX() < 0)
	{
		camera.setX(0);
	}
	if (camera.getY() < 0)
	{
		camera.setY(0);
	}
	if (camera.getX() > Game::getInstance()->_LevelWidth - camera.getW())
	{
		camera.setX(Game::getInstance()->_LevelWidth - camera.getW());
	}
	if (camera.getY() > Game::getInstance()->_LevelHeight - camera.getH())
	{
		camera.setY(Game::getInstance()->_LevelHeight - camera.getH());
	}
}

void View::centerView()
{
	//center camera
	camera.setX((mPlayerRef->getX() + mPlayerRef->getWidth() / 2) - Game::getInstance()->_DisplayWidth / 2);
	camera.setY((mPlayerRef->getY() + mPlayerRef->getHeight() / 2) - Game::getInstance()->_DisplayHeight / 2);
}

