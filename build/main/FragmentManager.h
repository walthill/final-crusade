#ifndef FRAGMENT_MAN
#define FRAGMENT_MAN

#include <vector>
#include "Trackable.h"
#include "Fragment.h"

typedef string EntityId;

//#include "Fragment.h"

class FragmentManager : public Trackable
{
private:
	vector<Fragment*> mFragmentList;
	vector<Collider*> mColliderList;

public:
	FragmentManager();
	~FragmentManager();

	//Explicit destructor
	void clearManager();

	//Calls individual functions
	void update(double elapsedTime, Player* playerObj);//, FragmentManager *manager);
	void draw(GraphicsSystem *system);
	void draw(GraphicsSystem *system, int camX, int camY);

	//Create object and add to collection
	void createAndAddEntity(EntityId key, int x, int y, Animation anim);
	void createAndAddEntity(EntityId key, int x, int y, Animation anim, int amount);
	void addEntity(EntityId key, Fragment *objToAdd);

	//Remove from collection by key or object
	void removeEntity(int index);
	void removeEntity(Fragment *objToRemove);

	Fragment* getEntity(int index);
	int getCount();

	vector<Collider*> getColliderList();

};

#endif // !FRAGMENT_MAN
