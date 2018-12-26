#pragma once

#include "Standard.h"
#include "GUIContainer.h"
#include "LinkedList.h"

class GlowSpeck;
class PromptBox;
class GUIButton;
class GUITextbox;
class GUITexture;
class UserDisplay;

class MainOverlay : public GUIContainer {
public:
	MainOverlay();
	virtual ~MainOverlay();

	void draw();
	void tick(int);
	void keyPress(char c);

	void showOptions(int fadeDuration);
	void hideOptions(int fadeDuration);

	void hideMainContainer(int fadeDuration);
	void showMainContainer(int fadeDuration);

	void showCreditsContainer(int fadeDuration);
	void hideCreditsContainer(int fadeDuration);

	void hideAll(int fadeDuration);
protected:
private:
	GUIContainer* mainContainer_;
	GUIContainer* optionsContainer_;
	GUIContainer* creditsContainer_;
};