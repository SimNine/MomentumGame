#pragma once

#include "Standard.h"

struct Message;
class Player;

class Pipe {
public:
	Pipe(SOCKET socket);
	virtual ~Pipe();

	void listenToClientLoop();
	void sendToClient(Message m);
	void recieveFromClient(Message m);

	int getClientID();

	std::string getUsername();
	void setUsername(std::string user);

	void close();
	bool isClosed();
protected:

private:
	SOCKET socket_;
	int clientID_;

	std::string username_;

	bool closed_;
};