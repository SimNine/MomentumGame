#pragma once

#include "Standard.h"
#include "GUIContainer.h"

class ProgramDisplayContainer;
class GUIButton;
class GUITexture;
class ProgramInventoryDisplay;
class PlayerDisplayContainer;
class ChatDisplay;
class Animation;
class Player;

class GameOverlay : public GUIContainer {
public:
	GameOverlay();
	virtual ~GameOverlay();

	// GUIContainer method overrides
	void draw();
	void tick(int);
	void keyPress(char);
	bool mouseDown();
	bool mouseUp();
	void resetBounds();

protected:
private:

	void drawViewport(Player* p, SDL_Rect viewport);
	void drawPlatforms(Coord cameraPos);
	void drawPlayer(Coord cameraPos, Player* p);
};