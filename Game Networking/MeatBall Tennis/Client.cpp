// Client.cpp : This file handles all client network functions.
#include "Client.h"
#include "../NetworkMessage.h"

// Initializes the client; connects to the server.
int Client::init(char* address, uint16_t port, uint8_t _player)
{
	state.player0.keyUp = state.player0.keyDown = false;
	state.player1.keyUp = state.player1.keyDown = false;
	state.gamePhase = WAITING;
	// TODO:
	//       1) Set the player.

	player = _player;

	if (INADDR_NONE == inet_addr(address))
		return ADDRESS_ERROR;


	clSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (clSocket == INVALID_SOCKET)
		return SETUP_ERROR;

	

	//       2) Set up the connection.
	
	
	name.sin_family = AF_INET;
	name.sin_addr.s_addr = inet_addr(address);
	name.sin_port = htons(port);



	//       3) Connect to the server.
	NetworkMessage messageoutput(_OUTPUT);
	messageoutput.writeByte(CL_CONNECT);
	messageoutput.writeByte(player);

	if (sendtoNetMessage(clSocket, messageoutput, &name) <= 0)
		return MESSAGE_ERROR;

	//       4) Get response from server.
	


	NetworkMessage messageinput(_INPUT);
	if (recvfromNetMessage(clSocket, messageinput, &name) <= 0)
	{
		int p = WSAGetLastError();
		return MESSAGE_ERROR;
	}
		
	
	SN = messageinput.readShort();
	uint8_t type = messageinput.readByte();
	//       5) Make sure to mark the client as running.
	switch (type)
	{
	case SV_OKAY:
	{
		active = true;
		return SUCCESS;
		break;
	}
	case SV_FULL:
	{
		active = false;
		return SHUTDOWN;
		break;
	}
	default:
		return MESSAGE_ERROR;
		break;
	}
}

// Receive and process messages from the server.
int Client::run()
{
	// TODO: Continuously process messages from the server aslong as the client running.
	// HINT: Set game phase to DISCONNECTED on SV_CL_CLOSE! (Try calling stop().)
	// HINT: You can keep track of the number of snapshots with a static variable...
	while (active)
	{
		NetworkMessage messageinput(_INPUT);
		if (recvfromNetMessage(clSocket, messageinput, &name) <= 0)
			return MESSAGE_ERROR;

		unsigned short SN2 = messageinput.readShort();
		uint8_t type = messageinput.readByte();

		if (SN < SN2)
		{
			SN = SN2;

			switch (type)
			{
			case SV_SNAPSHOT:
			{

				state.gamePhase = messageinput.readByte();

				state.ballX = messageinput.readShort();
				state.ballY = messageinput.readShort();
				state.player0.y = messageinput.readShort();
				state.player0.score = messageinput.readShort();
				state.player1.y = messageinput.readShort();
				state.player1.score = messageinput.readShort();

				count++;

				if (count >= 10)
				{
					sendAlive();
					count = 0;
				}
				break;
			}
			case SV_CL_CLOSE:
			{
				stop();
				return SHUTDOWN;
				break;
			}
			default:
				//return SHUTDOWN;
				break;
			}
		}
	}
}

// Clean up and shut down the client.
void Client::stop()
{
	// TODO:
	//       1) Make sure to send a SV_CL_CLOSE message.
	sendClose();
	//       2) Make sure to mark the client as shutting down and close the socket.
	active = false;
	closesocket(clSocket);
	//       3) Set the game phase to DISCONNECTED.
	state.gamePhase = DISCONNECTED;
}

// Send the player's input to the server.
int Client::sendInput(int8_t keyUp, int8_t keyDown, int8_t keyQuit)
{
	if (keyQuit)
	{
		stop();
		return SHUTDOWN;
	}

	cs.enter();
	if (player == 0)
	{
		state.player0.keyUp = keyUp;
		state.player0.keyDown = keyDown;
	}
	else
	{
		state.player1.keyUp = keyUp;
		state.player1.keyDown = keyDown;
	}
	cs.leave();

	//TODO:	Transmit the player's input status.

	NetworkMessage messageoutput(_OUTPUT);
	messageoutput.writeByte(CL_KEYS);
	messageoutput.writeByte(keyUp);
	messageoutput.writeByte(keyDown);

	if (sendtoNetMessage(clSocket, messageoutput, &name) <= 0)
		return MESSAGE_ERROR;

	return SUCCESS;
}

// Copies the current state into the struct pointed to by target.
void Client::getState(GameState* target)
{
	// TODO: Copy state into target.
	memcpy(&target->ballX, &state.ballX, sizeof(state.ballX));
	memcpy(&target->ballY, &state.ballY, sizeof(state.ballY));
	memcpy(&target->ballX, &state.ballX, sizeof(state.ballX));
	memcpy(&target->gamePhase, &state.gamePhase, sizeof(state.gamePhase));
	memcpy(&target->player0, &state.player0, sizeof(state.player0));
	memcpy(&target->player1, &state.player1, sizeof(state.player1));

}
// Sends a SV_CL_CLOSE message to the server (private, suggested)
void Client::sendClose()
{
	// TODO: Send a CL_CLOSE message to the server.
	NetworkMessage messageoutput(_OUTPUT);
	messageoutput.writeByte(SV_CL_CLOSE);

	sendtoNetMessage(clSocket, messageoutput, &name);
}

// Sends a CL_ALIVE message to the server (private, suggested)
int Client::sendAlive()
{
	// TODO: Send a CL_ALIVE message to the server.
	NetworkMessage messageoutput(_OUTPUT);
	messageoutput.writeByte(CL_ALIVE);

	if (sendtoNetMessage(clSocket, messageoutput, &name) <= 0)
		return MESSAGE_ERROR;

	return SUCCESS;
}
