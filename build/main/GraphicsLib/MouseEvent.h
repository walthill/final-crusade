#ifndef MOUSE_EVENT_H
#define MOUSE_EVENT_H

#include <string>
#include <trackable.h>
#include "Event.h"

using namespace std;

class MouseEvent: public Event
{
public:
	MouseEvent();
	MouseEvent(EventType type );
	MouseEvent(EventType type, int mouseX, int mouseY);
	virtual ~MouseEvent();

//	void setType(MouseEventType type, int mouseX, int);
	EventType getType() const { return mType; };
	const string& getEventName() const;
	
	void setEvent(EventType type, int mouseX, int mouseY);
	
	int getX();
	int getY();

private:
	EventType mType;
	int mX, mY;

};

#endif // !MOUSE_EVENT_H
