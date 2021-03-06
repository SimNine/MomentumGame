#include "Standard.h"

#include "Global.h"
#include "Message.h"
#include "Server.h"
#include "Pipe.h"
#include "MiscUtil.h"
#include "CommandProcessor.h"

// main function
int main(int argc, char* args[]) {
	logInit("log-server.txt", false);
	log("\n");
	log("-----SpybotServer-----\n");

	// initialize the server
	_server = new Server();
	log("SERVER MAIN: Server initialized\n");

	// start up the master listener thread
	std::thread masterListener(&Server::listenForNewConnections, _server);
	masterListener.detach();
	log("SERVER MAIN: Master listener set up and detached\n");

	// start up the command line thread
	std::thread commandLine(processCommandLoop);
	commandLine.detach();
	log("SERVER MAIN: Command line parser set up and detached\n");

	// tick the server continuously
	while (!_quit)
		_server->processAllMessages();

	return 0;
}