#pragma once

#include "Standard.h"
#include "LinkedList.h"

class PlayerMirror;

class GameMirror {
public:
	GameMirror(SDL_Rect bounds);
	virtual ~GameMirror();

	LinkedList<SDL_Rect*>* getPlatforms();
	SDL_Rect* addPlatform(SDL_Rect rect);

	PlayerMirror* getPlayerByID(int playerID);
	LinkedList<PlayerMirror*>* getPlayers();
	PlayerMirror* addPlayer(int playerID);
	void removePlayer(int playerID);

	SDL_Rect getLevelBounds();
protected:
private:
	LinkedList<PlayerMirror*>* playerList_;
	LinkedList<SDL_Rect*>* platformList_;

	SDL_Rect levelBounds_;
};