#include "Standard.h"
#include "CommandProcessor.h"

#include <iostream>

#include "LinkedList.h"
#include "Global.h"
#include "MiscUtil.h"
#include "Server.h"
#include "Message.h"
#include "Player.h"
#include "Game.h"
#include "Pipe.h"

void processCommandResponse(std::string message, int clientID) {
	if (clientID == 0) {
		log(">> " + message + "\n");
	} else {
		Message m;
		m.type = MSGTYPE::CHAT;
		strcpy_s(m.text, DEFAULT_MSG_TEXTSIZE, message.c_str());
		_server->sendMessageToClient(m, clientID);
	}
}

void processCommand(std::string cmd, int clientID) {
	// tokenize (gotta add a newline because that's how the tokenizer works)
	char* tokens[1024];
	int numTokens = tokenize(tokens, (cmd + "\n").c_str(), ' ');

	// process tokens
	if (numTokens == 0) {
		processCommandResponse("no command entered", clientID);
	} else if (strcmp(tokens[0], "help") == 0) {
		processCommandResponse("Available commands:", clientID);
		processCommandResponse("-----", clientID);

		// client and player manipulation
		processCommandResponse("listclients", clientID);
		processCommandResponse("kick (clientID)", clientID);
		processCommandResponse("say (string)", clientID);
		processCommandResponse("listplayers", clientID);
		processCommandResponse("getowner", clientID);
		processCommandResponse("-----", clientID);

		// level manipulation
		processCommandResponse("startgame", clientID);
		processCommandResponse("save (filename)", clientID);
		processCommandResponse("settile (x) (y) (tiletype)", clientID);
		processCommandResponse("settilerect (x) (y) (width) (height) (tiletype)", clientID);
		processCommandResponse("-----", clientID);
	}

	// CLIENT AND PLAYER MANIPULATION 
	else if (strcmp(tokens[0], "listclients") == 0) {
		processCommandResponse("connected clientIDs:", clientID);
		Iterator<Pipe*> it = _server->getClientList()->getIterator();
		while (it.hasNext()) {
			Pipe* curr = it.next();
			if (curr->getUsername() != "") {
				processCommandResponse("-- client " + to_string(curr->getClientID()) + " : logged in as " + curr->getUsername(), clientID);
			} else {
				processCommandResponse("-- client " + to_string(curr->getClientID()) + " : not logged in", clientID);
			}
		}
	} else if (strcmp(tokens[0], "kick") == 0) {
		if (numTokens != 2) {
			processCommandResponse("wrong number of arguments: kick (clientID)", clientID);
			processCommandResponse("kick (clientID)", clientID);
			destroyTokens(tokens);
			return;
		}

		int clientID = atoi(tokens[1]);
		if (clientID == 0) {
			processCommandResponse("cannot kick clientID 0: is server", clientID);
			destroyTokens(tokens);
			return;
		}
		std::string str = "attempting to kick clientID " + to_string(clientID);
		processCommandResponse(str, clientID);

		Message m;
		m.type = MSGTYPE::LEAVE;
		m.clientID = clientID;
		_server->recieveFromPipe(m);
	} else if (strcmp(tokens[0], "say") == 0) {
		std::string ret = "";
		for (int i = 1; i < numTokens; i++)
			ret += std::string(tokens[i]) + " ";

		Message m;
		m.type = MSGTYPE::CHAT;
		m.clientID = 0;
		strncpy_s(m.text, DEFAULT_MSG_TEXTSIZE, ret.c_str(), DEFAULT_MSG_TEXTSIZE);

		Iterator<Pipe*> clientIt = _server->getClientList()->getIterator();
		while (clientIt.hasNext()) {
			clientIt.next()->sendToClient(m);
		}
	} else if (strcmp(tokens[0], "listplayers") == 0) {
		if (_server->getGame() == NULL) {
			processCommandResponse("cannot list players - no game currently in session", clientID);
			destroyTokens(tokens);
			return;
		}

		/*
		if (_server->getGame()->getAllTeams()->getLength() == 0) {
			processCommandResponse("there are no players", clientID);
			destroyTokens(tokens);
			return;
		}
		*/

		/*
		Iterator<Team*> itTeam = _server->getGame()->getAllTeams()->getIterator();
		while (itTeam.hasNext()) {
			Team* currTeam = itTeam.next();
			std::string str = "team " + to_string(currTeam->getTeamID()) + ":";
			processCommandResponse(str, clientID);

			Iterator<Player*> itPlayer = currTeam->getAllPlayers()->getIterator();
			while (itPlayer.hasNext()) {
				Player* pCurr = itPlayer.next();
				if (pCurr->getMind() == NULL) {
					std::string str2 = "-- player " + std::string(to_string(pCurr->getPlayerID()) + " : human");
					processCommandResponse(str2, clientID);
				} else {
					std::string str2 = "-- player " + std::string(to_string(pCurr->getPlayerID()) + " : AI");
					processCommandResponse(str2, clientID);
				}
			}
		}
		*/
	} else if (strcmp(tokens[0], "getowner") == 0) {
		Pipe* ownerClient = _server->getOwner();
		if (ownerClient == NULL) {
			processCommandResponse("there is no owner client", clientID);
		} else {
			std::string str = "the owner client is " + ownerClient->getClientID();
			processCommandResponse(str, clientID);
			if (ownerClient->getUsername() != "") {
				std::string loginStr = to_string(ownerClient->getClientID()) + " <logged in as " + ownerClient->getUsername() + ">";
				processCommandResponse(loginStr, clientID);
			}
		}
	}
	
	// LEVEL MANIPULATION
	else if (strcmp(tokens[0], "startgame") == 0) {
		Message m;
		m.type = MSGTYPE::STARTGAME;
		m.clientID = 0;
		_server->recieveFromPipe(m);
	} else if (strcmp(tokens[0], "save") == 0) {
		if (numTokens != 2) {
			processCommandResponse("wrong number of arguments: save (filename)", clientID);
			destroyTokens(tokens);
			return;
		}

		Game* g = _server->getGame();
		if (g == NULL) {
			processCommandResponse("there is currently no game", clientID);
			destroyTokens(tokens);
			return;
		}

		//g->saveLevel(std::string(tokens[1]));
	}
	
	// PRINTING AND DEBUGGING

	
	// MISC
	else {
		std::string str = "\"" + cmd + "\" is not a valid command";
		processCommandResponse(str, clientID);
		processCommandResponse("use \"help\" for a list of commands", clientID);
	}

	// destroy tokens
	destroyTokens(tokens);
}

void processCommandLoop() {
	while (true) {
		// get the line
		std::string buffer;
		getline(std::cin, buffer);

		// process it
		processCommand(buffer, 0);
	}

	log("SERVER: parser loop existed for some reason\n");
}