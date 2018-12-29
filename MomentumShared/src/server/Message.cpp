#include "Message.h"

#include "Logger.h"
#include "MiscUtil.h"

void _serializeMessage(char* buffer, Message m) {
	int currByte = 0;

	// write type and subtype
	buffer[currByte] = m.type;
	currByte++;
	buffer[currByte] = m.subType;
	currByte++;

	// write IDs
	serializeInt(&buffer[currByte], m.clientID);
	currByte += 4;

	// write general-purpose coords
	serializeInt(&buffer[currByte], m.pos.x);
	currByte += 4;
	serializeInt(&buffer[currByte], m.pos.y);
	currByte += 4;
	serializeInt(&buffer[currByte], m.pos2.x);
	currByte += 4;
	serializeInt(&buffer[currByte], m.pos2.y);
	currByte += 4;

	// write general-purpose ints
	serializeInt(&buffer[currByte], m.num);
	currByte += 4;
	serializeInt(&buffer[currByte], m.num2);
	currByte += 4;

	// write text
	m.text[DEFAULT_MSG_TEXTSIZE - 1] = '\0';
	strcpy_s(&buffer[currByte], DEFAULT_MSG_TEXTSIZE, m.text);
	currByte += DEFAULT_MSG_TEXTSIZE;
}

Message _deserializeMessage(char* in) {
	Message m;
	int currByte = 0;

	// read type
	m.type = (MSGTYPE)in[currByte];
	currByte++;
	m.subType = in[currByte];
	currByte++;

	// read IDs
	deserializeInt(&in[currByte], &m.clientID);
	currByte += 4;

	// read general-purpose coords
	deserializeInt(&in[currByte], &m.pos.x);
	currByte += 4;
	deserializeInt(&in[currByte], &m.pos.y);
	currByte += 4;
	deserializeInt(&in[currByte], &m.pos2.x);
	currByte += 4;
	deserializeInt(&in[currByte], &m.pos2.y);
	currByte += 4;

	// read general-purpose ints
	deserializeInt(&in[currByte], &m.num);
	currByte += 4;
	deserializeInt(&in[currByte], &m.num2);
	currByte += 4;

	// read text
	strcpy_s(m.text, DEFAULT_MSG_TEXTSIZE, &in[currByte]);
	currByte += DEFAULT_MSG_TEXTSIZE;

	// return it
	return m;
}

