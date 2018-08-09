#ifndef ENTITY_MAN
#define ENTITY_MAN

#include <vector>
#include "Trackable.h"
#include "Ronin.h"

typedef string EntityId;

//#include "Ronin.h"

class RoninManager : public Trackable
{
	private:
		vector<Ronin*> mRoninList;
		vector<Collider*> mColliderList;

	public:
		RoninManager();
		~RoninManager();

		//Explicit destructor
		void clearManager();

		//Calls individual functions
		void update(double elapsedTime);//, RoninManager *manager);
		void draw(GraphicsSystem *system);
		void draw(GraphicsSystem *system, int camX, int camY);

		//Create object and add to collection
		void createAndAddEntity(EntityId key, int x, int y, Animation anim);
		void createAndAddEntity(EntityId key, int x, int y, Animation anim, int amount);
		void addEntity(EntityId key, Ronin *objToAdd);

		//Remove from collection by key or object
		void removeEntity(int index);
		void removeEntity(Ronin *objToRemove);

		Ronin* getEntity(int index);
		int getCount();

		vector<Collider*> getColliderList();

};

#endif // !ENTITY_MAN
