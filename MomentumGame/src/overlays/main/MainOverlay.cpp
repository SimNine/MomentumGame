#include "Standard.h"
#include "MainOverlay.h"

#include "Global.h"
#include "Resources.h"
#include "GUITexture.h"
#include "GUIButton.h"
#include "GUISlider.h"
#include "PromptBox.h"
#include "GUITextbox.h"
#include "AchievementButton.h"
#include "GUIEffectFade.h"
#include "GUIEffectTranslate.h"
#include "TimedEvent.h"
#include "BackgroundOverlay.h"
#include "GameOverlay.h"
#include "Client.h"
#include "Message.h"

MainOverlay::MainOverlay()
	: GUIContainer(NULL, ANCHOR_NORTHWEST, { 0, 0 }, { _screenWidth, _screenHeight }, _color_clear) {
	GUITexture* main_title = new GUITexture(this, ANCHOR_NORTHWEST, { 20, 20 }, _title_title);
	addObject(main_title);
	GUITexture* main_subtitle = new GUITexture(this, ANCHOR_NORTHWEST, { 20, 82 }, _title_sub);
	addObject(main_subtitle);

	/*
	OPTIONS CONTAINER
	*/

	optionsContainer_ = new GUIContainer(this, ANCHOR_SOUTHWEST, { 0, 0 }, { 1000, 500 }, _color_clear);
	GUIButton* options_backbutton = new GUIButton(optionsContainer_, ANCHOR_SOUTHWEST, { 20, -20 }, "BACK", 50,
		[] () {
		_mainOverlay->hideOptions(1000);
		_mainOverlay->showMainContainer(1000);
	});
	optionsContainer_->addObject(options_backbutton);
	GUISlider* options_slider_sound = new GUISlider(optionsContainer_, ANCHOR_SOUTHWEST, { 270, -150 }, { 200, 50 },
		[] (float d) {
		Mix_Volume(-1, (int)(d * 128));
	}, (float)((double)Mix_Volume(-1, -1)/128.0));
	optionsContainer_->addObject(options_slider_sound);
	GUITexture* options_label_soundslider = new GUITexture(optionsContainer_, ANCHOR_SOUTHWEST, { 20, -150 }, "SOUND VOLUME:", 50);
	optionsContainer_->addObject(options_label_soundslider);
	GUISlider* options_slider_music = new GUISlider(optionsContainer_, ANCHOR_SOUTHWEST, { 270, -210 }, { 200, 50 },
		[] (float d) {
		Mix_VolumeMusic((int)(d * 128));
	}, (float)((double)Mix_VolumeMusic(-1)/128.0));
	optionsContainer_->addObject(options_slider_music);
	GUITexture* options_label_musicslider = new GUITexture(optionsContainer_, ANCHOR_SOUTHWEST, { 20, -220 }, "MUSIC VOLUME:", 50);
	optionsContainer_->addObject(options_label_musicslider);
	GUIButton* options_fullscreenbutton = new GUIButton(optionsContainer_, ANCHOR_SOUTHWEST, { 20, -280 }, "TOGGLE FULLSCREEN", 50,
		[] () {
		//toggleFullscreen();
	});
	optionsContainer_->addObject(options_fullscreenbutton);
	addObject(optionsContainer_);

	optionsContainer_->setMovable(false);
	optionsContainer_->setTransparency(0);

	/*
	MAIN CONTAINER
	*/

	mainContainer_ = new GUIContainer(this, ANCHOR_SOUTHWEST, { 0, 0 }, { 1000, 500 }, _color_clear);
	mainContainer_->setMovable(false);
	this->addObject(mainContainer_);
	int ln = 0;
	GUIButton* button_quit = new GUIButton(mainContainer_, ANCHOR_SOUTHWEST, { 20, -(41 + 20)*ln++ - 20 }, "QUIT", 50,
		[] () {
		Mix_PlayChannel(-1, _sound_move_player, 0);
		_quit = true;
	});
	mainContainer_->addObject(button_quit);
	GUIButton* button_credits = new GUIButton(mainContainer_, ANCHOR_SOUTHWEST, { 20, -(41 + 20)*ln++ - 20 }, "CREDITS", 50,
		[]() {
		_fadeOverlay->addEffect(new GUIEffectFade(0, 1000, 0, 255));
		_timedEvents->addLast(new TimedEvent(1100,
			[]() {
			_mainOverlay->hideMainContainer(0);
			_mainOverlay->showCreditsContainer(0);
			_backgroundOverlay->setMode(BKGMODE_RAINBOW);
		}));
		_fadeOverlay->addEffect(new GUIEffectFade(1200, 1000, 255, 0));
	});
	mainContainer_->addObject(button_credits);
	GUIButton* button_options = new GUIButton(mainContainer_, ANCHOR_SOUTHWEST, { 20, -(41 + 20)*ln++ - 20 }, "OPTIONS", 50,
		[] () {
		_mainOverlay->showOptions(1000);
		_mainOverlay->hideMainContainer(1000);
		Mix_PlayChannel(-1, _sound_move_player, 0);
	});
	mainContainer_->addObject(button_options);
	GUIButton* button_game = new GUIButton(mainContainer_, ANCHOR_SOUTHWEST, { 20, -(41 + 20)*ln++ - 20 }, "GAME", 50,
		[]() {
		_client->connectToLocalServer();
		//_client->connectToExternalServer("localhost");

		Message m;
		m.type = MSGTYPE::STARTGAME;
		_client->sendToPipe(m);

		_fadeOverlay->addEffect(new GUIEffectFade(0, 1000, 0, 255));
		_timedEvents->addLast(new TimedEvent(1100,
			[]() {
			_overlayStack->removeAll();
			_overlayStack->push(_gameOverlay);
		}));
		_fadeOverlay->addEffect(new GUIEffectFade(1200, 1000, 255, 0));
	});
	mainContainer_->addObject(button_game);

	/*
	CREDITS CONTAINER
	*/
	creditsContainer_ = new GUIContainer(this, ANCHOR_CENTER, { 0,0 }, { 800, 600 }, _color_clear);
	creditsContainer_->setTransparency(0);
	this->addObject(creditsContainer_);
	
	int yDispCredits = 30;
	creditsContainer_->addObject(new GUITexture(creditsContainer_, ANCHOR_NORTH, { 0, yDispCredits }, "A GAME BY", 30));
	yDispCredits += 30;
	creditsContainer_->addObject(new GUITexture(creditsContainer_, ANCHOR_NORTH, { 0, yDispCredits }, "URF SOFTWARE", 40));
	yDispCredits += 40;

	yDispCredits += 30;

	creditsContainer_->addObject(new GUITexture(creditsContainer_, ANCHOR_NORTH, { 0, yDispCredits }, "GAME DESIGN:", 30));
	yDispCredits += 30;
	creditsContainer_->addObject(new GUITexture(creditsContainer_, ANCHOR_NORTH, { 0, yDispCredits }, "CHRIS URFFER", 20));
	yDispCredits += 20;

	yDispCredits += 30;

	creditsContainer_->addObject(new GUITexture(creditsContainer_, ANCHOR_NORTH, { 0, yDispCredits }, "GRAPHICS:", 30));
	yDispCredits += 30;
	creditsContainer_->addObject(new GUITexture(creditsContainer_, ANCHOR_NORTH, { 0, yDispCredits }, "CHRIS URFFER", 20));
	yDispCredits += 20;

	yDispCredits += 30;

	creditsContainer_->addObject(new GUITexture(creditsContainer_, ANCHOR_NORTH, { 0, yDispCredits }, "AUDIO:", 30));
	yDispCredits += 30;
	creditsContainer_->addObject(new GUITexture(creditsContainer_, ANCHOR_NORTH, { 0, yDispCredits }, "NOBODY JUST YET", 20));
	yDispCredits += 20;

	yDispCredits += 30;

	creditsContainer_->addObject(new GUITexture(creditsContainer_, ANCHOR_NORTH, { 0, yDispCredits }, "PROGRAMMING:", 30));
	yDispCredits += 30;
	creditsContainer_->addObject(new GUITexture(creditsContainer_, ANCHOR_NORTH, { 0, yDispCredits }, "CHRIS URFFER", 40));
	yDispCredits += 20;

	GUIButton* creditsBackButton = new GUIButton(creditsContainer_, ANCHOR_SOUTH, { 0, 0 }, "BACK", 30,
		[]() {
		_fadeOverlay->addEffect(new GUIEffectFade(0, 1000, 0, 255));
		_timedEvents->addLast(new TimedEvent(1100,
			[]() {
			_mainOverlay->showMainContainer(0);
			_mainOverlay->hideCreditsContainer(0);
			_backgroundOverlay->setMode(BKGMODE_STANDARD);
		}));
		_fadeOverlay->addEffect(new GUIEffectFade(1200, 1000, 255, 0));
	});
	creditsContainer_->addObject(creditsBackButton);
}

