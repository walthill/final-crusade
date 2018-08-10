#ifndef EVENT_H
#define EVENT_H

#include <string>
#include <trackable.h>

using namespace std;

enum EventType
{
	INVALID_EVENT_TYPE = -1,
	KEY_DOWN_EVENT,
	MOUSE_DOWN_EVENT, //Input system events, attached to Allegro

	ESC,
	MOUSE_MOTION,
	LCTRL,
	LEFT_MOUSE_DOWN,
	UP_ARROW,
	UP_ARROW_RELEASED,//Input translator events, focused on buttons/keys
	DOWN_ARROW,
	DOWN_ARROW_RELEASED,
	LEFT_ARROW,
	LEFT_ARROW_RELEASED,
	RIGHT_ARROW,
	RIGHT_ARROW_RELEASED,
	ENTER,

	PAUSE_GAME, //game evemts
	BUTTON_SELECT,
	MOVE_UP,
	MOVE_DOWN,
	MOVE_LEFT,
	MOVE_RIGHT,
	STOP_LEFT,
	STOP_RIGHT,
	STOP_UP,
	STOP_DOWN,
	ROTATION,
	SHOOT,
	SCREENCAP,
	
	UNDEFINED, //menu button events
	
	NEW_GAME,
	LOAD_GAME,
	OPTIONS,
	RETURN_MAIN,
	RETURN_OPTIONS,
	STATS,
	RESUME,
	CREDITS,
	LANG_CHANGE,
	CHANGE_AUDIO,

	QUIT, //Game events, in game actions based on input
	
	
	NUM_EVENT_TYPES //Tracks the number of event types
};

const string EVENT_NAMES[NUM_EVENT_TYPES] = {	
	"Key Down Event",											
	"Mouse Down Event",
	"Escape Key Event",
	"Up Arrow Event",
	"Down Arrow Event",
	"Left Arrow Event",
	"Right Arrow Event",
	"Space Event",
	"End Event",
	"Start Event",
	"Move Up",
	"Move Down",
	"Move Left",
	"Move Right",
	"Win",
	"Next Level",
	"Food score",
	"Bonus score",
	"Reset speed"
};

class Event:public Trackable
{
public:
	Event();
	Event( EventType type );
	virtual ~Event();

	void setType(EventType type);
	EventType getType() const { return mType; };
	const string& getEventName() const;

//	void setX(int xPos);
//	void setY(int yPos);

	//int getY() const;
//	int getX() const;

protected:
	EventType mType;
//	int x, y;
};

#endif // !EVENT_H
