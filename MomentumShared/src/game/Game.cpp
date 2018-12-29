#include "Standard.h"
#include "Game.h"

#include "Player.h"
#include "Message.h"
#include "Global.h"
#include "Server.h"
#include "MiscUtil.h"

Game::Game() {
	platforms_ = new LinkedList<SDL_Rect*>();
	players_ = new LinkedList<Player*>();

	levelBounds_ = { -100, -100, 1000, 1000 };
}

Game::~Game() {
	while (platforms_->getLength() > 0)
		delete platforms_->poll();
	delete platforms_;
	while (players_->getLength() > 0)
		delete players_->poll();
	delete players_;
}

LinkedList<SDL_Rect*>* Game::getPlatforms() {
	return platforms_;
}

LinkedList<Player*>* Game::getPlayers() {
	return players_;
}

Player* Game::getPlayerByID(int playerID) {
	Iterator<Player*> it = players_->getIterator();
	while (it.hasNext()) {
		Player* curr = it.next();
		if (curr->getPlayerID() == playerID)
			return curr;
	}

	return NULL;
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

void Game::addPlatform(SDL_Rect r) {
	platforms_->push(new SDL_Rect(r));

	Message m;
	m.clientID = 0;
	m.type = MSGTYPE::INFO;
	m.subType = MSGINFOTYPE::PLATFORM_ADD;
	m.pos = { r.x, r.y };
	m.pos2 = { r.w, r.h };
	_server->sendMessageToAllClients(m);
}

void Game::addPlayer() {
	Player* newP = new Player();
	players_->push(newP);

	Message m;
	m.clientID = 0;
	m.type = MSGTYPE::INFO;
	m.subType = MSGINFOTYPE::PLAYER_ADD;
	m.num = newP->getPlayerID();
	_server->sendMessageToAllClients(m);
}