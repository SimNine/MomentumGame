#include "Standard.h"
#include "Global.h"

#include "GUIContainer.h"
#include "GUITextbox.h"

#include "TitleOverlay.h"

/*
GLOBAL VARIABLES
*/

// window to render to
SDL_Window* _window = NULL;

// designate the renderer
SDL_Renderer* _renderer = NULL;

// designate a list of timed events
LinkedList<TimedEvent*>* _timedEvents = new LinkedList<TimedEvent*>();

// designate the GUI_containers
LinkedList<GUIContainer*>* _overlayStack = new LinkedList<GUIContainer*>();
TitleOverlay* _titleOverlay = NULL;
MainOverlay* _mainOverlay = NULL;
GameOverlay* _gameOverlay = NULL;
BackgroundOverlay* _backgroundOverlay = NULL;

GUIContainer* _fadeOverlay = NULL;
NotifyOverlay* _notifyOverlay = NULL;

// mouse position
Coord _mousePos = { 0, 0 };
bool _mousePressed = false;

// update flags
bool _acceptingInput = true;

// quit flag
bool _quit = false;

// debug flag
DEBUG _debug = DEBUG_NONE;

// GUIContainer currently held by the mouse, if any
GUIContainer* _heldContainer = NULL;

// GUITextbox currently active, if any
GUITextbox* _activeTextbox = NULL;

// initial (and minimum) screen dimension constants
int _screenWidth = 800;
int _screenHeight = 600;

// game singleton
Game* _game = NULL;