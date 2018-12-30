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
#include "GameMirror.h"
#include "PlayerMirror.h"
#include "Client.h"
#include "Message.h"
#include "PipeMirror.h"

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
	SDL_Rect viewportRect;

	// get the players
	LinkedList<PlayerMirror*>* players = _client->getGame()->getPlayers();

	if (players->getLength() == 1) {
		viewportRect.x = 0;
		viewportRect.y = 0;
		viewportRect.w = _screenWidth;
		viewportRect.h = _screenHeight;
		drawViewport(players->getObjectAt(0), viewportRect);
	}
	else if (players->getLength() == 2) {
		viewportRect.x = 0;
		viewportRect.y = 0;
		viewportRect.w = _screenWidth / 2;
		viewportRect.h = _screenHeight;
		drawViewport(players->getObjectAt(0), viewportRect);

		viewportRect.x = _screenWidth / 2;
		viewportRect.y = 0;
		viewportRect.w = _screenWidth / 2;
		viewportRect.h = _screenHeight;
		drawViewport(players->getObjectAt(1), viewportRect);

		SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
		const int barWidth = 3;
		SDL_Rect dividerBar;
		dividerBar.x = (_screenWidth / 2) - barWidth;
		dividerBar.y = 0;
		dividerBar.w = barWidth * 2;
		dividerBar.h = _screenHeight;
		SDL_RenderFillRect(_renderer, &dividerBar);
	}
	else if (players->getLength() == 3) {
		viewportRect.x = 0;
		viewportRect.y = 0;
		viewportRect.w = _screenWidth / 2;
		viewportRect.h = _screenHeight / 2;
		drawViewport(players->getObjectAt(0), viewportRect);

		viewportRect.x = 0;
		viewportRect.y = _screenHeight / 2;
		viewportRect.w = _screenWidth / 2;
		viewportRect.h = _screenHeight / 2;
		drawViewport(players->getObjectAt(1), viewportRect);

		viewportRect.x = _screenWidth / 2;
		viewportRect.y = 0;
		viewportRect.w = _screenWidth / 2;
		viewportRect.h = _screenHeight;
		drawViewport(players->getObjectAt(2), viewportRect);

		SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
		const int barWidth = 3;
		SDL_Rect dividerBar;
		dividerBar.x = (_screenWidth / 2) - barWidth;
		dividerBar.y = 0;
		dividerBar.w = barWidth * 2;
		dividerBar.h = _screenHeight;
		SDL_RenderFillRect(_renderer, &dividerBar);
		dividerBar.x = 0;
		dividerBar.y = (_screenHeight / 2) - barWidth;
		dividerBar.w = _screenWidth / 2;
		dividerBar.h = barWidth * 2;
		SDL_RenderFillRect(_renderer, &dividerBar);
	}
	else if (players->getLength()) {
		viewportRect.x = 0;
		viewportRect.y = 0;
		viewportRect.w = _screenWidth / 2;
		viewportRect.h = _screenHeight / 2;
		drawViewport(players->getObjectAt(0), viewportRect);

		viewportRect.x = _screenWidth / 2;
		viewportRect.y = 0;
		viewportRect.w = _screenWidth / 2;
		viewportRect.h = _screenHeight / 2;
		drawViewport(players->getObjectAt(1), viewportRect);

		viewportRect.x = 0;
		viewportRect.y = _screenHeight / 2;
		viewportRect.w = _screenWidth / 2;
		viewportRect.h = _screenHeight / 2;
		drawViewport(players->getObjectAt(2), viewportRect);

		viewportRect.x = _screenWidth / 2;
		viewportRect.y = _screenHeight / 2;
		viewportRect.w = _screenWidth / 2;
		viewportRect.h = _screenHeight / 2;
		drawViewport(players->getObjectAt(3), viewportRect);

		SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
		const int barWidth = 3;
		SDL_Rect dividerBar;
		dividerBar.x = (_screenWidth / 2) - barWidth;
		dividerBar.y = 0;
		dividerBar.w = barWidth * 2;
		dividerBar.h = _screenHeight;
		SDL_RenderFillRect(_renderer, &dividerBar);
		dividerBar.x = 0;
		dividerBar.y = (_screenHeight / 2) - barWidth;
		dividerBar.w = _screenWidth;
		dividerBar.h = barWidth * 2;
		SDL_RenderFillRect(_renderer, &dividerBar);
	}
	else {
		//log("why are there more than four players?\n");
	}
}

