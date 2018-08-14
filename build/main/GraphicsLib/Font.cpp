#include "Font.h"

Font::Font()
{
	mFont = NULL;
	mFontSize = 12;
}

//TTF_RenderText_Solid render text my dude
Font::Font(string filename, int size)
{
	mFontSize = size;

	TTF_Font *fnt = TTF_OpenFont(filename.c_str(), mFontSize);
	mFont = fnt;
	fnt = NULL;
}


Font::~Font()
{
	TTF_CloseFont(mFont);
}


void Font::setFontSize(int size)
{
	mFontSize = size;
}


int Font::getFontSize()
{
	return mFontSize;
}