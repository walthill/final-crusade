#ifndef ENTITY_MAN
#define ENTITY_MAN

#include <map>
#include "Trackable.h"
#include "Ronin.h"

typedef string EntityId;

#include "Ronin.h"

class RoninManager : Trackable
{
	private:

		//TODO: turn this into a vector??
		map<EntityId, Ronin*> mEntityMap;

		//TODO: collider list??


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
		void removeEntity(EntityId key);
		void removeEntity(Ronin *objToRemove);

		Ronin* getEntity(EntityId key);
		int getCount();

};

#endif // !ENTITY_MAN
