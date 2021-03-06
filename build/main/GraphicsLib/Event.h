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
	//R_STICK_MOTION,
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
	UP_ARROW_CONTROLLER,
	UP_ARROW_RELEASED_CONTROLLER,//Input translator events, focused on buttons/keys
	DOWN_ARROW_CONTROLLER,
	DOWN_ARROW_RELEASED_CONTROLLER,
	LEFT_ARROW_CONTROLLER,
	LEFT_ARROW_RELEASED_CONTROLLER,
	RIGHT_ARROW_CONTROLLER,
	RIGHT_ARROW_RELEASED_CONTROLLER,
	ENTER,
	ENTER_CONTROLLER,
	DEVICE_ADDED_OR_REMOVED,

	PAUSE_GAME, //game evemts
	BUTTON_SELECT,
	MOVE_UP,
	MOVE_DOWN,
	MOVE_LEFT,
	MOVE_RIGHT,
	MOVE_UP_CONTROLLER,
	MOVE_DOWN_CONTROLLER,
	MOVE_LEFT_CONTROLLER,
	MOVE_RIGHT_CONTROLLER, 
	STOP_LEFT,
	STOP_RIGHT,
	STOP_UP,
	STOP_DOWN,
	STOP_LEFT_CONTROLLER,
	STOP_RIGHT_CONTROLLER,
	STOP_UP_CONTROLLER,
	STOP_DOWN_CONTROLLER,
	ROTATION,
	SHOOT,
	TOGGLE_CONTROLLER,
	SCREENCAP,
	
	UNDEFINED, //menu button events
	
	NEW_GAME,
	LOAD_GAME,
	OPTIONS,
	RETURN_MAIN,
	RETURN_OPTIONS,
	RETURN_STATS,
	RETURN_CREDITS,
	STATS,
	RESUME,
	CREDITS,
	LANG_CHANGE,
	CONTROLLER,
	SHOW_CONTROLS,
	CHANGE_AUDIO,
	RETURN_TO_OPTIONS,

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
