#ifndef COLOR_H
#define COLOR_H

#include "SDL.h"
#include <Trackable.h>

using namespace std;

class Color : Trackable
{
	private:

		//Allows GraphicsSystem and GraphicsBuffer access to member variables
		friend class GraphicsSystem;
		friend class GraphicsBuffer;
		
		SDL_Color mColor;

	public:
		Color();
		Color(int r, int g, int b);

		~Color();


		void setColor(int setAllRGB);
		void setColor(int r, int g, int b);


		//Get individual rgb values
		int r();
		int g();
		int b();
		int a();
};

#endif // !COLOR_H