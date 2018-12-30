#include "Standard.h"
#include "Server.h"

#include <iostream>
#include <string>

#include "Game.h"
#include "Message.h"
#include "MiscUtil.h"
#include "Player.h"
#include "Pipe.h"
#include "CommandProcessor.h"

Server::Server() {
	game_ = NULL;
	ownerClient_ = NULL;

	clients_ = new LinkedList<Pipe*>();
	msgQueue_ = new LinkedList<Message*>();

	std::thread pingThread(&Server::pingSender, this);
	pingThread.detach();
}

Server::~Server() {
	if (game_ != NULL)
		delete game_;

	while (clients_->getLength() > 0)
		delete clients_->poll();
	delete clients_;

	while (msgQueue_->getLength() > 0)
		delete msgQueue_->poll();
	delete msgQueue_;
}

void Server::processAllMessages() {
	msgMutex_.lock();
	while (msgQueue_->getLength() > 0) {
		Message* m = msgQueue_->removeFirst();
		processMessage(m);
		delete m;
	}
	msgMutex_.unlock();
}

void Server::sendMessageToAllClients(Message message) {
	Iterator<Pipe*> it = clients_->getIterator();
	while (it.hasNext()) {
		Pipe* curr = it.next();
		curr->sendToClient(message);
	}
}

void Server::sendMessageToClient(Message message, int clientID) {
	Pipe* curr = getClientByID(clientID);
	if (curr->getUsername() != "")
		curr->sendToClient(message);
}

void Server::sendMessageToAllClientsExcept(Message message, int clientID) {
	Iterator<Pipe*> it = clients_->getIterator();
	while (it.hasNext()) {
		Pipe* curr = it.next();
		if (curr->getClientID() != clientID && curr->getUsername() != "")
			curr->sendToClient(message);
	}
}

Pipe* Server::getClientByID(int clientID) {
	Iterator<Pipe*> it = clients_->getIterator();
	while (it.hasNext()) {
		Pipe* curr = it.next();
		if (curr->getClientID() == clientID) {
			return curr;
		}
	}

	// if a client with a matching ID wasn't found, return null
	return NULL;
}

void Server::recieveFromPipe(Message message) {
	msgMutex_.lock();
	msgQueue_->addLast(new Message(message));
	msgMutex_.unlock();
}

void Server::processMessage(Message* msg) {
	if (msg->type != MSGTYPE::PING) {
		log("SERVER RECIEVED MSG: ");
		_printMessage(*msg);
	}

	// get the client of the issuer
	Pipe* issuingClient = getClientByID(msg->clientID);
	if (issuingClient == NULL && msg->clientID != 0) {
		log("SERVER ERR: got a message from a client that doesn't exist\n");
		return;
	}

	// process the incoming message and then send it to all clients
	// assuming the client is logged in or is the server
	switch (msg->type) {
	case MSGTYPE::CHAT:
		// check if usable as a command
		if ((msg->text[0] == '/' && issuingClient == ownerClient_) || issuingClient == NULL) {
			std::thread doCommand(processCommand, std::string(&msg->text[1]), msg->clientID);
			doCommand.detach();
		} else
			// forward to all clients
			sendMessageToAllClients(*msg);
		break;
	case MSGTYPE::CONNECT:
		// put the clientID back
		msg->clientID = msg->num;

		// if there is no owner client, make this it
		if (ownerClient_ == NULL)
			ownerClient_ = getClientByID(msg->clientID);

		// set this client's name
		// getClientByID(msg->clientID)->setName(msg->text);

		// send a connect message for the new client to all *other* clients
		sendMessageToAllClientsExcept(*msg, msg->clientID);

		// send the issuing client a message for every client currently connected
		{
			Iterator<Pipe*> it = clients_->getIterator();
			while (it.hasNext()) {
				Pipe* curr = it.next();

				// send the newly connected client a list of all clients
				Message m;
				m.type = MSGTYPE::CONNECT;
				m.clientID = curr->getClientID();
				if (curr == ownerClient_)
					m.num = 9000; // an arbitrary value to indicate this is the owner client
				else
					m.num = 0;
				sendMessageToClient(m, msg->clientID);
			}
		}
		break;
	case MSGTYPE::INFO:
		if (msg->subType == MSGINFOTYPE::PLAYER_MOVE) {
			Player* p = game_->getPlayerByID(msg->num);
			game_->displacePlayer(p, msg->pos);
		}
		else if (msg->subType == MSGINFOTYPE::PLAYER_ADD) {
			game_->addPlayer();
		}
		break;
	case MSGTYPE::STARTGAME:
		if (game_ != NULL)
			delete game_;

		game_ = new Game();
		SDL_Rect gBounds = game_->getLevelBounds();

		Message m;
		m.type = MSGTYPE::STARTGAME;
		m.pos = Coord{ gBounds.x, gBounds.y };
		m.pos2 = Coord{ gBounds.w, gBounds.h };
		sendMessageToAllClients(m);

		game_->addPlatform(SDL_Rect{ 100, 100, 400, 200 });
		game_->addPlatform(SDL_Rect{ 800, 100, 200, 400 });

		game_->addPlayer();
		game_->addPlayer();

		break;
	case MSGTYPE::RESYNCGAME:
		resyncClient(msg->clientID);
		break;
	case MSGTYPE::LEAVE:
	{
		// this case is triggered ONLY if the client voluntarily and intentionally disconnects
		// do NOT do anything else in this case
		disconnect(issuingClient);
		break;
	}
	default:
		break;
	}
}

