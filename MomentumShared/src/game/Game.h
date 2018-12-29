#pragma once

#include "Standard.h"
#include "LinkedList.h"

class Player;

class Game {
public:
	Game();
	virtual ~Game();

	LinkedList<SDL_Rect*>* getPlatforms();
	void addPlatform(SDL_Rect plat);

	Player* getPlayerByID(int playerID);
	LinkedList<Player*>* getPlayers();
	void addPlayer();
	void removePlayer(int playerID);

	SDL_Rect getLevelBounds();

	void displacePlayer(Player* p, Coord disp);

protected:
private:
	LinkedList<SDL_Rect*>* platforms_;
	LinkedList<Player*>* players_;

	SDL_Rect levelBounds_;
};