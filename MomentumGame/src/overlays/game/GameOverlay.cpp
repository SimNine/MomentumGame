#include "Standard.h"
#include "GameOverlay.h"

#include "Global.h"
#include "ResourceLoader.h"
#include "Resources.h"
#include "GUIButton.h"
#include "GUITexture.h"
#include "MiscUtil.h"
#include "GUIEffectFade.h"
#include "GUIEffectTranslate.h"

GameOverlay::GameOverlay()
	: GUIContainer(NULL, ANCHOR_NORTHWEST, { 0, 0 }, { _screenWidth, _screenHeight }, _color_black) {
}

GameOverlay::~GameOverlay() {
	//dtor
}

void GameOverlay::resetBounds() {
	GUIContainer::resetBounds();
}

bool GameOverlay::mouseDown() {
	if (GUIContainer::mouseDown())
		return true;

	/*
	Coord click = { (bkgPos_.x + _mousePos.x) / _tileWidth, (bkgPos_.y + _mousePos.y) / _tileWidth };

	if (_client->getGame()->getStatus() == GAMESTATUS_PREGAME) {
		Message msg;
		msg.type = MSGTYPE_SELECT;
		msg.pos = click;
		msg.selectType = MSGSELECTTYPE_TILE;
		_connectionManager->sendMessage(msg);
	} else if (_client->getGame()->getStatus() == GAMESTATUS_PLAYING) {
		if ((_client->getPlayer()->getSelectedProgram() == NULL || !_client->getPlayer()->canSelectedProgramMoveTo(click)) &&
			(_client->getPlayer()->getSelectedAction() == NULL || !(_client->getPlayer()->getSelectedActionDist(click) > 0))) {
			Message msg;
			msg.type = MSGTYPE_SELECT;
			msg.pos = click;
			msg.selectType = MSGSELECTTYPE_TILE;
			_connectionManager->sendMessage(msg);
		} else { // if (game->getPlayer()->canSelectedProgramMoveTo(x, y))
			if (_client->getPlayer()->getSelectedAction() == NULL) {
				Message msg;
				msg.type = MSGTYPE_MOVE;
				msg.pos = click;
				msg.playerID = _client->getPlayer()->getPlayerID();
				msg.programID = _client->getPlayer()->getSelectedProgram()->getProgramID();
				_connectionManager->sendMessage(msg);
			} else if (((_client->getPlayer()->getSelectedAction()->type_ == ACTIONTYPE_TILEPLACE && !_client->getGame()->isTiled(click) && _client->getPlayer()->getSelectedActionDistAll(click) > 0) ||
				(_client->getPlayer()->getSelectedActionDist(click) > 0 && _client->getPlayer()->getSelectedAction()->type_ != ACTIONTYPE_TILEPLACE)) &&
				_client->getPlayer()->getSelectedProgram()->getActionsLeft() > 0) {
				Message msg;
				msg.type = MSGTYPE_ACTION;
				msg.pos = click;
				msg.playerID = _client->getPlayer()->getPlayerID();
				msg.programID = _client->getPlayer()->getSelectedProgram()->getProgramID();
				_connectionManager->sendMessage(msg);
			}
		}

		return false;
	}
	*/

	return true;
}

bool GameOverlay::mouseUp() {
	return GUIContainer::mouseUp();
}

void GameOverlay::draw() {
	drawBkg();
	GUIContainer::drawContents();
}

void GameOverlay::keyPress(char c) {
	// show or hide pause menu
	if (c == SDLK_ESCAPE) {
		//_gameOverlay->showPauseMenu();
	}

	// handle chat display
	//if (c != 0)
	//	_gameOverlay->getChatDisplay()->addInputChar(out);
}

void GameOverlay::tick(int ms) {
	// tick gui objects
	GUIContainer::tick(ms);
	
	// check if the current music track is done, if so, pick a new one
	/*
	if (Mix_PlayingMusic() == 0) {
		int rand_1 = rand() % 4;
		int rand_2 = rand() % 2;
		switch (rand_1) {
		case 0:
			Mix_PlayMusic(_music_game1, rand_2);
			break;
		case 1:
			Mix_PlayMusic(_music_game2, rand_2);
			break;
		case 2:
			Mix_PlayMusic(_music_game3, rand_2);
			break;
		case 3:
			Mix_PlayMusic(_music_game4, rand_2);
			break;
		}
	}
	*/

	// scan for keys currently pressed
	/*
	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
	int shiftAmt = (int)(shiftSpeed_* (double)ms);
	if (currentKeyStates[SDL_SCANCODE_UP])
		shiftBkg({ 0, -shiftAmt });
	else if (currentKeyStates[SDL_SCANCODE_DOWN])
		shiftBkg({ 0, shiftAmt });

	if (currentKeyStates[SDL_SCANCODE_LEFT])
		shiftBkg({ -shiftAmt, 0 });
	else if (currentKeyStates[SDL_SCANCODE_RIGHT])
		shiftBkg({ shiftAmt, 0 });

	// if the mouse is at an edge, try to shift the background
	if (_mousePos.x < 20)
		shiftBkg({ -shiftAmt, 0 });
	else if (_mousePos.x > _screenWidth - 20)
		shiftBkg({ shiftAmt, 0 });

	if (_mousePos.y < 20)
		shiftBkg({ 0, -shiftAmt });
	else if (_mousePos.y > _screenHeight - 20)
		shiftBkg({ 0, shiftAmt });
		*/
}