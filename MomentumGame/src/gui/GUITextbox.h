#pragma once

#include "GUIObject.h"

class GUIButton;

class GUITextbox : public GUIObject {
public:
	GUITextbox(GUIContainer* parent, ANCHOR anchor, Coord disp, Coord dims, int maxChars, bool censored);
	~GUITextbox();

	// GUIObject overrides
	void setTransparency(int alpha);
	bool mouseDown();
	bool mouseUp();
	void keyPress(char);
	void draw();
	void tick(int ms);
	void resetBounds();

	std::string getContents();
	void clearContents();
private:
	std::string contentText_;
	int textSize_;
	int maxChars_;
	int cursorFade_;
	bool censored_;
};

