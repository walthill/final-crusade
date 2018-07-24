#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"


class Player : public Entity
{
	private:
		//rotation math vars
		double dX, dY, angle;
		const double PI = 3.1416, DEGREE_CONVERSION_VAL = 180.0000;

		const int ROT_ALIGNMENT = 8;

		//movement vars
		bool mLeft, mRight, mUp, mDown;
		int mPlayerFrameSpeed, mVelocity; 
		int mXBound, mYBound;
		int mScreenXSize, mScreenYSize;//TODO: data driven here

		bool lastLife, hasRecovered = false;
		double dtTime =0;
		int tempInvulnerable = 5000;

		bool collisionDetected = false;

	//	Collider mCollider;

	public:
		Player();
		~Player();

		void init(int xBounds, int yBounds, int screenXSize, int screenYSize);

		void update(double timeElapsed, vector<Collider*> colliderList, int mouseX, int mouseY, int camX, int camY);
		//void draw(GraphicsSystem *graphicsSystem, int camX, int camY);

		void checkBounds();
		void checkForEnemyCollision(vector<Collider*> colliderList, double timeElapsed);
//		bool checkCollision(Collider a, Collider b);

		void move(int mouseX, int mouseY, int camX, int camY);
		void rotate(int mouseX, int mouseY, int camX, int camY);

		void setLeft(bool isMovingLeft);
		void setRight(bool isMovingRight);
		void setDown(bool isMovingDown);
		void setUp(bool isMovingUp);

		int getWidth();
		int getHeight();

};

#endif //!PLAYER_H