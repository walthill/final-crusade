#ifndef CAMERA_H
#define CAMERA_H

#include "SDL.h"
#include "Trackable.h"

class Camera : public Trackable
{
	friend class GraphicsSystem;

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

		//SDL_Rect* getCamera();
};

#endif // !CAMERA_H

