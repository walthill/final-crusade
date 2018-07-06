#include "Camera.h"

Camera::Camera()
{
	camera = { 0,0,0,0 };
}

Camera::Camera(int maxWidth, int maxHeight)
{
	camera = { 0,0, maxWidth, maxHeight };
}

Camera::Camera(int x, int y, int maxWidth, int maxHeight)
{
	camera = { x,y, maxWidth, maxHeight };
}

Camera::~Camera() 
{
}

void Camera::setX(int xLoc)
{
	camera.x = xLoc;
}

void Camera::setY(int yLoc)
{
	camera.y = yLoc;
}
void Camera::setW(int width)
{
	//if (width < 0)
//		camera.w = 0;
//	else
		camera.w = width;
}

void Camera::setH(int height)
{
	camera.h = height;
}

int Camera::getX()
{
	return camera.x;
}

int Camera::getY()
{
	return camera.y;
}

int Camera::getW()
{
	return camera.w;
}

int Camera::getH()
{
	return camera.h;
}

/*SDL_Rect* Camera::getCamera()
{
	return &camera;
}*/