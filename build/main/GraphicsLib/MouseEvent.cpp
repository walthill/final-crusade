#include "MouseEvent.h"

MouseEvent::MouseEvent()
{
	mType = INVALID_EVENT_TYPE;
}

MouseEvent::MouseEvent(EventType type)
:mType(type)
{
}

MouseEvent::MouseEvent(EventType type, int mouseX, int mouseY)
{
	mType = type;
	mX = mouseX;
	mY = mouseY;
}

MouseEvent::~MouseEvent()
{
}

void MouseEvent::setEvent(EventType type, int mouseX, int mouseY)
{
	mType = type;
	mX = mouseX;
	mY = mouseY;
}

const string& MouseEvent::getEventName() const
{
	if( mType > INVALID_EVENT_TYPE && mType < NUM_EVENT_TYPES )
	{
		return EVENT_NAMES[mType];
	}
	else
	{
		static const string nullString;
		return nullString;
	}
}

int MouseEvent::getX()
{
	return mX;
}
int MouseEvent::getY()
{
	return mY;
}

