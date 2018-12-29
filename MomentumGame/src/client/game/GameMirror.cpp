#include "Standard.h"
#include "GameMirror.h"

#include "Global.h"
#include "PlayerMirror.h"

GameMirror::GameMirror() {
	platformList_ = new LinkedList<SDL_Rect*>();
	playerList_ = new LinkedList<PlayerMirror*>();

	levelBounds_ = { -100, -100, 1000, 1000 };
}

GameMirror::~GameMirror() {
	while (platformList_->getLength() > 0)
		delete platformList_->poll();
	delete platformList_;
	while (playerList_->getLength() > 0)
		delete playerList_->poll();
	delete playerList_;
}

PlayerMirror* GameMirror::addPlayer(int playerID) {
	PlayerMirror* p = new PlayerMirror(playerID);
	playerList_->push(p);
	return p;
}

SDL_Rect* GameMirror::addPlatform(SDL_Rect r) {
	SDL_Rect* newPlat = new SDL_Rect(r);
	platformList_->push(newPlat);
	return newPlat;
}

LinkedList<PlayerMirror*>* GameMirror::getPlayers() {
	return playerList_;
}

LinkedList<SDL_Rect*>* GameMirror::getPlatforms() {
	return platformList_;
}

PlayerMirror* GameMirror::getPlayerByID(int playerID) {
	Iterator<PlayerMirror*> it = playerList_->getIterator();
	while (it.hasNext()) {
		PlayerMirror* curr = it.next();
		if (curr->getPlayerID() == playerID)
			return curr;
	}

	return NULL;
}

void GameMirror::removePlayer(int playerID) {
	PlayerMirror* p = getPlayerByID(playerID);
	if (p == NULL) {
		log("CLIENT ERR: tried to remove nonexistent player " + to_string(playerID) + "\n");
		return;
	}

	delete p;
}

SDL_Rect GameMirror::getLevelBounds() {
	return levelBounds_;
}