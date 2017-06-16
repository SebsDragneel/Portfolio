#pragma once
#include "ChatLobby.h"//Interface With Game Functionality
#include "NetDefines.h"
#include <stdint.h>//specifided data length types included (uint16_t)
#include <map>
#include <vector>
#include <string>

class TCPChatServer
{
	struct user
	{
		SOCKET mSocket = INVALID_SOCKET;
		uint8_t ID;
		std::string name;
		/*std::string buffer;*/
	};

	ChatLobby& chat_interface;//For making calls to add/remove users and display text

	SOCKET ASocket;
	SOCKET LSocket;

	std::vector<user> UserList;
	std::vector<UINT32> AvIDList;


public:
	TCPChatServer(ChatLobby& chat_int);
	~TCPChatServer(void);
	//Establishes a listening socket, Only Called once in Separate Server Network Thread, Returns true aslong as listening can and has started on the proper address and port, otherwise return false
	bool init(uint16_t port);
	//Recieves data from clients, parses it, responds accordingly, Only called in Separate Server Network Thread, Will be continuously called until return = false;
	bool run(void);
	//Notfies clients of close & closes down sockets, Can be called on multiple threads
	bool stop(void);
};
