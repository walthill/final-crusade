#ifndef GRAPHICS_SYS_H
#define GRAPHICS_SYS_H

#include <cassert>
#include <stdlib.h>

/*
	All object headers include Trackable in order 
	to use time and memory managment tool
*/
#include"Trackable.h"

#include "GraphicsBuffer.h"
#include "Sprite.h"

struct SDL_Window;
class Font;

class GraphicsSystem : Trackable
{
	private:
		SDL_Window* mDisplay;		//The game window
		SDL_Renderer* mRenderer;
		GraphicsBuffer *mBackBuffer;	//The window's own backbuffer
		int mHeight, mWidth;			
		const int mDEFAULT_FLAG = 0;	//Flag should always be zero

	public:
		GraphicsSystem();	
		~GraphicsSystem();	

		//Initialize all allegro libraries
		bool initGraphics(int displayHeight, int displayWidth);
		

		//Explicit destructor
		void cleanupGraphics(); 


		//Retrieves display buffer
		GraphicsBuffer* getBackbuffer();


		//Flip the buffers and display what's in memory
		void flip();


		//Draw given sprite's assigned bitmap
		void draw(int targetX, int targetY, Sprite &spr, float scaleX, float scaleY);


		//Draw to backbuffer
		void drawBackbuffer(GraphicsBuffer *targetBuffer, int displayX, int displayY,
			Sprite sprBuffer, float scaleX, float scaleY);
		

		//Draw sprite from sheet
		void drawSprite(GraphicsBuffer *targetBuffer, int sourceX, int sourceY,
						Sprite sprBuffer, int spriteW, int spriteH, int displayX, int displayY);
		

		//Save specified buffer
		void saveBuffer(GraphicsBuffer *bufferToSave, string filename);


		//Write text to backBuffer
		void writeTextToBackbuffer(int locX, int locY, Font &fontToWrite, 
								   Color writeColor, string msg);
		

		//Write text to given buffer
		void writeText(GraphicsBuffer *targetBuffer, int locX, int locY, 
					   Font &fontToWrite, Color writeColor, string msg);

};

#endif