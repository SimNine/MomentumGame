#include "Standard.h"
#include "ServerMirror.h"

#include "Message.h"
#include "Global.h"
#include "MiscUtil.h"
#include "Resources.h"
#include "PipeMirror.h"
#include "PlayerMirror.h"
#include "Server.h"
#include "Client.h"
#include "Pipe.h"
#include "NotifyOverlay.h"
#include "BackgroundOverlay.h"
#include "MainOverlay.h"

ServerMirror::ServerMirror() {
	clientList_ = new LinkedList<PipeMirror*>();
	serverOwner_ = NULL;
}

ServerMirror::~ServerMirror() {
	//dtor
}

LinkedList<PipeMirror*>* ServerMirror::getClientList() {
	return clientList_;
}

PipeMirror* ServerMirror::getClientMirrorByClientID(int clientID) {
	Iterator<PipeMirror*> it = clientList_->getIterator();
	while (it.hasNext()) {
		PipeMirror* curr = it.next();
		if (curr->clientID_ == clientID)
			return curr;
	}

	return NULL;
}

PipeMirror* ServerMirror::getClientMirrorByPlayerID(int playerID) {
	Iterator<PipeMirror*> it = clientList_->getIterator();
	while (it.hasNext()) {
		PipeMirror* curr = it.next();
		if (curr->player_->getPlayerID() == playerID)
			return curr;
	}

	return NULL;
}

PipeMirror* ServerMirror::getServerOwner() {
	return serverOwner_;
}

void ServerMirror::setServerOwner(PipeMirror* serverOwner) {
	serverOwner_ = serverOwner;
}