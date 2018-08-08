#ifndef MOUNTAIN_MAN_H
#define MOUNTAIN_MAN_H

#include <vector>
#include "Trackable.h"
#include "Mountain.h"

typedef string EntityId;

class MountainManager : public Trackable
{
	private:
		vector<Mountain*> mMountainList;
		vector<Collider*> mColliderList;

	public:
		MountainManager();
		~MountainManager();

		//Explicit destructor
		void clearManager();

		//Calls individual functions
		void update(double elapsedTime, Player *playerObj);//, MountainManager *manager);
		void draw(GraphicsSystem *system);
		void draw(GraphicsSystem *system, int camX, int camY);

		//Create object and add to collection
		void createAndAddEntity(EntityId key, int x, int y, Animation anim);
		void createAndAddEntity(EntityId key, int x, int y, Animation anim, int amount);
		void addEntity(EntityId key, Mountain *objToAdd);

		//Remove from collection by key or object
		void removeEntity(int index);
		void removeEntity(Mountain *objToRemove);

		Mountain* getEntity(EntityId key);
		int getCount();

		vector<Collider*> getColliderList();

};

#endif // !MOUNTAIN_MAN_H
