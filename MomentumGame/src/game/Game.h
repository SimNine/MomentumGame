#pragma once

#include "Standard.h"

#include "LinkedList.h"

class Player;

class Game {
public:
	Game();
	virtual ~Game();

	LinkedList<SDL_Rect*>* getPlatforms();
	LinkedList<Player*>* getPlayers();

	SDL_Rect getLevelBounds();

	void displacePlayer(Player* p, Coord disp);

protected:
private:
	LinkedList<SDL_Rect*>* platforms_;
	LinkedList<Player*>* players_;

	SDL_Rect levelBounds_;
};