#pragma once

#include "Standard.h"
#include "LinkedList.h"

class PlayerMirror;

class PipeMirror {
public:
	PipeMirror();
	virtual ~PipeMirror();

	int clientID_;
	PlayerMirror* player_;
	std::string name_;
protected:

private:
};