/* 
Creates a new pipe and adds it to the server
*/
Pipe* Server::connect(SOCKET client, bool isLocal) {
	Pipe* newPipe = new Pipe(client);
	clients_->addFirst(newPipe);

	if (!isLocal) {
		std::thread newThread(&Pipe::listenToClientLoop, newPipe);
		newThread.detach();
	}

	return newPipe;
}

void Server::disconnect(Pipe* client) {
	if (!client->isClosed()) {
		// kill the thread formally so that death can begin; the pipe will call this method again
		client->close();
		return;
	}

	// let the server know
	log("SERVER: Client " + to_string(client->getClientID()) + " disconnected\n");

	// remove client from list
	clients_->remove(client);

	// if this client was the owner
	if (client == ownerClient_) {
		if (clients_->getLength() == 0)
			ownerClient_ = NULL;
		else
			ownerClient_ = clients_->getFirst();
	}

	// remove the player from the game
	/*
	if (game_ != NULL) {
		Player* p = game_->getPlayerByID(client->getPlayer());
		Team* t = game_->getTeamByID(p->getTeam());
		t->getAllPlayers()->remove(p);
		delete p;
	}
	*/

	// send leave message to all connected clients
	Message msg;
	msg.type = MSGTYPE::LEAVE;
	msg.clientID = client->getClientID();
	sendMessageToAllClients(msg);
}

void Server::quitAll() {
	Iterator<Pipe*> it = clients_->getIterator();
	while (it.hasNext()) {
		Pipe* curr = it.next();

		Message m;
		m.type = MSGTYPE::LEAVE;
		m.clientID = curr->getClientID();
		sendMessageToAllClients(m);
	}
}

void Server::resyncAll() {
	Iterator<Pipe*> it = clients_->getIterator();
	while (it.hasNext()) {
		resyncClient(it.next()->getClientID());
	}
}

void Server::resyncClient(int clientID) {
	// initialize message
	Message msg;
	msg.clientID = 0;

	// send all platforms
}

LinkedList<Pipe*>* Server::getClientList() {
	return clients_;
}

Game* Server::getGame() {
	return game_;
}

Pipe* Server::getOwner() {
	return ownerClient_;
}

void Server::pingSender() {
	log("SERVER: pinger set up and detached\n");

	// continue until client list is null (meaning the server has been deleted)
	while (true) {
		if (clients_ == NULL)
			break;

		Message m;
		m.type = MSGTYPE::PING;
		sendMessageToAllClients(m);

		Sleep(1000);
	}

	log("SERVER: pinger exited\n");
}

// main loop to recieve new sockets
void Server::listenForNewConnections() {
	WSADATA wsaData;
	int iResult;

	SOCKET ListenSocket = INVALID_SOCKET;

	struct addrinfo *result = NULL;
	struct addrinfo hints;

	int recvbuflen = DEFAULT_BUFLEN;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		log("WSAStartup failed with error: " + to_string(iResult) + "\n");
		exit(1);
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		log("getaddrinfo failed with error: " + to_string(iResult) + "\n");
		WSACleanup();
		exit(1);
	}

	// Create a SOCKET for connecting to server
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		log("socket failed with error: " + to_string(WSAGetLastError()) + "\n");
		freeaddrinfo(result);
		WSACleanup();
		exit(1);
	}

	// Setup the TCP listening socket
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		log("bind failed with error: " + to_string(WSAGetLastError()) + "\n");
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		exit(1);
	}

	freeaddrinfo(result);

	// listen for incoming connections
	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		log("listen failed with error: " + to_string(WSAGetLastError()) + "\n");
		closesocket(ListenSocket);
		WSACleanup();
		exit(1);
	}

	// main master listener loop
	while (true) {
		// attempt to accept a client socket
		SOCKET newSock = accept(ListenSocket, NULL, NULL);
		if (newSock == INVALID_SOCKET) {
			log("accept failed with error: " + to_string(WSAGetLastError()) + "\n");
			closesocket(ListenSocket);
			WSACleanup();
			exit(1);
		}
		else
			connect(newSock, false);
	}
}