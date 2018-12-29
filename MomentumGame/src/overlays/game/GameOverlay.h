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
class PlayerMirror;

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

	void drawViewport(PlayerMirror* p, SDL_Rect viewport);
	void drawPlatforms(Coord cameraPos);
	void drawPlayer(Coord cameraPos, PlayerMirror* p);
};