void GameOverlay::keyPress(char c) {
	// show or hide pause menu
	if (c == SDLK_ESCAPE) {
		//_gameOverlay->showPauseMenu();
	}

	/*
	if (c == SDLK_SPACE) {
		Message m;
		m.type = MSGTYPE::INFO;
		m.subType = MSGINFOTYPE::PLAYER_ADD;
		_client->sendToPipe(m);
	}
	*/

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
	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
	double shiftSpeed_ = 0.5;
	int shiftAmt = (int)(shiftSpeed_* (double)ms);
	Coord shiftAmount = Coord{ 0, 0 };
	if (currentKeyStates[SDL_SCANCODE_UP])
		shiftAmount = shiftAmount + Coord{ 0, -shiftAmt };
	else if (currentKeyStates[SDL_SCANCODE_DOWN])
		shiftAmount = shiftAmount + Coord{ 0, shiftAmt };

	if (currentKeyStates[SDL_SCANCODE_LEFT])
		shiftAmount = shiftAmount + Coord{ -shiftAmt, 0 };
	else if (currentKeyStates[SDL_SCANCODE_RIGHT])
		shiftAmount = shiftAmount + Coord{ shiftAmt, 0 };

	// if the mouse is at an edge, try to shift the background
	/*
	if (_mousePos.x < 20)
		shiftAmount = shiftAmount + Coord{ -shiftAmt, 0 };
	else if (_mousePos.x > _screenWidth - 20)
		shiftAmount = shiftAmount + Coord{ shiftAmt, 0 };

	if (_mousePos.y < 20)
		shiftAmount = shiftAmount + Coord{ 0, -shiftAmt };
	else if (_mousePos.y > _screenHeight - 20)
		shiftAmount = shiftAmount + Coord{ 0, shiftAmt };
	*/

	// if there's no sum of movement, do nothing
	if (shiftAmount == Coord{0, 0})
		return;

	// TODO: find a better way to determine which player's movement is being sent
	if (_client->getMyClientMirror()->player_ != NULL) {
		Message m;
		m.type = MSGTYPE::INFO;
		m.subType = MSGINFOTYPE::PLAYER_MOVE;
		m.num = _client->getMyClientMirror()->player_->getPlayerID();
		m.pos = shiftAmount;
		_client->sendToPipe(m);
	}
}

void GameOverlay::drawPlatforms(Coord cameraPos) {
	SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
	Iterator<SDL_Rect*> rectIt = _client->getGame()->getPlatforms()->getIterator();
	while (rectIt.hasNext()) {
		SDL_Rect* curr = rectIt.next();
		SDL_Rect newRect = *curr;
		newRect.x -= cameraPos.x;
		newRect.y -= cameraPos.y;
		SDL_RenderFillRect(_renderer, &newRect);
	}
}

void GameOverlay::drawViewport(PlayerMirror* p, SDL_Rect viewport) {
	// calculate this player's camera position
	Coord cameraPos;
	cameraPos = p->getPos() - Coord{ viewport.w / 2, viewport.h / 2 };

	// set the viewport
	SDL_RenderSetViewport(_renderer, &viewport);

	// draw background and platforms
	drawBkg();
	drawPlatforms(cameraPos);

	// draw all players
	Iterator<PlayerMirror*> playerIt = _client->getGame()->getPlayers()->getIterator();
	while (playerIt.hasNext()) {
		drawPlayer(cameraPos, playerIt.next());
	}

	// draw the level's bounding box
	SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 255);
	SDL_Rect levelBounds = _client->getGame()->getLevelBounds();
	levelBounds.x -= cameraPos.x;
	levelBounds.y -= cameraPos.y;
	SDL_RenderDrawRect(_renderer, &levelBounds);

	// draw player position if debug string is enabled
	if (_debug >= DEBUG_NORMAL) {
		SDL_Texture* coordTex = loadString("(" + to_string(p->getPos().x) + "," + to_string(p->getPos().y) + ")", FONT_NORMAL, 10, _color_white);
		int w, h;
		SDL_QueryTexture(coordTex, NULL, NULL, &w, &h);
		SDL_Rect coordBounds{ 0, 0, w, h };
		SDL_RenderCopy(_renderer, coordTex, NULL, &coordBounds);
	}

	// reset the viewport
	SDL_Rect defaultViewport = SDL_Rect{ 0, 0, _screenWidth, _screenHeight };
	SDL_RenderSetViewport(_renderer, &defaultViewport);
}

void GameOverlay::drawPlayer(Coord cameraPos, PlayerMirror* p) {
	SDL_Rect playerRect;
	playerRect.x = p->getPos().x - cameraPos.x;
	playerRect.y = p->getPos().y - cameraPos.y;
	playerRect.w = p->getDims().x;
	playerRect.h = p->getDims().y;
	SDL_SetRenderDrawColor(_renderer, 255, 255, 0, 255);
	SDL_RenderFillRect(_renderer, &playerRect);
}