#ifndef ENTITY_H
#define ENTITY_H

#include "GraphicsSystem.h"
#include "Animation.h"
#include "Collider.h"

/*
All object headers include Trackable in order
to use time and memory managment tool
*/
class Entity : public Trackable
{

	private:
		
	protected:
		double mXLoc, mYLoc;	//world-space location
		float mXScale, mYScale;
		double mRotation;
		Animation mAnim;
		bool mIsVisible, mShouldAnimate, mHasCollider;

		Collider mThisCollider;
		
		//collder vars
		int mLeftA, mLeftB, mRightA, mRightB;
		int mTopA, mTopB, mBottomA, mBottomB;

	public:
		Entity();

		~Entity();

		//Call animation update
		void update(double timeElapsed);
		
		bool checkCollision(Collider a, Collider b);

		//Draw animated sprite
		void draw(GraphicsSystem *graphicsSystem, int camX, int camY);
		void draw(GraphicsSystem *graphicsSystem);
		
		//Give mAnim a new sprite
		void shouldAnimate(bool pause);

		void setRotation(double roationAngle);

		void setAnimation(Animation targetAnim);
		Animation* getAnimation();

		//Toggle object visibility in-game
		void setVisible(bool setVisibilty);
		bool isVisible();

		//Location fucntions
		void setLoc(int x, int y);
		double getX();
		double getY();

		//Scale functions
		void setScale(float x, float y);
		float getXScale();
		float getYScale();
		double getRotation();

		void setCollider(string colliderNameTag);
		Collider* getCollider();
};

#endif // !ENTITY_H
