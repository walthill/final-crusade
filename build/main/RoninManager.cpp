#include "RoninManager.h"

RoninManager::RoninManager()
{

}

RoninManager::~RoninManager()
{
	clearManager();
}


void RoninManager::clearManager()
{
	map<EntityId, Ronin*>::iterator iter;

	for (iter = mEntityMap.begin(); iter != mEntityMap.end(); iter++)
	{
		Ronin* tmp = iter->second;

		if (tmp != NULL)
			delete tmp;
	}

	mEntityMap.clear();
}


void RoninManager::update(double elapsedTime)
{
	map<EntityId, Ronin*>::iterator iter;

	for (iter = mEntityMap.begin(); iter != mEntityMap.end(); iter++)
		iter->second->update(elapsedTime);
}


void RoninManager::draw(GraphicsSystem *system)
{
	map<EntityId, Ronin*>::iterator iter;

	for (iter = mEntityMap.begin(); iter != mEntityMap.end(); iter++)
		iter->second->draw(system);
}

void RoninManager::draw(GraphicsSystem *system, int camX, int camY)
{
	map<EntityId, Ronin*>::iterator iter;

	for (iter = mEntityMap.begin(); iter != mEntityMap.end(); iter++)
		iter->second->draw(system, camX, camY);
}


void RoninManager::createAndAddEntity(EntityId key, int x, int y, Animation anim)
{
	//pass in the animation, location
	map<EntityId, Ronin*>::iterator iter = mEntityMap.find(key);

	if (iter == mEntityMap.end())
	{
		Ronin *newEntity = new Ronin;

		newEntity->setAnimation(anim);
		newEntity->setLoc(x, y);
		
		newEntity->setCollider("ronin");

		mEntityMap[key] = newEntity;
		newEntity = NULL;

		cout << "NEW SPRITE" << endl;
	}
}


void RoninManager::createAndAddEntity(EntityId key, int x, int y, Animation anim, int amount)
{
	//pass in the animation, location
	map<EntityId, Ronin*>::iterator iter = mEntityMap.find(key);
	srand(unsigned(time(NULL)));

	for (int i = 0; i < amount; i++)
	{
		int randX = rand() % 1500;
		int randY = rand() % 1100;


		if (iter == mEntityMap.end())
		{
			Ronin *newEntity = new Ronin;

			newEntity->setAnimation(anim);
			newEntity->setLoc(randX, randY);

			mEntityMap[key] = newEntity;
			newEntity = NULL;

			cout << "NEW SPRITE" << endl;
		}
	}
}


void RoninManager::addEntity(EntityId key, Ronin *objToAdd)
{
	map<EntityId, Ronin*>::iterator iter = mEntityMap.find(key);

	if (iter == mEntityMap.end())
		mEntityMap[key] = objToAdd;
}


void RoninManager::removeEntity(EntityId key)
{
	map<EntityId, Ronin*>::iterator iter = mEntityMap.find(key);

	if (iter != mEntityMap.end())
		mEntityMap.erase(key);
}


void RoninManager::removeEntity(Ronin *objToRemove)
{
	map<EntityId, Ronin*>::iterator iter;

	for (iter = mEntityMap.begin(); iter != mEntityMap.end(); iter++)
	{
		if (objToRemove == iter->second)
		{
			delete objToRemove;
			mEntityMap.erase(iter);

			return;
		}
	}
}


Ronin* RoninManager::getEntity(EntityId key)
{
	map<EntityId, Ronin*>::iterator iter = mEntityMap.find(key);

	if (iter != mEntityMap.end())
	{
		return iter->second;
	}
	else
	{
		return NULL;
	}

}


int RoninManager::getCount()
{
	return mEntityMap.size();
}