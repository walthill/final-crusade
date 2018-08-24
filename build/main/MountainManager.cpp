#include "MountainManager.h"

MountainManager::MountainManager()
{

}

MountainManager::~MountainManager()
{
	clearManager();
}


void MountainManager::clearManager()
{

	for (unsigned int i = 0; i < mMountainList.size(); i++)
	{
		Mountain* tmp = mMountainList[i];
		Collider* tmpCol = mColliderList[i];

		if (tmp != NULL)
			delete tmp;
		//if (tmpCol != NULL)
		//	delete tmpCol;
	}

	mMountainList.clear();
	mColliderList.clear();

	/*	map<EntityId, Mountain*>::iterator iter;

	for (iter = mEntityMap.begin(); iter != mEntityMap.end(); iter++)
	{
	Mountain* tmp = iter->second;

	if (tmp != NULL)
	delete tmp;
	}

	mEntityMap.clear();*/
}


void MountainManager::update(double elapsedTime, Player *playerObj)
{

	for (unsigned int i = 0; i < mMountainList.size(); i++)
	{
		mMountainList[i]->update(elapsedTime, playerObj);
	}
	/*	map<EntityId, Mountain*>::iterator iter;

	for (iter = mEntityMap.begin(); iter != mEntityMap.end(); iter++)
	iter->second->update(elapsedTime);*/
}


void MountainManager::draw(GraphicsSystem *system)
{

	for (unsigned int i = 0; i < mMountainList.size(); i++)
	{
		mMountainList[i]->draw(system);
	}
	/*map<EntityId, Mountain*>::iterator iter;

	for (iter = mEntityMap.begin(); iter != mEntityMap.end(); iter++)
	iter->second->draw(system);*/
}

void MountainManager::draw(GraphicsSystem *system, int camX, int camY)
{

	for (unsigned int i = 0; i < mMountainList.size(); i++)
	{
		mMountainList[i]->draw(system, camX, camY);
	}
	/*	map<EntityId, Mountain*>::iterator iter;

	for (iter = mEntityMap.begin(); iter != mEntityMap.end(); iter++)
	iter->second->draw(system, camX, camY);*/
}


void MountainManager::createAndAddEntity(EntityId key, int x, int y, Animation anim)
{
	//pass in the animation, location
	//	map<EntityId, Mountain*>::iterator iter = mEntityMap.find(key);

	//if (iter == mEntityMap.end())
	//{
	Mountain *newEntity = new Mountain;

	newEntity->setAnimation(anim);
	newEntity->shouldAnimate(false);
	newEntity->getAnimation()->setLooping(false);


	newEntity->setLoc(x, y);

	newEntity->setCollider("mountain");

	mMountainList.push_back(newEntity);
	mColliderList.push_back(newEntity->getCollider());
	//mEntityMap[key] = newEntity;
	newEntity = NULL;

	cout << "NEW SPRITE" << endl;
	//}
}


void MountainManager::createAndAddEntity(EntityId key, int x, int y, Animation anim, int amount)
{
	/*//pass in the animation, location
	map<EntityId, Mountain*>::iterator iter = mEntityMap.find(key);
	srand(unsigned(time(NULL)));

	for (int i = 0; i < amount; i++)
	{
	int randX = rand() % 1500;
	int randY = rand() % 1100;


	if (iter == mEntityMap.end())
	{
	Mountain *newEntity = new Mountain;

	newEntity->setAnimation(anim);
	newEntity->setLoc(randX, randY);

	mEntityMap[key] = newEntity;
	newEntity = NULL;

	cout << "NEW SPRITE" << endl;
	}
	}*/
}


void MountainManager::addEntity(EntityId key, Mountain *objToAdd)
{
	mMountainList.push_back(objToAdd);
	mColliderList.push_back(objToAdd->getCollider());


	/*map<EntityId, Mountain*>::iterator iter = mEntityMap.find(key);

	if (iter == mEntityMap.end())
	mEntityMap[key] = objToAdd;*/
}


void MountainManager::removeEntity(int index)
{
	mMountainList.erase(mMountainList.begin() + index);
	mColliderList.erase(mColliderList.begin() + index);
	/*map<EntityId, Mountain*>::iterator iter = mEntityMap.find(key);

	if (iter != mEntityMap.end())
	mEntityMap.erase(key);*/
}


void MountainManager::removeEntity(Mountain *objToRemove)
{
	/*	map<EntityId, Mountain*>::iterator iter;

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


Mountain* MountainManager::getEntity(int index)
{
	return mMountainList[index];
}


int MountainManager::getCount()
{
	return mMountainList.size();
}


vector<Collider*> MountainManager::getColliderList()
{
	return mColliderList;
}