MainOverlay::~MainOverlay() {
	//dtor
}

void MainOverlay::draw() {
	// draw GUIObjects
	drawContents();
}

void MainOverlay::tick(int ms) {
	// tick all GUIObjects
	GUIContainer::tick(ms);
}

void MainOverlay::showMainContainer(int fadeDuration) {
	mainContainer_->addEffect(new GUIEffectTranslate(0, fadeDuration, Coord{ 100, 0 }, Coord{ 0, 0 }));
	mainContainer_->addEffect(new GUIEffectFade(0, fadeDuration, mainContainer_->getTransparency(), 255));
}

void MainOverlay::hideMainContainer(int fadeDuration) {
	mainContainer_->addEffect(new GUIEffectFade(0, fadeDuration, mainContainer_->getTransparency(), 0));
}

void MainOverlay::showCreditsContainer(int fadeDuration) {
	creditsContainer_->addEffect(new GUIEffectFade(0, fadeDuration, mainContainer_->getTransparency(), 255));
}

void MainOverlay::hideCreditsContainer(int fadeDuration) {
	creditsContainer_->addEffect(new GUIEffectFade(0, fadeDuration, mainContainer_->getTransparency(), 0));
}

void MainOverlay::showOptions(int fadeDuration) {
	optionsContainer_->addEffect(new GUIEffectFade(0, fadeDuration, optionsContainer_->getTransparency(), 255));
}

void MainOverlay::hideOptions(int fadeDuration) {
	optionsContainer_->addEffect(new GUIEffectFade(0, fadeDuration, optionsContainer_->getTransparency(), 0));
}

void MainOverlay::hideAll(int fadeDuration) {
	mainContainer_->addEffect(new GUIEffectFade(0, fadeDuration, mainContainer_->getTransparency(), 0));
	optionsContainer_->addEffect(new GUIEffectFade(0, fadeDuration, optionsContainer_->getTransparency(), 0));
}

void MainOverlay::keyPress(char c) {
	// do nothing
}