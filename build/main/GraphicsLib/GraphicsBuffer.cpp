#include "SDL.h"
#include "GraphicsBuffer.h"
#include "Color.h"

GraphicsBuffer::GraphicsBuffer()
{
	mBitmapWidth = 0;
	mBitmapHeight = 0;
	isOwner = true;
}

GraphicsBuffer::GraphicsBuffer(SDL_Surface* bMap, SDL_Renderer* bRender)
{
	mpRenderer = bRender;
	mpSurface = bMap;
	mpBitmap = SDL_CreateTextureFromSurface(mpRenderer ,bMap);

	mBitmapHeight = bMap->h;
	mBitmapWidth = bMap->w;

	cout << "Graphics buffer initialized w/ bitmap" << endl;
	isOwner = false;
}


GraphicsBuffer::GraphicsBuffer(int width, int height)
{
	mBitmapWidth = width;
	mBitmapHeight = height;
	mpBitmap = SDL_CreateTextureFromSurface(mpRenderer ,SDL_CreateRGBSurface (0, width, height, 0, 0, 0, 0, 0));

	cout << "Graphics buffer initialized" << endl;
	isOwner = false;
}


GraphicsBuffer::GraphicsBuffer(int width, int height, string filePath)
{
	mpBitmap = SDL_CreateTextureFromSurface(mpRenderer, IMG_Load(filePath.c_str()));
	mBitmapWidth = width;
	mBitmapHeight = height;

	cout << "Buffer initialized from file" << endl;
	isOwner = true;
}


GraphicsBuffer::~GraphicsBuffer()
{
	if (isOwner)
		SDL_DestroyTexture(mpBitmap);
}

void GraphicsBuffer::initGraphicsBuffer(GraphicsBuffer *surface, int width, int height, string filePath)
{
	//NOTE: mp surface gets width and height
	mpSurface = IMG_Load(filePath.c_str());
	mpRenderer = surface->mpRenderer;

	mpBitmap = SDL_CreateTextureFromSurface(mpRenderer, mpSurface);
	
	mBitmapWidth = mpSurface->w;//width;
	mBitmapHeight = mpSurface->h; //height;

	SDL_FreeSurface(mpSurface);
	cout << "Buffer initialized from file" << endl;
	isOwner = false;

}



void GraphicsBuffer::createBitmap(int width, int height)
{
	mpBitmap = SDL_CreateTextureFromSurface(mpRenderer, SDL_CreateRGBSurface(0, width, height, 0, 0, 0, 0, 0));
}

void GraphicsBuffer::drawSolidColor(Color solidColor)
{
	//get target, set new target, set the old target
	SDL_Texture *prevBit = SDL_GetRenderTarget(mpRenderer);

	SDL_SetRenderTarget(mpRenderer, mpBitmap);
	SDL_SetRenderDrawColor(mpRenderer, solidColor.mColor.r, solidColor.mColor.g, solidColor.mColor.b, 1);
	SDL_RenderClear(mpRenderer);

	SDL_SetRenderTarget(mpRenderer, prevBit);
}


void GraphicsBuffer::cleanupGraphicsBuffer()
{
	if (mpBitmap != NULL)
	{
		SDL_DestroyTexture(mpBitmap);
		mpBitmap = NULL;
	}
}


int GraphicsBuffer::getBitmapHeight()
{
	return mBitmapHeight;
}

int GraphicsBuffer::getBitmapWidth()
{
	return mBitmapWidth;
}