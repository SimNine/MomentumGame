#include "Standard.h"
#include "Game.h"

#include "Player.h"

Game::Game() {
	platforms_ = new LinkedList<SDL_Rect*>();

	platforms_->push(new SDL_Rect{ 100, 100, 400, 200 });
	platforms_->push(new SDL_Rect{ 800, 100, 200, 400 });

	levelBounds_ = { -100, -100, 1000, 1000 };

	players_ = new LinkedList<Player*>();

	players_->push(new Player());
	players_->push(new Player());
	players_->push(new Player());
	players_->push(new Player());
}

Game::~Game() {
	while (platforms_->getLength() > 0)
		delete platforms_->poll();
	delete platforms_;
}

LinkedList<SDL_Rect*>* Game::getPlatforms() {
	return platforms_;
}

LinkedList<Player*>* Game::getPlayers() {
	return players_;
}

SDL_Rect Game::getLevelBounds() {
	return levelBounds_;
}

void Game::displacePlayer(Player* p, Coord disp) {
	Coord pos = p->getPos();
	Coord dims = p->getDims();
	SDL_Rect playerBounds = SDL_Rect{ pos.x, pos.y, dims.x, dims.y };

	SDL_Rect newPlayerPos = playerBounds;
	newPlayerPos.x += disp.x;
	newPlayerPos.y += disp.y;

	SDL_Rect resultRect{ 0, 0, 0, 0 };
	bool intersected = false;
	Iterator<SDL_Rect*> platIt = platforms_->getIterator();
	while (platIt.hasNext()) {
		SDL_Rect* currPlat = platIt.next();
		if (intersected = SDL_IntersectRect(currPlat, &newPlayerPos, &resultRect)) {
			break;
		}
	}

	if (!intersected) {
		p->translate(disp);
	}
}