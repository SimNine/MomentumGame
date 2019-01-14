#pragma once

#include "Standard.h"
#include "LinkedList.h"

class PlayerMirror {
public:
	PlayerMirror(int playerID);
	virtual ~PlayerMirror();

	Coord getDims();
	void setDims(Coord dims);

	Coord getPos();
	void setPos(Coord pos);

	SDL_Color getColor();

	int getPlayerID();

private:
	Coord dims_;
	Coord pos_;
	SDL_Color color_;
	int playerID_;
};