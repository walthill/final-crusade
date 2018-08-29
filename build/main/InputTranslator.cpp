#include "InputTranslator.h"

int InputTranslator::mId = 0;

InputTranslator::InputTranslator()
				:EventListener(nullptr) //Null because Event System is static
{
}


InputTranslator::~InputTranslator()
{
	cleanupInputTranslator();
}


void InputTranslator::initInputTranslator()
{
	//Game controls
	EventSystem::getInstance()->addListener(ESC, this);

	EventSystem::getInstance()->addListener(UP_ARROW, this);
	EventSystem::getInstance()->addListener(DOWN_ARROW, this);
	EventSystem::getInstance()->addListener(LEFT_ARROW, this);
	EventSystem::getInstance()->addListener(RIGHT_ARROW, this);
	EventSystem::getInstance()->addListener(ENTER, this);
	EventSystem::getInstance()->addListener(DOWN_ARROW_RELEASED, this);
	EventSystem::getInstance()->addListener(LEFT_ARROW_RELEASED, this);
	EventSystem::getInstance()->addListener(RIGHT_ARROW_RELEASED, this);
	EventSystem::getInstance()->addListener(UP_ARROW_RELEASED, this);

	EventSystem::getInstance()->addListener(UP_ARROW_CONTROLLER, this);
	EventSystem::getInstance()->addListener(DOWN_ARROW_CONTROLLER, this);
	EventSystem::getInstance()->addListener(LEFT_ARROW_CONTROLLER, this);
	EventSystem::getInstance()->addListener(RIGHT_ARROW_CONTROLLER, this);
	EventSystem::getInstance()->addListener(ENTER_CONTROLLER, this);
	EventSystem::getInstance()->addListener(DOWN_ARROW_RELEASED_CONTROLLER, this);
	EventSystem::getInstance()->addListener(LEFT_ARROW_RELEASED_CONTROLLER, this);
	EventSystem::getInstance()->addListener(RIGHT_ARROW_RELEASED_CONTROLLER, this);
	EventSystem::getInstance()->addListener(UP_ARROW_RELEASED_CONTROLLER, this);
	EventSystem::getInstance()->addListener(MOUSE_MOTION, this);
	EventSystem::getInstance()->addListener(LEFT_MOUSE_DOWN, this);
	EventSystem::getInstance()->addListener(LCTRL, this);
	EventSystem::getInstance()->addListener(DEVICE_ADDED_OR_REMOVED, this);
	//EventSystem::getInstance()->addListener(R_STICK_MOTION, this);

}


void InputTranslator::cleanupInputTranslator()
{
}


