#ifndef FONT_H
#define FONT_H

#include "Trackable.h"
#include "SDL.h"
#include "SDL_ttf.h"

using namespace std;


class Font : public Trackable
{
	private:

		//Allows GraphicsSystem access to member variables
		friend class GraphicsSystem;

		TTF_Font* mFont;
		int mFontSize;

	public: 
		Font();
		Font(string filename, int size);
		~Font();

		int getFontSize();
		void setFontSize(int size);
};

#endif