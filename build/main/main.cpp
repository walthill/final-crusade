/**************
	Author: Walter Hill & Karim Elzanaty
	Class: Game Architecture
	Assignment: Final Project
	Date: 3/2/18

	Program Description: Tetris

	Certification of Authenticity:
	I certify that this is entirely my own work, except where I have given fully-documented references to the work of others.
	I understand the definition and consequences of plagiarism and acknowledge that the assessor of this assignment may,
	for the purpose of assessing this assignment:- Reproduce this assignment and provide a copy to another member of
	academic staff; and/or - Communicate a copy of this assignment to a plagiarism checking service
	(which may then retain a copy of this assignment on its database for the purpose of future plagiarism checking)
************/

#include "Game.h"


int main(int argc, char* argv[])
{
	Game::initInstance();
	
	if (!Game::getInstance()->initGame())
	{
		cout << "ERROR: DISPLAY CREATION FAILED." << endl;
		return -1;
	}

	if (!Game::getInstance()->runGameLoop())
	{
		Game::getInstance()->cleanupGame();
		Game::getInstance()->cleanupInstance();

		gMemoryTracker.reportAllocations(cout);
		system("pause");

		return 1;
	}

	return 0;

}