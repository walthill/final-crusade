#include "Font.h"

#include <SDL.h>

#include "GraphicsSystem.h"
#include "Color.h"


GraphicsSystem::GraphicsSystem()
{
	mDisplay = NULL;
	mBackBuffer = NULL;
	mHeight = 0;
	mWidth = 0;
}


GraphicsSystem::~GraphicsSystem()
{
	cleanupGraphics();
}


bool GraphicsSystem::initGraphics(string windowName, int displayWidth, int displayHeight, Camera *camera)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		cout << "error with SDL init";
		return false;
	}

	if(IMG_Init(IMG_INIT_PNG) == 0)
	{
		cout << "error with SDL Image Extension initialization";
		return false;
	}

	if (TTF_Init() != 0)
	{
		cout << "error with SDL Font Extension init";
		return false;

	}

	mWidth = displayWidth;
	mHeight = displayHeight;

	mDisplay = SDL_CreateWindow(windowName.c_str(),
								300, 100,
								mWidth, mHeight,
								0);
	assert(mDisplay);

	mRenderer = SDL_CreateRenderer(mDisplay, -1, SDL_RENDERER_ACCELERATED);
	mBackBuffer = new GraphicsBuffer( SDL_GetWindowSurface(mDisplay), mRenderer);

	mCamera = camera;
	SDL_ShowCursor(1);
	
	cout << "Graphics Library Initialized" << endl;

	return true;
}


void GraphicsSystem::cleanupGraphics()
{
	if (mDisplay != NULL)
	{
		SDL_DestroyRenderer(mRenderer);
		SDL_DestroyWindow(mDisplay);
		mDisplay = NULL;

		delete mBackBuffer;
		mBackBuffer = NULL;

		cout << "Display destroyed" << endl;

		IMG_Quit();
		SDL_Quit();
	}
}


GraphicsBuffer* GraphicsSystem::getBackbuffer()
{
	return mBackBuffer;
}

void GraphicsSystem::flip()
{
	SDL_RenderPresent(mRenderer);
	SDL_RenderClear(mRenderer);
}


void GraphicsSystem::draw(int targetX, int targetY, Sprite &spr, float scaleX, float scaleY, double rotationAngle)
{
	spriteWidth = spr.getSpriteWidth();
	spriteHeight = spr.getSpriteHeight();

	sourceRect.x = spr.getSpriteX();
	sourceRect.y = spr.getSpriteY();
	sourceRect.w = spriteWidth;
	sourceRect.h = spriteHeight;

	drawRect.x = targetX;
	drawRect.y = targetY;
	drawRect.w = spriteWidth;
	drawRect.h = spriteHeight;

	SDL_RenderCopyEx(mRenderer, spr.getBuffer()->mpBitmap, &sourceRect, &drawRect, rotationAngle, NULL, SDL_FLIP_NONE);

}


void GraphicsSystem::viewDraw(int targetX, int targetY, Sprite &spr, double rotationAngle)
{
	drawRect.x = targetX;
	drawRect.y = targetY;
	drawRect.w = mCamera->camera.w;
	drawRect.h = mCamera->camera.h;

	SDL_RenderCopyEx(mRenderer, spr.getBuffer()->mpBitmap, &mCamera->camera, &drawRect, rotationAngle, NULL, SDL_FLIP_NONE);

}

void GraphicsSystem::drawBackbuffer(GraphicsBuffer *targetBuffer, int displayX, int displayY,
	Sprite sprBuffer, float scaleX, float scaleY)
{
	SDL_Texture *prevBit = SDL_GetRenderTarget(mRenderer);

	int width = sprBuffer.getSpriteWidth();
	int height = sprBuffer.getSpriteHeight();

	SDL_SetRenderTarget(mRenderer, targetBuffer->mpBitmap);
	SDL_SetRenderTarget(mRenderer, prevBit);

}


//Big help from TalesM @ StackOverflow https://bit.ly/2AAUTkU
void GraphicsSystem::takeScreenshot(GraphicsBuffer *bufferToSave, string filename)
{
	SDL_Surface *saveSurface = bufferToSave->mpSurface; //SDL_CreateRGBSurface(0, mWidth, mHeight, PIXEL_BIT_DEPTH,
													//rMask, gMask, bMask, aMask);

	SDL_RenderReadPixels(mRenderer, NULL, SDL_GetWindowPixelFormat(mDisplay),
						 saveSurface->pixels, saveSurface->pitch);

	filePath = SCREENS_FOLDER + filename;
	if (SDL_SaveBMP(saveSurface, filePath.c_str()) == 0)
	{
		cout << "Screenshot Captured!" << endl;
		//TODO(very low): add screenshot sound
	}
	else
	{
		cout << "Screenshot failed." << endl;
	}

	SDL_FreeSurface(saveSurface);

}


void GraphicsSystem::writeTextToBackbuffer(int locX, int locY, Font &fontToWrite,
	Color writeColor, string msg)
{
	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(fontToWrite.mFont, msg.c_str(), writeColor.mColor);

	SDL_Texture* Message = SDL_CreateTextureFromSurface(mRenderer, surfaceMessage); //now you can convert it into a texture

	SDL_Rect Message_rect; 
	Message_rect.x = locX;  
	Message_rect.y = locY; 
	Message_rect.w = surfaceMessage->w; 
	Message_rect.h = surfaceMessage->h; 

	SDL_RenderCopy(mRenderer, Message, NULL, &Message_rect);

	SDL_FreeSurface(surfaceMessage); 
	SDL_DestroyTexture(Message);
}


void GraphicsSystem::writeText(GraphicsBuffer *targetBuffer, int locX, int locY,
	Font &fontToWrite, Color writeColor, string msg)
{
	SDL_Texture *prevBit = SDL_GetRenderTarget(mRenderer);
	SDL_SetRenderTarget(mRenderer, targetBuffer->mpBitmap);

	TTF_RenderText_Solid(fontToWrite.mFont, msg.c_str(), writeColor.mColor);

	SDL_SetRenderTarget(mRenderer, prevBit);
}
