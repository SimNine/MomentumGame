#include "Standard.h"
#include "PlayerMirror.h"

#include "Global.h"

PlayerMirror::PlayerMirror(int playerID) {
	dims_ = { 10, 10 };
	pos_ = { 0, 0 };

	playerID_ = playerID;

	color_ = { (Uint8)rand(), (Uint8)rand(), (Uint8)rand(), (Uint8)rand() };
}

PlayerMirror::~PlayerMirror() {
	// dtor
}

Coord PlayerMirror::getDims() {
	return dims_;
}

Coord PlayerMirror::getPos() {
	return pos_;
}

SDL_Color PlayerMirror::getColor() {
	return color_;
}

void PlayerMirror::setPos(Coord pos) {
	pos_ = pos;
}

int PlayerMirror::getPlayerID() {
	return playerID_;
}