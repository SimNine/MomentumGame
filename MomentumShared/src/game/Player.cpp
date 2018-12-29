#include "Standard.h"
#include "Player.h"

#include "Message.h"
#include "Global.h"
#include "Server.h"

Player::Player() {
	dims_ = { 10, 10 };
	pos_ = { 0, 0 };

	playerID_ = rand();
}

Player::~Player() {
	// dtor
}

Coord Player::getDims() {
	return dims_;
}

Coord Player::getPos() {
	return pos_;
}

SDL_Color Player::getColor() {
	return color_;
}

void Player::translate(Coord disp) {
	pos_ += disp;

	Message m;
	m.clientID = 0;
	m.type = MSGTYPE::INFO;
	m.subType = MSGINFOTYPE::PLAYER_MOVE;
	m.pos = pos_;
	m.num = playerID_;
	_server->sendMessageToAllClients(m);
}

int Player::getPlayerID() {
	return playerID_;
}