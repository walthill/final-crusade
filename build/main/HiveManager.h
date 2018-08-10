#ifndef HIVE_MAN_H
#define HIVE_MAN_H

#include <vector>
#include "Trackable.h"
#include "Player.h"
#include "Hive.h"

typedef string EntityId;

class HiveManager : public Trackable
{
private:
	vector<Hive*> mHiveList;
	vector<Collider*> mColliderList;

public:
	HiveManager();
	~HiveManager();

	//Explicit destructor
	void clearManager();

	//Calls individual functions
	void update(double elapsedTime, Player *playerObj);//, HiveManager *manager);
	void draw(GraphicsSystem *system);
	void draw(GraphicsSystem *system, int camX, int camY);

	//Create object and add to collection
	void createAndAddEntity(EntityId key, float x, float y, Animation anim);
	void createAndAddEntity(EntityId key, float x, float y, Animation anim, int amount);
	void addEntity(EntityId key, Hive *objToAdd);

	//Remove from collection by key or object
	void removeEntity(int index);
	void removeEntity(Hive *objToRemove);

	Hive* getEntity(int index);
	int getCount();

	vector<Collider*> getColliderList();

};

#endif // !Hive_MAN_H
