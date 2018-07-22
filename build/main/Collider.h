#ifndef COLLIDER_H
#define COLLIDER_H

#include "SDL_rect.h"
//#include "Entity.h"
#include <string>

using namespace std;

class Entity;

class Collider
{
	
	private:
		SDL_Rect mCollider;
		string mTag;

		Entity* mParentObject;

	public:
		Collider();
		~Collider();

		void initCollider(int x, int y, int w, int h, string tag, Entity* parent);

		int getX();
		int getY();
		int getW();
		int getH();

		void setX(int x);
		void setY(int y);
		void setW(int w);
		void setH(int h);

		Entity* getEntity();

		string getTag();
		void setTag(string tag);

};

#endif // !COLLIDER_H
