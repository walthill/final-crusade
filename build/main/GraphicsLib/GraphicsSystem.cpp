//NOTE: Additional allegro libraries included into this file

#include "Font.h"

#include <SDL.h>
//#include <SDL_main.h>

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


bool GraphicsSystem::initGraphics(int displayWidth, int displayHeight)
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
	/*if (!al_init())
	{
		cout << "error initting Allegro\n";
		return false;
	}
	if (!al_init_image_addon())
	{
		cout << "error - Image Add-on not initted\n";
		return false;
	}
	if (!al_init_font_addon())
	{
		cout << "error - Font Add-on not initted\n";
		return false;
	}
	if (!al_init_ttf_addon())
	{
		cout << "error - TTF Add-on not initted\n";
		return false;
	}
	if (!al_init_primitives_addon())
	{
		cout << "error - primitives Add-on not initted\n";
		return false;
	}
	if (!al_install_audio())
	{
		cout << "error - Audio Add-on not initted\n";
		return false;
	}
	if (!al_init_acodec_addon())
	{
		cout << "error - Audio Codec Add-on not initted\n";
		return false;
	}
	if (!al_reserve_samples(1))
	{
		cout << "error - samples not reserved\n";
		return false;
	}*/

	mDisplay = SDL_CreateWindow("Final Crusade",
								300, 100,
								displayWidth, displayHeight,
								0);
	assert(mDisplay);

	mRenderer = SDL_CreateRenderer(mDisplay, -1, SDL_RENDERER_ACCELERATED);
	mBackBuffer = new GraphicsBuffer( SDL_GetWindowSurface(mDisplay), mRenderer);

	
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
	int width = spr.getSpriteWidth();
	int height = spr.getSpriteHeight();
	SDL_Rect sourceRect;
	SDL_Rect drawRect;

	sourceRect.x = spr.getSpriteX();
	sourceRect.y = spr.getSpriteY();
	sourceRect.w = width;
	sourceRect.h = height;
	
	drawRect.x = targetX;
	drawRect.y = targetY;
	drawRect.w = width;
	drawRect.h = height;

	SDL_RenderCopyEx(mRenderer, spr.getBuffer()->mpBitmap, &sourceRect, &drawRect, rotationAngle, NULL, SDL_FLIP_NONE);
}


void GraphicsSystem::drawBackbuffer(GraphicsBuffer *targetBuffer, int displayX, int displayY,
	Sprite sprBuffer, float scaleX, float scaleY)
{
	SDL_Texture *prevBit = SDL_GetRenderTarget(mRenderer);

	int width = sprBuffer.getSpriteWidth();
	int height = sprBuffer.getSpriteHeight();

	SDL_SetRenderTarget(mRenderer, targetBuffer->mpBitmap);
	/*al_draw_scaled_bitmap(sprBuffer.getBuffer()->mBitmap, sprBuffer.getSpriteX(), sprBuffer.getSpriteY(),
						  width, height, displayX, displayY, width*scaleX, 
						  height*scaleY, mDEFAULT_FLAG);
						  */
	SDL_SetRenderTarget(mRenderer, prevBit);


}

void GraphicsSystem::drawSprite(GraphicsBuffer *targetBuffer, int sourceX, int sourceY,
	Sprite sprBuffer, int spriteW, int spriteH,
	int displayX, int displayY)
{
	//al_draw_bitmap_region(targetBuffer->mBitmap, sourceX, sourceY, spriteW, spriteH, displayX, displayY, mDEFAULT_FLAG);
}

//TODO: (low) test this function
void GraphicsSystem::saveBuffer(GraphicsBuffer *bufferToSave, string filename)
{
	/*int SDL_SaveBMP(SDL_Surface* surface,
		const char*  file)
*/
	SDL_Surface *tmpSurface = nullptr;
	tmpSurface = SDL_GetWindowSurface(mDisplay);
	SDL_SaveBMP(tmpSurface, filename.c_str());

//	SDL_SaveBMP( bufferToSave->mBitmap, filename.c_str());
	cout << "Buffer saved " << endl;
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
