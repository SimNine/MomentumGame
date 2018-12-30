#include "Standard.h"
#include "Client.h"

#include "Message.h"
#include "Global.h"
#include "MiscUtil.h"
#include "NotifyOverlay.h"
#include "Resources.h"
#include "GameOverlay.h"
#include "MainOverlay.h"
#include "PipeMirror.h"
#include "BackgroundOverlay.h"
#include "Server.h"
#include "GameMirror.h"
#include "PlayerMirror.h"
#include "TimedEvent.h"
#include "Pipe.h"
#include "ServerMirror.h"

Client::Client() {
	msgQueue_ = new LinkedList<Message*>();

	// mirrors
	game_ = NULL;
	player_ = NULL;

	// for server connectivity
	socket_ = INVALID_SOCKET;
	pipe_ = NULL;
}

Client::~Client() {
	//dtor
}

void Client::processAllMessages() {
	while (msgQueue_->getLength() > 0) {
		msgMutex_.lock();
		Message* curr = msgQueue_->poll();
		msgMutex_.unlock();
		processMessage(curr);
		delete curr;
	}
}

void Client::processMessage(Message* msg) {
	if (msg->type != MSGTYPE::PING) {
		log("CLIENT RECIEVED MSG: ");
		_printMessage(*msg);
	}

	switch (msg->type) {
	case MSGTYPE::STARTGAME:
	{
		if (game_ != NULL)
			delete game_;

		game_ = new GameMirror(SDL_Rect{msg->pos.x, msg->pos.y, msg->pos2.x, msg->pos2.y});
	}
	break;
	case MSGTYPE::SOUND:
		switch (msg->num) {
		case MSGSOUNDNAME_MOVE:
			Mix_PlayChannel(-1, _sound_move_player, msg->num);
			break;
		case MSGSOUNDNAME_DAMAGE:
			Mix_PlayChannel(-1, _sound_action_attack, msg->num);
			break;
		case MSGSOUNDNAME_HEAL:
			Mix_PlayChannel(-1, _sound_action_heal, msg->num);
			break;
		case MSGSOUNDNAME_SIZEMOD:
			Mix_PlayChannel(-1, _sound_action_heal, msg->num);
			break;
		case MSGSOUNDNAME_SPEEDMOD:
			Mix_PlayChannel(-1, _sound_action_speed, msg->num);
			break;
		case MSGSOUNDNAME_ZERO:
			Mix_PlayChannel(-1, _sound_action_grid_damage, msg->num);
			break;
		case MSGSOUNDNAME_ONE:
			Mix_PlayChannel(-1, _sound_action_grid_fix, msg->num);
			break;
		case MSGSOUNDNAME_PICKUPCREDIT:
			Mix_PlayChannel(-1, _sound_pickup_credit, msg->num);
			break;
		default:
			break;
		}
		break;
	case MSGTYPE::INFO:
		switch ((MSGINFOTYPE)msg->subType) {
		case MSGINFOTYPE::PLATFORM_ADD:
			game_->addPlatform(SDL_Rect{ msg->pos.x, msg->pos.y, msg->pos2.x, msg->pos2.y });
			break;
		case MSGINFOTYPE::PLATFORM_REM:
			// TODO
			break;
		case MSGINFOTYPE::PLAYER_ADD:
			{
			// TODO: this is a temporary fix
				PlayerMirror* myPlayer = game_->addPlayer(msg->num);
				if (myClientMirror_->player_ == NULL)
					myClientMirror_->player_ = myPlayer;
			}
			break;
		case MSGINFOTYPE::PLAYER_REM:
			game_->removePlayer(msg->num);
			break;
		case MSGINFOTYPE::PLAYER_MOVE:
			{
				PlayerMirror* pm = game_->getPlayerByID(msg->num);
				pm->setPos(msg->pos);
			}
			break;
		}
		break;
	case MSGTYPE::SETCLIENTPLAYER:
	{
		// if this is my client's player
		if (msg->clientID == myClientMirror_->clientID_)
			player_ = game_->getPlayerByID(msg->clientID);

		// set this clientMirror's player
		PipeMirror* mirr = serverMirror_->getClientMirrorByClientID(msg->clientID);
		mirr->player_ = game_->getPlayerByID(msg->num);
	}
	break;
	case MSGTYPE::CONNECT:
	{
		log("CLIENT: Connection to server confirmed\n");
		log("CLIENT: Assigned client ID " + to_string(msg->clientID) + "\n");

		myClientMirror_ = new PipeMirror();
		myClientMirror_->clientID_ = pipe_->getClientID();

		if (_server == NULL) {
			//_mainOverlay->hideIPEntry(1000);
			//_mainOverlay->showLoginContainer(1000);
		}
	}
	break;
	case MSGTYPE::DISCONNECT:
		// TODO: have the pipe be destroyed
		break;
	case MSGTYPE::LEAVE:
		//if (_overlayStack->getFirst() == _lobbyOverlay) 
		{
			// find the index of this clientID
			int index = -1;
			for (int i = 0; i < serverMirror_->getClientList()->getLength(); i++) {
				if (serverMirror_->getClientList()->getObjectAt(i)->clientID_ == msg->clientID)
					index = i;
			}

			// if there's a game going on, remove the player
			//game_->removePlayer(clientList_->getObjectAt(index)->player_->getPlayerID());

			// remove the client
			_notifyOverlay->addNotification(serverMirror_->getClientList()->getObjectAt(index)->name_ + " has disconnected from the server");
			delete serverMirror_->getClientList()->removeObjectAt(index);
		}

		break;
	case MSGTYPE::CHAT:
	{
		std::string text = "";
		for (int i = 0; i < serverMirror_->getClientList()->getLength(); i++) {
			if (serverMirror_->getClientList()->getObjectAt(i)->clientID_ == msg->clientID)
				text += serverMirror_->getClientList()->getObjectAt(i)->name_;
		}
		text += "> " + std::string(msg->text);

		/*
		if (_overlayStack->getFirst() == _lobbyOverlay)
			_lobbyOverlay->getChatDisplay()->addMessage(text);
		else if (_overlayStack->getFirst() == _gameOverlay)
			_gameOverlay->getChatDisplay()->addMessage(text);
		*/
	}
	break;
	case MSGTYPE::ERR:
		_notifyOverlay->addNotification(std::string(msg->text));
		break;
	case MSGTYPE::PING:
		resetPingCount();
		break;
	}
}

