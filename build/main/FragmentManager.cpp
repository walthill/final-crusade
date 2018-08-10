#include "FragmentManager.h"

FragmentManager::FragmentManager()
{

}

FragmentManager::~FragmentManager()
{
	clearManager();
}


void FragmentManager::clearManager()
{

	for (unsigned int i = 0; i < mFragmentList.size(); i++)
	{
		Fragment* tmp = mFragmentList[i];
		Collider* tmpCol = mColliderList[i];

		if (tmp != NULL)
			delete tmp;
		//if (tmpCol != NULL)
		//	delete tmpCol;
	}

	mFragmentList.clear();
	mColliderList.clear();

}


void FragmentManager::update(double elapsedTime, Player* playerObj)
{

	for (unsigned int i = 0; i < mFragmentList.size(); i++)
	{
		mFragmentList[i]->update(elapsedTime, playerObj);
	}
}


void FragmentManager::draw(GraphicsSystem *system)
{

	for (unsigned int i = 0; i < mFragmentList.size(); i++)
	{
		mFragmentList[i]->draw(system);
	}
}

void FragmentManager::draw(GraphicsSystem *system, int camX, int camY)
{

	for (unsigned int i = 0; i < mFragmentList.size(); i++)
	{
		mFragmentList[i]->draw(system, camX, camY);
	}
}


void FragmentManager::createAndAddEntity(EntityId key, int x, int y, Animation anim)
{
	Fragment *newEntity = new Fragment;

	newEntity->setAnimation(anim);
	newEntity->setLoc(x, y);

	newEntity->setCollider(anim.getCurrentSprite().getSpriteWidth()+32, anim.getCurrentSprite().getSpriteHeight()+32, "fragment");

	mFragmentList.push_back(newEntity);
	mColliderList.push_back(newEntity->getCollider());

	newEntity = NULL;

	cout << "NEW SPRITE" << endl;
}


void FragmentManager::createAndAddEntity(EntityId key, int x, int y, Animation anim, int amount)
{
	/*//pass in the animation, location
	map<EntityId, Fragment*>::iterator iter = mEntityMap.find(key);
	srand(unsigned(time(NULL)));

	for (int i = 0; i < amount; i++)
	{
	int randX = rand() % 1500;
	int randY = rand() % 1100;


	if (iter == mEntityMap.end())
	{
	Fragment *newEntity = new Fragment;

	newEntity->setAnimation(anim);
	newEntity->setLoc(randX, randY);

	mEntityMap[key] = newEntity;
	newEntity = NULL;

	cout << "NEW SPRITE" << endl;
	}
	}*/
}


void FragmentManager::addEntity(EntityId key, Fragment *objToAdd)
{
	mFragmentList.push_back(objToAdd);
	mColliderList.push_back(objToAdd->getCollider());


	/*map<EntityId, Fragment*>::iterator iter = mEntityMap.find(key);

	if (iter == mEntityMap.end())
	mEntityMap[key] = objToAdd;*/
}


void FragmentManager::removeEntity(int index)
{
	mFragmentList.erase(mFragmentList.begin() + index);
	mColliderList.erase(mColliderList.begin() + index);
	/*map<EntityId, Fragment*>::iterator iter = mEntityMap.find(key);

	if (iter != mEntityMap.end())
	mEntityMap.erase(key);*/
}


void FragmentManager::removeEntity(Fragment *objToRemove)
{
	/*	map<EntityId, Fragment*>::iterator iter;

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


Fragment* FragmentManager::getEntity(int index)
{
	return mFragmentList[index];
}


int FragmentManager::getCount()
{
	return mFragmentList.size();
}


vector<Collider*> FragmentManager::getColliderList()
{
	return mColliderList;
}