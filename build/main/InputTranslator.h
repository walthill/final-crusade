#ifndef INPUT_TRANS_H
#define INPUT_TRANS_H

#include "EventListener.h"
#include "System.h"

class InputTranslator : EventListener
{
	private:
		int x, y;
		static int mId;
		unsigned int mEventType;

		Event mTranslatorEvent;
		MouseEvent mTranslatorMouseEvent;
		
	public:
		InputTranslator();
		~InputTranslator();

		//Init and cleanup
		void initInputTranslator();
		void cleanupInputTranslator();

		//Event system handler
		void handleEvent(const Event& theEvent);
};

#endif // !INPUT_TRANS_H