PlayerMirror* Client::getPlayer() {
	return player_;
}

void Client::setPlayer(PlayerMirror* player) {
	player_ = player;
}

GameMirror* Client::getGame() {
	return game_;
}

void Client::setGame(GameMirror* game) {
	game_ = game;
}

PipeMirror* Client::getMyClientMirror() {
	return myClientMirror_;
}

void Client::setMyClientMirror(PipeMirror* mirr) {
	myClientMirror_ = mirr;
}

void Client::connectToLocalServer() {
	// disconnect in case there is still some connection
	disconnect();

	// create a new local server and connect to it
	_server = new Server();
	pipe_ = _server->connect(NULL, true);
}

void Client::connectToExternalServer(std::string IP) {
	// disconnect in case there is still some connection
	disconnect();

	struct addrinfo hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	int iResult;
	struct addrinfo *result = NULL;
	iResult = getaddrinfo(IP.c_str(), DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		log("CLIENT ERR: getaddrinfo failed with error: " + to_string(iResult) + "\n");
	}

	// Attempt to connect to an address until one succeeds
	struct addrinfo* ptr = NULL;
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
		// Create a SOCKET for connecting to server
		socket_ = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (socket_ == INVALID_SOCKET) {
			log("CLIENT ERR: socket failed with error: " + to_string(WSAGetLastError()) + "\n");
		}

		// Connect to server.
		iResult = connect(socket_, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(socket_);
			socket_ = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (socket_ == INVALID_SOCKET) {
		log("CLIENT ERR: Unable to connect to server!\n");
	}

	// Receive until the peer closes the connection
	log("CLIENT: attempting to connect to server \"" + IP + "\"\n");
	std::thread newThread(&Client::listenToPipeLoop, this);
	newThread.detach();

	// start ping checker
	std::thread pingChecker(&Client::pingChecker, this);
	pingChecker.detach();
}

void Client::disconnect() {
	if (socket_ != INVALID_SOCKET) { // disconnect from external server
		if (shutdown(socket_, SD_SEND) == SOCKET_ERROR) {
			log("CLIENT ERR: shutdown failed with error: " + to_string(WSAGetLastError()) + "\n");
		}
		closesocket(socket_);
		socket_ = INVALID_SOCKET;
	}
	else if (_server != NULL) { // disconnect from internal server
		delete _server;
		_server = NULL;
	}

	// clean up
	while (msgQueue_->getLength() > 0)
		delete msgQueue_->poll();
	delete serverMirror_;
	serverMirror_ = NULL;
	delete myClientMirror_;
	myClientMirror_ = NULL;

	_notifyOverlay->addNotification("Disconnected from server");
	_overlayStack->removeAll();
	_overlayStack->push(_backgroundOverlay);
	_overlayStack->push(_mainOverlay);
}

void Client::listenToPipeLoop() {
	int bytesRead;

	char readBuffer[DEFAULT_MSG_SIZE];
	int readBufferLength = DEFAULT_MSG_SIZE;

	// listen for the first message, which should be a connection confirmation
	bytesRead = recv(socket_, readBuffer, readBufferLength, 0);
	if (bytesRead > 0) {
		Message m = _deserializeMessage(readBuffer);
		if (m.type == MSGTYPE::CONNECT) {
			// TODO: have a new PipeMirror be created
			//_client->setClientID(m.clientID);
			//strncpy_s(m.text, DEFAULT_MSG_TEXTSIZE, username.c_str(), DEFAULT_MSG_TEXTSIZE);
			log("CLIENT: received connection confirmation from server, acquired client ID " + to_string(myClientMirror_->clientID_) + "\n");
			recieveFromPipe(m);
			sendToPipe(m);
		}
		else {
			log("CLIENT ERR: first message recieved from server was not connection confirmation\n");
			disconnect();
			return;
		}
	}
	else if (bytesRead == 0) {
		log("CLIENT: recieved shutdown from server\n");
	}
	else {
		log("CLIENT ERR: recieve from server failed with error: " + to_string(WSAGetLastError()) + "\n");
	}

	// receive messages until the peer shuts down the connection or there's a read error
	do {
		bytesRead = recv(socket_, readBuffer, readBufferLength, 0);
		if (bytesRead > 0) {
			Message m = _deserializeMessage(readBuffer);
			recieveFromPipe(m);
		}
		else if (bytesRead == 0)
			log("CLIENT: recieved shutdown from server\n");
		else {
			log("CLIENT ERR: recieve from server failed with error: " + to_string(WSAGetLastError()) + "\n");
		}
	} while (bytesRead > 0);

	disconnect();
	return;
}

void Client::recieveFromPipe(Message message) {
	msgMutex_.lock();
	msgQueue_->addLast(new Message(message));
	msgMutex_.unlock();
}

void Client::sendToPipe(Message m) {
	if (socket_ != INVALID_SOCKET) { // if server is external
		char buffer[DEFAULT_MSG_SIZE];
		_serializeMessage(buffer, m);

		int bytesSent = send(socket_, buffer, DEFAULT_MSG_SIZE, 0);
		if (bytesSent == SOCKET_ERROR) {
			log("CLIENT ERR: send failed with error: " + to_string(WSAGetLastError()) + "\n");
			closesocket(socket_);
		}
	}
	else if (_server != NULL) { // if server is local
		pipe_->recieveFromClient(m);
	}
}

void Client::pingChecker() {
	log("CLIENT: pingChecker launched\n");

	while (_server != NULL || socket_ != INVALID_SOCKET) {
		pingCount_++;

		if (pingCount_ > 5) {
			log("CLIENT WARNING: ping count is " + to_string(pingCount_) + "\n");
		}

		Sleep(1000);
	}

	log("CLIENT: pingChecker exited\n");
}

void Client::resetPingCount() {
	pingCount_ = 0;
}