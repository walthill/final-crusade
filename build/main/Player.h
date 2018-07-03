#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"

class Player : public Entity
{
	private:
		int dX; 
		int dY; 

		double angle;

	public:
		Player();
		~Player();

		void update(double timeElapsed, int mouseX, int mouseY);

		void rotate(int mouseX, int mouseY);
};

#endif //!PLAYER_H