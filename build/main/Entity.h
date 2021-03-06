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
		float mXLoc, mYLoc;	//world-space location
		float mXScale, mYScale;
		float mRotation;
		Animation mAnim;
		bool mIsVisible, mShouldAnimate, mHasCollider;

		Collider mThisCollider;
		
		//collder vars
		bool mHasCollided = false;
		int mLeftA, mLeftB, mRightA, mRightB;
		int mTopA, mTopB, mBottomA, mBottomB;

		bool destroyedLastFrame = false;

	public:
		Entity();

		~Entity();

		void destroy();

		//Call animation update
		void update(double timeElapsed);
		
		bool checkCollision(Collider *a, Collider *b);

		//Draw animated sprite
		void draw(GraphicsSystem *graphicsSystem, int camX, int camY);
		void draw(GraphicsSystem *graphicsSystem);
		
		//Turn on/off animating
		void shouldAnimate(bool pause);

		void setRotation(float roationAngle);

		void setAnimation(Animation targetAnim);
		Animation* getAnimation();
		//Animation getAnimation();


		//Toggle object visibility in-game
		void setVisible(bool setVisibilty);
		bool isVisible();

		//Location fucntions
		void setLoc(float x, float y);
		float getX();
		float getY();

		//Scale functions
		void setScale(float x, float y);
		float getXScale();
		float getYScale();
		float getRotation();

		void setCollider(string colliderNameTag);
		void setCollider(int w, int h, string colliderNameTag);
		Collider* getCollider();

};

#endif // !ENTITY_H
