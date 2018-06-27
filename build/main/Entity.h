#ifndef ENTITY_H
#define ENTITY_H

#include "GraphicsSystem.h"
#include "Animation.h"

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
		Animation mAnim;
		bool mVisibility, mShouldAnimate;

		

	public:
		Entity();
		~Entity();

		//Call animation update
		void update(double timeElapsed);
		//Draw animated sprite
		void draw(GraphicsSystem *graphicsSystem);

		
		//Give mAnim a new sprite
		void shouldAnimate(bool pause);


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
};

#endif // !ENTITY_H
