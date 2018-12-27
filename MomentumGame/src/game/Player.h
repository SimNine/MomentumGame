#pragma once

#include "Standard.h"

#include "LinkedList.h"

class Player {
public:
	Player();
	virtual ~Player();

	Coord getDims();
	Coord getPos();
	SDL_Color getColor();

	void translate(Coord disp);

protected:
private:
	Coord dims_;
	Coord pos_;
	SDL_Color color_;
};