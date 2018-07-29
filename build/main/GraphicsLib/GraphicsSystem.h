#ifndef GRAPHICS_SYS_H
#define GRAPHICS_SYS_H

#include <cassert>
#include <stdlib.h>
#include <time.h>

/*
	All object headers include Trackable in order 
	to use time and memory managment tool
*/
#include"Trackable.h"

#include "GraphicsBuffer.h"
#include "Sprite.h"
#include "Camera.h"

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
		Camera *mCamera; 
		//TODO(low): look at performance of this pointer vs passing a parameter thru scene object 

		//draw vars
		SDL_Rect drawRect;
		SDL_Rect sourceRect;
		int spriteWidth, spriteHeight;

		//screenshot vars
		const string charList = "0123456789abcdefghijklmnopqrstuwxyzABCDEFGHIJKLMNOPQRSTUWXYZ";
		const string SCREENS_FOLDER = "..\\..\\ext\\screens\\";
		string filePath;
		const int PIXEL_BIT_DEPTH = 32;
		Uint32 rMask = 0x00ff0000,
			   gMask = 0x0000ff00,
			   bMask = 0x000000ff,
			   aMask = 0xff000000;


	public:
		GraphicsSystem();	
		~GraphicsSystem();	

		//Initialize all allegro libraries
		bool initGraphics(string windowName, int displayHeight, int displayWidth, Camera *camera);
		

		//Explicit destructor
		void cleanupGraphics(); 


		//Retrieves display buffer
		GraphicsBuffer* getBackbuffer();


		//Flip the buffers and display what's in memory
		void flip();


		//Draw given sprite's assigned bitmap
		void draw(int targetX, int targetY, Sprite &spr, float scaleX, float scaleY, double rotationAngle = NULL);
		void viewDraw(int targetX, int targetY, Sprite &spr, double rotationAngle = NULL);


		//Draw to backbuffer
		void drawBackbuffer(GraphicsBuffer *targetBuffer, int displayX, int displayY,
			Sprite sprBuffer, float scaleX, float scaleY);
		
		//Save specified buffer
		void takeScreenshot(GraphicsBuffer *bufferToSave);


		//Write text to backBuffer
		void writeTextToBackbuffer(int locX, int locY, Font &fontToWrite, 
								   Color writeColor, string msg);
		

		//Write text to given buffer
		void writeText(GraphicsBuffer *targetBuffer, int locX, int locY, 
					   Font &fontToWrite, Color writeColor, string msg);

};

#endif