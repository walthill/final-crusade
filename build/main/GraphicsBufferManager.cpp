#include "GraphicsBufferManager.h"


GraphicsBufferManager::GraphicsBufferManager()
{

}

GraphicsBufferManager::~GraphicsBufferManager()
{
	clearManager();
}


void GraphicsBufferManager::clearManager()
{
	map<BufferId, GraphicsBuffer*>::iterator iter;

	for (iter = mBufferMap.begin(); iter != mBufferMap.end(); iter++)
	{
		GraphicsBuffer* tmp = iter->second;
		
		if (tmp != NULL)
			tmp->cleanupGraphicsBuffer();
	}

	mBufferMap.clear();
}


void GraphicsBufferManager::createAndAddGraphicsBuffer(BufferId key, int width, int height, string path)
{
	map<BufferId, GraphicsBuffer*>::iterator iter = mBufferMap.find(key);

	if (iter == mBufferMap.end())
	{
		GraphicsBuffer *newBuffer = new GraphicsBuffer;
//	TODO: (low) parameters need to be changed
		//newBuffer->initGraphicsBuffer(width, height, path);

		mBufferMap[key] = newBuffer;
		newBuffer = NULL;

		cout << "NEW BUFFER" << endl;
	}

}


void GraphicsBufferManager::addGraphicsBuffer(BufferId key, GraphicsBuffer *objToAdd)
{
	map<BufferId, GraphicsBuffer*>::iterator iter = mBufferMap.find(key);

	if (iter == mBufferMap.end())
		mBufferMap[key] = objToAdd;
}


void GraphicsBufferManager::removeGraphicsBuffer(BufferId key)
{
	map<BufferId, GraphicsBuffer*>::iterator iter = mBufferMap.find(key);

	if (iter != mBufferMap.end())
		mBufferMap.erase(key);
}


void GraphicsBufferManager::removeGraphicsBuffer(GraphicsBuffer *objToRemove)
{
	map<BufferId, GraphicsBuffer*>::iterator iter;

	for (iter = mBufferMap.begin(); iter != mBufferMap.end(); iter++)
	{
		if (objToRemove == iter->second)
		{
			mBufferMap.erase(iter);
		}
	}
}


GraphicsBuffer* GraphicsBufferManager::getGraphicsBuffer(BufferId key)
{
	map<BufferId, GraphicsBuffer*>::iterator iter = mBufferMap.find(key);

	if (iter != mBufferMap.end())
		return iter->second;
	else
		return NULL;

}
