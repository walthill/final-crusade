#include "HiveManager.h"

HiveManager::HiveManager()
{

}

HiveManager::~HiveManager()
{
	clearManager();
}


void HiveManager::clearManager()
{

	for (unsigned int i = 0; i < mHiveList.size(); i++)
	{
		Hive* tmp = mHiveList[i];
		Collider* tmpCol = mColliderList[i];

		if (tmp != NULL)
			delete tmp;
		//if (tmpCol != NULL)
		//	delete tmpCol;
	}

	mHiveList.clear();
	mColliderList.clear();

	/*	map<EntityId, Hive*>::iterator iter;

	for (iter = mEntityMap.begin(); iter != mEntityMap.end(); iter++)
	{
	Hive* tmp = iter->second;

	if (tmp != NULL)
	delete tmp;
	}

	mEntityMap.clear();*/
}


void HiveManager::update(double elapsedTime, Player *playerObj)
{

	for (unsigned int i = 0; i < mHiveList.size(); i++)
	{
		mHiveList[i]->update(elapsedTime, playerObj);
	}
	/*	map<EntityId, Hive*>::iterator iter;

	for (iter = mEntityMap.begin(); iter != mEntityMap.end(); iter++)
	iter->second->update(elapsedTime);*/
}


void HiveManager::draw(GraphicsSystem *system)
{

	for (unsigned int i = 0; i < mHiveList.size(); i++)
	{
		mHiveList[i]->draw(system);
	}
	/*map<EntityId, Hive*>::iterator iter;

	for (iter = mEntityMap.begin(); iter != mEntityMap.end(); iter++)
	iter->second->draw(system);*/
}

void HiveManager::draw(GraphicsSystem *system, int camX, int camY)
{

	for (unsigned int i = 0; i < mHiveList.size(); i++)
	{
		mHiveList[i]->draw(system, camX, camY);
	}
	/*	map<EntityId, Hive*>::iterator iter;

	for (iter = mEntityMap.begin(); iter != mEntityMap.end(); iter++)
	iter->second->draw(system, camX, camY);*/
}


void HiveManager::createAndAddEntity(EntityId key, float x, float y, Animation anim)
{
	//pass in the animation, location
	//	map<EntityId, Hive*>::iterator iter = mEntityMap.find(key);

	//if (iter == mEntityMap.end())
	//{
	Hive *newEntity = new Hive;

	newEntity->setAnimation(anim);
	newEntity->setLoc(x, y);

	newEntity->setCollider("hive");

	mHiveList.push_back(newEntity);
	mColliderList.push_back(newEntity->getCollider());
	//mEntityMap[key] = newEntity;
	newEntity = NULL;

	cout << "NEW SPRITE" << endl;
	//}
}


void HiveManager::createAndAddEntity(EntityId key, float x, float y, Animation anim, int amount)
{
	/*//pass in the animation, location
	map<EntityId, Hive*>::iterator iter = mEntityMap.find(key);
	srand(unsigned(time(NULL)));

	for (int i = 0; i < amount; i++)
	{
	int randX = rand() % 1500;
	int randY = rand() % 1100;


	if (iter == mEntityMap.end())
	{
	Hive *newEntity = new Hive;

	newEntity->setAnimation(anim);
	newEntity->setLoc(randX, randY);

	mEntityMap[key] = newEntity;
	newEntity = NULL;

	cout << "NEW SPRITE" << endl;
	}
	}*/
}


void HiveManager::addEntity(EntityId key, Hive *objToAdd)
{
	mHiveList.push_back(objToAdd);
	mColliderList.push_back(objToAdd->getCollider());


	/*map<EntityId, Hive*>::iterator iter = mEntityMap.find(key);

	if (iter == mEntityMap.end())
	mEntityMap[key] = objToAdd;*/
}


void HiveManager::removeEntity(int index)
{
	mHiveList.erase(mHiveList.begin() + index);
	mColliderList.erase(mColliderList.begin() + index);
	/*map<EntityId, Hive*>::iterator iter = mEntityMap.find(key);

	if (iter != mEntityMap.end())
	mEntityMap.erase(key);*/
}


void HiveManager::removeEntity(Hive *objToRemove)
{
	/*	map<EntityId, Hive*>::iterator iter;

	for (iter = mEntityMap.begin(); iter != mEntityMap.end(); iter++)
	{
	if (objToRemove == iter->second)
	{
	delete objToRemove;
	mEntityMap.erase(iter);

	return;
	}
	}*/
}


Hive* HiveManager::getEntity(int index)
{
	return mHiveList[index];
}


int HiveManager::getCount()
{
	return mHiveList.size();
}


vector<Collider*> HiveManager::getColliderList()
{
	return mColliderList;
}