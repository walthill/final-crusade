#ifndef BUFFER_MAN
#define BUFFER_MAN

#include <map>
#include "GraphicsBuffer.h"

typedef string BufferId;

class GraphicsBufferManager : public Trackable
{
	private:
		map<BufferId, GraphicsBuffer*> mBufferMap;
	
	public:
		GraphicsBufferManager();
		~GraphicsBufferManager();
		
		//Explicit destructor
		void clearManager();
		
		//Create object and add to collection
		void createAndAddGraphicsBuffer(BufferId key, int width, int height, string path);

		//Add buffer to collection
		void addGraphicsBuffer(BufferId key, GraphicsBuffer *objToAdd);
		
		//Remove from collection by object or key
		void removeGraphicsBuffer(BufferId key);
		void removeGraphicsBuffer(GraphicsBuffer *objToRemove);

		GraphicsBuffer* getGraphicsBuffer(BufferId key);

};

#endif // !BUFFER_MAN
