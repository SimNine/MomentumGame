#pragma once

#include "Standard.h"

#include "LinkedList.h"

// DEFINE used to make server function correctly for local clients
#define LOCALSERVER

class TimedEvent;
class GUIContainer;
class GUITextbox;
class TitleOverlay;
class MainOverlay;
class NotifyOverlay;
class GameOverlay;
class BackgroundOverlay;
class Server;
class Client;

/*
GLOBAL VARIABLES
*/

// window to render in
extern SDL_Window* _window;

// designate the renderer
extern SDL_Renderer* _renderer;

// designate a list of timed events
extern LinkedList<TimedEvent*>* _timedEvents;

// mouse data
extern Coord _mousePos;
extern bool _mousePressed;

// action flags
extern bool _acceptingInput;

// quit flag
extern bool _quit;

// debug level flag
extern DEBUG _debug;

// overlays
extern LinkedList<GUIContainer*>* _overlayStack;
extern TitleOverlay* _titleOverlay;
extern MainOverlay* _mainOverlay;
extern GameOverlay* _gameOverlay;
extern BackgroundOverlay* _backgroundOverlay;

extern GUIContainer* _fadeOverlay;
extern NotifyOverlay* _notifyOverlay;

// GUIContainer currently held by the mouse
extern GUIContainer* _heldContainer;

// GUITextbox currently active
extern GUITextbox* _activeTextbox;

// screen dimensions
extern int _screenWidth;
extern int _screenHeight;

// server singleton
extern Server* _server;

// client singleton
extern Client* _client;