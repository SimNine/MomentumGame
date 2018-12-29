#pragma once

#include "Standard.h"
#include "LinkedList.h"

class PlayerMirror {
public:
	PlayerMirror(int playerID);
	virtual ~PlayerMirror();

	Coord getDims();
	Coord getPos();
	SDL_Color getColor();

	void setPos(Coord pos);

	int getPlayerID();

private:
	Coord dims_;
	Coord pos_;
	SDL_Color color_;
	int playerID_;
};