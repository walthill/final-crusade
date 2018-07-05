#ifndef VIEW_H
#define VIEW_H

//#include "GraphicsSystem.h"
#include "Camera.h"
#include "Player.h"
#include "Game.h"

class View 
{
	private:
		bool isActive;
		//bool isFollowingPlayer;

		Camera camera; //TODO(high): camera initialization of width and height in View constructor
		Player *mPlayerRef;

	public:
		View();
		View(Player *player);
		~View();

		void update(double timeElapsed);
		void render(GraphicsSystem *graphicsSystem);

		void checkBounds();
		void centerView();

		void setActive(bool activate);
		bool isViewActive();
};

#endif // !VIEW_H