void _printMessage(Message m) {
	log(to_string(m.clientID) + " - ");

	switch (m.type) {
	case MSGTYPE::ACTION:
		log("MSGTYPE::ACTION\n");
		break;
	case MSGTYPE::AISTEP:
		log("MSGTYPE::AISTEP\n");
		break;
	case MSGTYPE::CLEAR:
		log("MSGTYPE::CLEAR\n");
		break;
	case MSGTYPE::NEXTTURN:
		log("MSGTYPE::NEXTTURN\n");
		break;
	case MSGTYPE::INFO:
		log("MSGTYPE::INFO - ");

		switch ((MSGINFOTYPE)m.subType) {
		case MSGINFOTYPE::PLATFORM_ADD:
			log("MSGINFOTYPE::PLATFORM_ADD - TEAM " + to_string(m.num) + "\n");
			break;
		case MSGINFOTYPE::PLAYER_ADD:
			log("MSGINFOTYPE::PLAYER_ADD - PLAYER " + to_string(m.num) + "\n");
			break;
		case MSGINFOTYPE::PLATFORM_REM:
			log("MSGINFOTYPE::PLATFORM_REM - TEAM " + to_string(m.num) + "\n");
			break;
		case MSGINFOTYPE::PLAYER_REM:
			log("MSGINFOTYPE::PLAYER_REM - PLAYER " + to_string(m.num) + "\n");
			break;

		case MSGINFOTYPE::PLAYER_MOVE:
			log("MSGINFOTYPE::PLAYER_MOVE - PLAYER " + to_string(m.num) + " to " + to_string(m.pos.x) + "," + to_string(m.pos.y) + "\n");
			break;
		}

		break;
	case MSGTYPE::INQUIRY:
		log("MSGTYPE::INQUIRY\n");
		break;
	case MSGTYPE::SETCLIENTPLAYER:
		log("MSGTYPE::SETCLIENTPLAYER - CLIENT " + to_string(m.clientID) + " - PLAYER " + to_string(m.num) + "\n");
		break;
	case MSGTYPE::LEAVE:
		log("MSGTYPE::LEAVE\n");
		break;
	case MSGTYPE::STARTGAME:
		log("MSGTYPE::STARTGAME\n");
		break;
	case MSGTYPE::DONELOAD:
		log("MSGTYPE::DONELOAD\n");
		break;
	case MSGTYPE::MOVE:
		log("MSGTYPE::MOVE\n");
		break;
	case MSGTYPE::NONE:
		log("MSGTYPE::NONE\n");
		break;
	case MSGTYPE::RESYNCGAME:
		log("MSGTYPE::RESYNCGAME\n");
		break;
	case MSGTYPE::SELECT:
		log("MSGTYPE::SELECT - ");
		switch ((MSGSELECTTYPE)m.subType) {
		case MSGSELECTTYPE_TILE:
			log("TILE (" + to_string(m.pos.x) + "," + to_string(m.pos.y) + ")\n");
			break;
		case MSGSELECTTYPE_PROGRAM:
			log("PROGRAM " + to_string(m.num) + "\n");
			break;
		case MSGSELECTTYPE_ACTION:
			log("ACTION " + to_string(m.num) + "\n");
			break;
		}
		break;
	case MSGTYPE::SOUND:
		log("MSGTYPE::SOUND\n");
		break;
	case MSGTYPE::CONNECT:
		log("MSGTYPE::CONNECT\n");
		break;
	case MSGTYPE::DISCONNECT:
		log("MSGTYPE::DISCONNECT\n");
		break;
	case MSGTYPE::TEXT:
		log("MSGTYPE::TEXT - " + std::string(m.text) + "\n");
		break;
	case MSGTYPE::GAMECONFIG:
		log("MSGTYPE::GAMECONFIG - ");
		switch ((MSGGAMECONFIGTYPE)m.subType) {
		case MSGGAMECONFIGTYPE_CAMPAIGN:
			log("MSGGAMECONFIGTYPE_CAMPAIGN\n");
			break;
		case MSGGAMECONFIGTYPE_COOP:
			log("MSGGAMECONFIGTYPE_COOP\n");
			break;
		case MSGGAMECONFIGTYPE_FFA:
			log("MSGGAMECONFIGTYPE_FFA\n");
			break;
		case MSGGAMECONFIGTYPE_TEAMDM:
			log("MSGGAMECONFIGTYPE_TEAMDM\n");
			break;
		case MSGGAMECONFIGTYPE_LEVEL_ARRAY:
			log("MSGGAMECONFIGTYPE_LEVEL_ARRAY\n");
			break;
		case MSGGAMECONFIGTYPE_LEVEL_CROSS:
			log("MSGGAMECONFIGTYPE_LEVEL_CROSS\n");
			break;
		case MSGGAMECONFIGTYPE_LEVEL_HASH:
			log("MSGGAMECONFIGTYPE_LEVEL_HASH\n");
			break;
		case MSGGAMECONFIGTYPE_LEVEL_SHOWDOWN:
			log("MSGGAMECONFIGTYPE_LEVEL_SHOWDOWN\n");
			break;
		case MSGGAMECONFIGTYPE_LEVEL_NUMBERED:
			log("MSGGAMECONFIGTYPE_LEVEL_NUMBERED - " + to_string(m.num) + "\n");
			break;
		}
		break;
	case MSGTYPE::LOGIN:
		log("MSGTYPE::LOGIN - " + std::string(m.text) + "\n");
		break;
	case MSGTYPE::CREATEUSER:
		log("MSGTYPE::CREATEUSER - " + std::string(m.text) + "\n");
		break;
	case MSGTYPE::ERR:
		log("MSGTYPE::ERROR - " + std::string(m.text) + "\n");
		break;
	case MSGTYPE::PLACEPROG:
		log("MSGTYPE::PLACEPROG\n");
		break;
	case MSGTYPE::PROGINVENTORY:
		log("MSGTYPE::PROGINVENTORY - TYPE " + to_string(m.num) + ", COUNT " + to_string(m.num2) + "\n");
		break;
	case MSGTYPE::CREDITPICKUP:
		log("MSGTYPE::CREDITPICKUP - CREDITS " + to_string(m.num) + "\n");
		break;
	case MSGTYPE::LEVELUNLOCK:
		log("MSGTYPE::LEVELUNLOCK - LEVEL " + to_string(m.num) + "\n");
		break;
	case MSGTYPE::PING:
		log("MSGTYPE::PING\n");
		break;
	}
}