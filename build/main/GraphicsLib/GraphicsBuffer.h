#ifndef GRAPHICS_BUFFER
#define GRAPHICS_BUFFER

#include "SDL.h"
#include "SDL_image.h"
#include "Trackable.h"

using namespace std;

class Color;

class GraphicsBuffer : public Trackable
{

	private:

		//Allows GraphicsSystem access to member variables
		friend class GraphicsSystem;

		SDL_Renderer *mpRenderer;
		SDL_Texture *mpBitmap;
		SDL_Surface *mpSurface;
		int mBitmapHeight;
		int mBitmapWidth;
		GraphicsBuffer(SDL_Surface* bMap, SDL_Renderer* bRender);	//private constructor
		bool isOwner;

	public:
		GraphicsBuffer();
		GraphicsBuffer(int width, int height);
		GraphicsBuffer(int width, int height, string filePath);
		~GraphicsBuffer();

		void initGraphicsBuffer(GraphicsBuffer *surface, string filePath);


		//Explicit destructor
		void cleanupGraphicsBuffer();

		//Calls allegro's create buffer function
		void createBitmap(int width, int height);

		//Draw a single color to the entire buffer
		void drawSolidColor(Color solidColor);


		//Retrive bitmap dimensions
		int getBitmapHeight();
		int getBitmapWidth();

};

#endif