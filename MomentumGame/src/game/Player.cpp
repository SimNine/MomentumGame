#include "Standard.h"
#include "Player.h"

Player::Player() {
	dims_ = { 10, 10 };
	pos_ = { 0, 0 };
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
}