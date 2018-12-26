#pragma once

#include "Standard.h"
#include "GUIObject.h"

class GUIContainer;

class AchievementButton : public GUIObject {
public:
	AchievementButton(GUIContainer* parent, ANCHOR anchor, Coord displacement, ACHIEVEMENT achievement);
	virtual ~AchievementButton();

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
	ACHIEVEMENT achievement_;
	Uint8 mouseoverFade_;
};