void InputTranslator::handleEvent(const Event& theEvent)
{
	mEventType = theEvent.getType();
	
	switch (mEventType)
	{
		case ESC:
			mTranslatorEvent.setType(PAUSE_GAME);
			EventSystem::getInstance()->fireEvent(mTranslatorEvent);
			break;

		case LCTRL:
			mTranslatorEvent.setType(SCREENCAP);
			EventSystem::getInstance()->fireEvent(mTranslatorEvent);
			break;

		case MOUSE_MOTION:
		{	
			const MouseEvent& mouseEvent = static_cast<const MouseEvent&>(theEvent);
		//	mTranslatorMouseEvent = mouseEvent;
			//mTranslatorEvent.setType(ROTATION);

  			//mTranslatorEvent.setType(MOVE_UP);
  			EventSystem::getInstance()->fireEvent(MouseEvent(ROTATION, mouseEvent.getX(), mouseEvent.getY()));
			break;
		}
	/*	case R_STICK_MOTION:
		{
			const MouseEvent& mouseEvent = static_cast<const MouseEvent&>(theEvent);
			//	mTranslatorMouseEvent = mouseEvent;
			//mTranslatorEvent.setType(ROTATION);

			//mTranslatorEvent.setType(MOVE_UP);
			EventSystem::getInstance()->fireEvent(MouseEvent(RSTICK_ROTATION, mouseEvent.getX(), mouseEvent.getY()));
			break;
		}*/
		case DEVICE_ADDED_OR_REMOVED:
			mTranslatorEvent.setType(TOGGLE_CONTROLLER);
			EventSystem::getInstance()->fireEvent(mTranslatorEvent);
			break;
		case LEFT_MOUSE_DOWN:
			mTranslatorEvent.setType(SHOOT);
			EventSystem::getInstance()->fireEvent(mTranslatorEvent);

			break;

		case UP_ARROW:
			mTranslatorEvent.setType(MOVE_UP);
			EventSystem::getInstance()->fireEvent(mTranslatorEvent);

			break;

		case DOWN_ARROW:
			mTranslatorEvent.setType(MOVE_DOWN);
			EventSystem::getInstance()->fireEvent(mTranslatorEvent);

			break;

		case LEFT_ARROW:
			mTranslatorEvent.setType(MOVE_LEFT);
			EventSystem::getInstance()->fireEvent(mTranslatorEvent);
			break;

		case RIGHT_ARROW:
			mTranslatorEvent.setType(MOVE_RIGHT);
			EventSystem::getInstance()->fireEvent(mTranslatorEvent);
			break;

		case UP_ARROW_RELEASED:
			mTranslatorEvent.setType(STOP_UP);
			EventSystem::getInstance()->fireEvent(mTranslatorEvent);

			break;

		case DOWN_ARROW_RELEASED:
			mTranslatorEvent.setType(STOP_DOWN);
			EventSystem::getInstance()->fireEvent(mTranslatorEvent);

			break;

		case LEFT_ARROW_RELEASED:
			mTranslatorEvent.setType(STOP_LEFT);
			EventSystem::getInstance()->fireEvent(mTranslatorEvent);
			break;

		case RIGHT_ARROW_RELEASED:
			mTranslatorEvent.setType(STOP_RIGHT);
			EventSystem::getInstance()->fireEvent(mTranslatorEvent);
			break;
			case UP_ARROW_CONTROLLER:
			mTranslatorEvent.setType(MOVE_UP_CONTROLLER);
			EventSystem::getInstance()->fireEvent(mTranslatorEvent);

			break;

		case DOWN_ARROW_CONTROLLER:
			mTranslatorEvent.setType(MOVE_DOWN_CONTROLLER);
			EventSystem::getInstance()->fireEvent(mTranslatorEvent);

			break;

		case LEFT_ARROW_CONTROLLER:
			mTranslatorEvent.setType(MOVE_LEFT_CONTROLLER);
			EventSystem::getInstance()->fireEvent(mTranslatorEvent);
			break;

		case RIGHT_ARROW_CONTROLLER:
			mTranslatorEvent.setType(MOVE_RIGHT_CONTROLLER);
			EventSystem::getInstance()->fireEvent(mTranslatorEvent);
			break;

		case UP_ARROW_RELEASED_CONTROLLER:
			mTranslatorEvent.setType(STOP_UP_CONTROLLER);
			EventSystem::getInstance()->fireEvent(mTranslatorEvent);

			break;

		case DOWN_ARROW_RELEASED_CONTROLLER:
			mTranslatorEvent.setType(STOP_DOWN_CONTROLLER);
			EventSystem::getInstance()->fireEvent(mTranslatorEvent);

			break;

		case LEFT_ARROW_RELEASED_CONTROLLER:
			mTranslatorEvent.setType(STOP_LEFT_CONTROLLER);
			EventSystem::getInstance()->fireEvent(mTranslatorEvent);
			break;

		case RIGHT_ARROW_RELEASED_CONTROLLER:
			mTranslatorEvent.setType(STOP_RIGHT_CONTROLLER);
			EventSystem::getInstance()->fireEvent(mTranslatorEvent);
			break;

		case ENTER:
			mTranslatorEvent.setType(BUTTON_SELECT);
			EventSystem::getInstance()->fireEvent(mTranslatorEvent);
			break;
	}
}
