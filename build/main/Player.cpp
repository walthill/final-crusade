#include "Player.h"

Player::Player()
{

}

Player::~Player()
{

}

void Player::update(double timeElapsed, int mouseX, int mouseY)
{
	//Entity::update(timeElapsed);

	rotate(mouseX, mouseY);
}

void Player::rotate(int mouseX, int mouseY)
{
	//Math salvation from jordsti @ https://bit.ly/2KrJx7Y

	dX = mXLoc - mouseX;
	dY = mYLoc - mouseY;

	angle = (atan2(dY, dX)*180.0) / 3.1416;
	setRotation(angle);

}
