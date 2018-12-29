#pragma once

#include "Standard.h"
#include "LinkedList.h"

struct Message;
class GameMirror;
class PlayerMirror;
class PipeMirror;
class ServerMirror;
class Pipe;

class Client {
public:
	Client();
	virtual ~Client();

	void processMessage(Message* m);
	void processAllMessages();

	PlayerMirror* getPlayer();
	void setPlayer(PlayerMirror* player);

	GameMirror* getGame();
	void setGame(GameMirror* game);

	PipeMirror* getMyClientMirror();
	void setMyClientMirror(PipeMirror* mirr);

	void connectToExternalServer(std::string IP);
	void connectToLocalServer();
	void disconnect();
	void listenToPipeLoop();

	void sendToPipe(Message message);
	void recieveFromPipe(Message message);

	void pingChecker();
	void resetPingCount();
protected:

private:
	// game entity and player entity (both NULL when no game is being played)
	GameMirror* game_;
	PlayerMirror* player_;

	// clientmirror entity
	PipeMirror* myClientMirror_;

	// server mirror entity
	ServerMirror* serverMirror_;

	// message passing utils
	LinkedList<Message*>* msgQueue_;
	std::mutex msgMutex_;

	// for keeping track of server survival
	int pingCount_ = 0;

	// for external server interation
	SOCKET socket_;

	// for internal server interaction
	Pipe* pipe_;
};