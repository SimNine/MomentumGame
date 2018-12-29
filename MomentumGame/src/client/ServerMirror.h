#pragma once

#include "Standard.h"
#include "LinkedList.h"

class PipeMirror;

class ServerMirror {
public:
	ServerMirror();
	virtual ~ServerMirror();

	PipeMirror* getClientMirrorByClientID(int clientID);
	PipeMirror* getClientMirrorByPlayerID(int playerID);
	LinkedList<PipeMirror*>* getClientList();
	PipeMirror* getServerOwner();
	void setServerOwner(PipeMirror* clientMirror);
protected:

private:
	LinkedList<PipeMirror*>* clientList_;
	PipeMirror* serverOwner_;
};