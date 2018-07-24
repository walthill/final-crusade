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

	for (int i = 0; i < mRoninList.size(); i++)
	{
		Ronin* tmp = mRoninList[i];
		Collider* tmpCol = mColliderList[i];

		if (tmp != NULL)
			delete tmp;
		//if (tmpCol != NULL)
		//	delete tmpCol;
	}

	mRoninList.clear();
	mColliderList.clear();

/*	map<EntityId, Ronin*>::iterator iter;

	for (iter = mEntityMap.begin(); iter != mEntityMap.end(); iter++)
	{
		Ronin* tmp = iter->second;

		if (tmp != NULL)
			delete tmp;
	}

	mEntityMap.clear();*/
}


void RoninManager::update(double elapsedTime)
{

	for (int i = 0; i < mRoninList.size(); i++)
	{
		mRoninList[i]->update(elapsedTime);
	}
/*	map<EntityId, Ronin*>::iterator iter;

	for (iter = mEntityMap.begin(); iter != mEntityMap.end(); iter++)
		iter->second->update(elapsedTime);*/
}


void RoninManager::draw(GraphicsSystem *system)
{

	for (int i = 0; i < mRoninList.size(); i++)
	{
		mRoninList[i]->draw(system);
	}
	/*map<EntityId, Ronin*>::iterator iter;

	for (iter = mEntityMap.begin(); iter != mEntityMap.end(); iter++)
		iter->second->draw(system);*/
}

void RoninManager::draw(GraphicsSystem *system, int camX, int camY)
{

	for (int i = 0; i < mRoninList.size(); i++)
	{
		mRoninList[i]->draw(system, camX, camY);
	}
/*	map<EntityId, Ronin*>::iterator iter;

	for (iter = mEntityMap.begin(); iter != mEntityMap.end(); iter++)
		iter->second->draw(system, camX, camY);*/
}


void RoninManager::createAndAddEntity(EntityId key, int x, int y, Animation anim)
{
	//pass in the animation, location
//	map<EntityId, Ronin*>::iterator iter = mEntityMap.find(key);

	//if (iter == mEntityMap.end())
	//{
		Ronin *newEntity = new Ronin;

		newEntity->setAnimation(anim);
		newEntity->setLoc(x, y);
		
		newEntity->setCollider("ronin");

		mRoninList.push_back(newEntity);
		mColliderList.push_back(newEntity->getCollider());
		//mEntityMap[key] = newEntity;
		newEntity = NULL;

		cout << "NEW SPRITE" << endl;
	//}
}


void RoninManager::createAndAddEntity(EntityId key, int x, int y, Animation anim, int amount)
{
	/*//pass in the animation, location
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
	}*/
}


void RoninManager::addEntity(EntityId key, Ronin *objToAdd)
{
	mRoninList.push_back(objToAdd);
	mColliderList.push_back(objToAdd->getCollider());


	/*map<EntityId, Ronin*>::iterator iter = mEntityMap.find(key);

	if (iter == mEntityMap.end())
		mEntityMap[key] = objToAdd;*/
}


void RoninManager::removeEntity(int index)
{
	mRoninList.erase(mRoninList.begin() + index);
	mColliderList.erase(mColliderList.begin() + index);
	/*map<EntityId, Ronin*>::iterator iter = mEntityMap.find(key);

	if (iter != mEntityMap.end())
		mEntityMap.erase(key);*/
}


void RoninManager::removeEntity(Ronin *objToRemove)
{
/*	map<EntityId, Ronin*>::iterator iter;

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


Ronin* RoninManager::getEntity(EntityId key)
{
	/*map<EntityId, Ronin*>::iterator iter = mEntityMap.find(key);

	if (iter != mEntityMap.end())
	{
		return iter->second;
	}
	else
	{
		return NULL;
	}*/
	return nullptr;
}


int RoninManager::getCount()
{
	return mRoninList.size();
}


vector<Collider*> RoninManager::getColliderList()
{
	return mColliderList;
}