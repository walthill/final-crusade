#ifndef CAMERA_H
#define CAMERA_H

#include "SDL.h"

class Camera
{
	private:
		SDL_Rect camera;
	
	public:
		Camera();
		Camera(int maxWidth, int maxHeight);
		Camera(int x, int y, int maxWidth, int maxHeight);

		~Camera();

		void setX(int xLoc);
		void setY(int yLoc);
		void setW(int width);
		void setH(int height);


		int getX();
		int getY();
		int getW();
		int getH();
};

#endif // !CAMERA_H

