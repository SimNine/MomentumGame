#pragma once

#include "Standard.h"
#include "GUIObject.h"

class GUIContainer;

class GUISlider : public GUIObject {
public:
	GUISlider(GUIContainer* parent, ANCHOR anchor, Coord disp, Coord dims, void(*func) (float), float initValue);
	virtual ~GUISlider();

	// GUIObject overrides
	void setTransparency(int);
	bool mouseDown();
	bool mouseUp();
	void keyPress(char);
	void draw();
	void tick(int);
	void resetBounds();
protected:
private:
	SDL_Rect sliderBounds_;
	int sliderVal_;
	void(*func) (float);
};