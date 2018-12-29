#pragma once

#include "Standard.h"
#include "LinkedList.h"
#include "GameConfig.h"

class Pipe;
struct Message;
class Game;
class Player;

class Server {
public:
	Server();
	virtual ~Server();

	void processAllMessages();

	Pipe* connect(SOCKET clientSocket, bool isLocal);
	void disconnect(Pipe* client);

	void sendMessageToClient(Message message, int clientID);
	void sendMessageToAllClients(Message message);
	void sendMessageToAllClientsExcept(Message message, int clientID);
	void recieveFromPipe(Message message);

	LinkedList<Pipe*>* getClientList();

	Game* getGame();
	Pipe* getOwner();

	void pingSender();

	void listenForNewConnections();
protected:

private:
	// game configuration
	GameConfig config_;

	// game
	Game* game_;

	// clients
	Pipe* ownerClient_;
	LinkedList<Pipe*>* clients_;
	Pipe* getClientByID(int clientID);

	// message processing core
	void processMessage(Message* msg);
	LinkedList<Message*>* msgQueue_;

	// mutexes
	std::mutex msgMutex_;

	// message processing helpers
	void resyncClient(int clientID);
	void resyncAll();
	void quitAll();
};