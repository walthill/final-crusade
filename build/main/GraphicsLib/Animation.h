#ifndef ANIM_H
#define ANIM_H

#include "Timer.h"
#include "Sprite.h"
#include <vector>


/*
All object headers include Trackable in order
to use time and memory managment tool
*/
class Animation : public Trackable
{
	private:
		vector<Sprite> spriteList; //List of frames
		unsigned int currentSpriteIndex = 0; 
		bool isLooping, isAnimating;
		double animSpeed;
		double msPerFrame; 

	public:
		Animation();
		Animation(bool looping);
		~Animation();

		//Import sprite sheet and build into animation frames
		void addSpriteSheet(GraphicsBuffer *sprBuffer, int sheetRows, int sheetColumns, int spriteWideth, int spriteHeight);

		//Perform animation
		void update(double tiimeElapsed, bool playAnim);

		//Acessor
		Sprite getCurrentSprite();

		int getSpriteIndex();
		void setSpriteIndex(int index);

		//Determine if played animation should loop
		void setLooping(bool loop);
		
		//Change animation time
		void speedUpAnim();
		void slowAnim();

		//Operator overload
		Animation& operator =(const Animation &rhs);

};

#endif // !ANIM_H
