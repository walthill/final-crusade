#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"


class Player : public Entity
{
	private:
		string playerHit;

		//rotation math vars
		float dX, dY, angle;
		const float PI = 3.1416f, DEGREE_CONVERSION_VAL = 180.0000f;
		const int ANGLE_OFFSET = 90;

		const int ROT_ALIGNMENT = 8;

		//movement vars
		bool mLeft, mRight, mUp, mDown;
		int mPlayerFrameSpeed, mVelocity; 
		int mXBound, mYBound;

		//collision vars
		bool lastLife, hasRecovered = false, collisionDetected = false,
			mControllerConnected;
		Collider b;

		int invulnerableTime;
		
		//gameplay vars
		const int FRAGMENTS_TO_COLLECT = 4;
		int numFragments;

	public:double dtTime;
		Player();
		~Player();

		void init(int xBounds, int yBound, string hitAudioId);

		void update(double timeElapsed, vector<Collider*> colliderList, int mouseX, int mouseY, int camX, int camY);
		//void draw(GraphicsSystem *graphicsSystem, int camX, int camY);

		void checkBounds();
		void checkForEnemyCollision(vector<Collider*> colliderList, double timeElapsed);
//		void checkForFragmentCollision(vector<Collider*> colliderList);

//		bool checkCollision(Collider a, Collider b);

		void move(int mouseX, int mouseY, int camX, int camY);
		void rotate(int mouseX, int mouseY, int camX, int camY);

		void setLeft(bool isMovingLeft);
		void setRight(bool isMovingRight);
		void setDown(bool isMovingDown);
		void setUp(bool isMovingUp);

		int getWidth();
		int getHeight();

		bool isLastLife();
		void setLastLife(bool isLastLife);
		void setCollisionDetected(bool collisionDetected);
		bool isCollisionDetected();

		void setControllerConnected(bool controllerConnected);
		bool isControllerConnected();
};

#endif //!PLAYER_H