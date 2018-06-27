#include "Color.h"

Color::Color()
{
	mColor.r = 0;
	mColor.g = 0;
	mColor.b = 0;
}


Color::Color(int r, int g, int b)
{
	mColor.r = r;
	mColor.g = g;
	mColor.b = b;
}


Color::~Color()
{
}


void Color::setColor(int r, int g, int b)
{
	mColor.r = r;
	mColor.g = g;
	mColor.b = b;

}


void Color::setColor(int setRGB)
{
	mColor.r = setRGB;
	mColor.g = setRGB;
	mColor.b = setRGB;

}


int Color::r()
{
	return mColor.r;
}


int Color::g()
{
	return mColor.g;

}


int Color::b()
{
	return mColor.b;
}


int Color::a()
{
	return mColor.a;
}