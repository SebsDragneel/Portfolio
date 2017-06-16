// Server.cpp : Contains all functions of the server.
#include "Server.h"

// Initializes the server. (NOTE: Does not wait for player connections!)
int Server::init(uint16_t port)
{
	initState();

	// TODO:
	//       1) Set up a socket for listening.
	svSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (svSocket == INVALID_SOCKET)
		return SETUP_ERROR;

	name.sin_family = AF_INET;
	name.sin_addr.s_addr = INADDR_ANY;
	name.sin_port = htons(port);

	if (bind(svSocket, (sockaddr*)&name, sizeof(name)) == SOCKET_ERROR)
		return BIND_ERROR;

	unsigned long nolock = 1;

	if (ioctlsocket(svSocket, FIONBIO, &nolock) < 0)
		return WSAGetLastError();

	//       2) Mark the server as active.

	active = true;

	sequence[0] = 0;
	sequence[1] = 0;

	return SUCCESS;

}

// Updates the server; called each game "tick".
int Server::update()
{

	if (!active)
		return SHUTDOWN;

	NetworkMessage messageinput = NetworkMessage(_INPUT);
	sockaddr_in name;

	int result = recvfromNetMessage(svSocket, messageinput, &name);

	if (result < 0 && !EWOULDBLOCK)
		return DISCONNECT;

	if (!(result == -1 && EWOULDBLOCK))
	{
		int result2 = parseMessage(name, messageinput);

		if (result2 < 0)
			return WSAGetLastError();
	}

	if (playerTimer[0] > 50)
	{
		
		disconnectClient(0);
		playerTimer[0] = 0;
		playerTimer[1] = 0;
	}
		

	if (playerTimer[1] > 50)
	{
		disconnectClient(1);
		playerTimer[0] = 0;
		playerTimer[1] = 0;
	}

	for (int i = 0; i < noOfPlayers; i++)
		sequence[i]++;

	updateState();

	sendState();


	return SUCCESS;
}

// Stops the server.
void Server::stop()
{
	// TODO:
	//       1) Sends a "close" message to each client.
	sendClose();
	//       2) Shuts down the server gracefully (update method should exit with SHUTDOWN code.)
	active = false;
	closesocket(svSocket);
}

// Parses a message and responds if necessary. (private, suggested)
int Server::parseMessage(sockaddr_in& source, NetworkMessage& message)
{
	// TODO: Parse a message from client "source."
	
	uint8_t type = message.readByte();

	if (type == CL_CONNECT)
	{
		if (noOfPlayers < 2)
		{
			sendOkay(source);

			if (noOfPlayers == 0)
				connectClient(0, source);
			else
				connectClient(1, source);
		}
		else
			sendFull(source);
	}
	else
	{
		if (source.sin_port == playerAddress[0].sin_port &&
			source.sin_addr.s_addr == playerAddress[0].sin_addr.s_addr)
		{
			if (type == CL_KEYS)
			{
				state.player0.keyUp = message.readByte();
				state.player0.keyDown = message.readByte();
			}
			if (type == CL_ALIVE)
				playerTimer[0] = 0;
			if (type == SV_CL_CLOSE)
			{
				disconnectClient(0);
				playerTimer[0] = 0;
				playerTimer[1] = 0;

			}
		}
		else
		{
			if (type == CL_KEYS)
			{
				state.player1.keyUp = message.readByte();
				state.player1.keyDown = message.readByte();
			}
			if (type == CL_ALIVE)
				playerTimer[1] = 0;
			if (type == SV_CL_CLOSE)
			{
				disconnectClient(1);
				playerTimer[0] = 0;
				playerTimer[1] = 0;
			}
		}
	}
	return SUCCESS;
}

// Sends the "SV_OKAY" message to destination. (private, suggested)
int Server::sendOkay(sockaddr_in& destination)
{
	// TODO: Send "SV_OKAY" to the destination.

	NetworkMessage messageoutput = NetworkMessage(_OUTPUT);

	if (destination.sin_port == playerAddress[0].sin_port &&
		destination.sin_addr.s_addr == playerAddress[0].sin_addr.s_addr)
		messageoutput.writeShort(sequence[0]);
	else
		messageoutput.writeShort(sequence[1]);

	messageoutput.writeByte(SV_OKAY);
	sendtoNetMessage(svSocket, messageoutput, &destination);

	return SUCCESS;
}

// Sends the "SV_FULL" message to destination. (private, suggested)
int Server::sendFull(sockaddr_in& destination)
{
	// TODO: Send "SV_FULL" to the destination.

	NetworkMessage messageoutput = NetworkMessage(_OUTPUT);

	if (destination.sin_port == playerAddress[0].sin_port &&
		destination.sin_addr.s_addr == playerAddress[0].sin_addr.s_addr)
		messageoutput.writeShort(sequence[0]);
	else
		messageoutput.writeShort(sequence[1]);

	messageoutput.writeByte(SV_FULL);
	sendtoNetMessage(svSocket, messageoutput, &destination);

	return SUCCESS;
}

// Sends the current snapshot to all players. (private, suggested)
int Server::sendState()
{
	// TODO: Send the game state to each client.

	for (int i = 0; i < noOfPlayers; i++)
	{
		NetworkMessage message = NetworkMessage(_OUTPUT);
		message.writeShort(sequence[i]);
		message.writeByte(SV_SNAPSHOT);
		message.writeByte(state.gamePhase);
		message.writeShort(state.ballX);
		message.writeShort(state.ballY);
		message.writeShort(state.player0.y);
		message.writeShort(state.player0.score);
		message.writeShort(state.player1.y);
		message.writeShort(state.player1.score);
		sendtoNetMessage(svSocket, message, &playerAddress[i]);
	}


	return SUCCESS;
}

// Sends the "SV_CL_CLOSE" message to all clients. (private, suggested)
void Server::sendClose()
{
	// TODO: Send the "SV_CL_CLOSE" message to each client
	for (int i = 0; i < noOfPlayers; i++)
	{
		NetworkMessage msg = NetworkMessage(IO::_OUTPUT);
		msg.writeShort(sequence[i]);
		msg.writeByte(SV_CL_CLOSE);
		sendtoNetMessage(svSocket, msg, &playerAddress[i]);
	}

}

// Server message-sending helper method. (private, suggested)
int Server::sendMessage(sockaddr_in& destination, NetworkMessage& message)
{
	// TODO: Send the message in the buffer to the destination.
	return sendtoNetMessage(svSocket, message, &destination);
}

// Marks a client as connected and adjusts the game state.
void Server::connectClient(int player, sockaddr_in& source)
{
	playerAddress[player] = source;
	playerTimer[player] = 0;

	noOfPlayers++;
	if (noOfPlayers == 1)
		state.gamePhase = WAITING;
	else
		state.gamePhase = RUNNING;
}

// Marks a client as disconnected and adjusts the game state.
void Server::disconnectClient(int player)
{
	playerAddress[player].sin_addr.s_addr = INADDR_NONE;
	playerAddress[player].sin_port = 0;

	noOfPlayers--;
	if (noOfPlayers == 1)
		state.gamePhase = WAITING;
	else
		state.gamePhase = DISCONNECTED;
}

// Updates the state of the game.
void Server::updateState()
{
	// Tick counter.
	static int timer = 0;

	// Update the tick counter.
	timer++;

	// Next, update the game state.
	if (state.gamePhase == RUNNING)
	{
		// Update the player tick counters (for ALIVE messages.)
		playerTimer[0]++;
		playerTimer[1]++;

		// Update the positions of the player paddles
		if (state.player0.keyUp)
			state.player0.y--;
		if (state.player0.keyDown)
			state.player0.y++;

		if (state.player1.keyUp)
			state.player1.y--;
		if (state.player1.keyDown)
			state.player1.y++;
		
		// Make sure the paddle new positions are within the bounds.
		if (state.player0.y < 0)
			state.player0.y = 0;
		else if (state.player0.y > FIELDY - PADDLEY)
			state.player0.y = FIELDY - PADDLEY;

		if (state.player1.y < 0)
			state.player1.y = 0;
		else if (state.player1.y > FIELDY - PADDLEY)
			state.player1.y = FIELDY - PADDLEY;

		//just in case it get stuck...
		if (ballVecX)
			storedBallVecX = ballVecX;
		else
			ballVecX = storedBallVecX;

		if (ballVecY)
			storedBallVecY = ballVecY;
		else
			ballVecY = storedBallVecY;

		state.ballX += ballVecX;
		state.ballY += ballVecY;

		// Check for paddle collisions & scoring
		if (state.ballX < PADDLEX)
		{
			// If the ball has struck the paddle...
			if (state.ballY + BALLY > state.player0.y && state.ballY < state.player0.y + PADDLEY)
			{
				state.ballX = PADDLEX;
				ballVecX *= -1;
			}
			// Otherwise, the second player has scored.
			else
			{
				newBall();
				state.player1.score++;
				ballVecX *= -1;
			}
		}
		else if (state.ballX >= FIELDX - PADDLEX - BALLX)
		{
			// If the ball has struck the paddle...
			if (state.ballY + BALLY > state.player1.y && state.ballY < state.player1.y + PADDLEY)
			{
				state.ballX = FIELDX - PADDLEX - BALLX - 1;
				ballVecX *= -1;
			}
			// Otherwise, the first player has scored.
			else
			{
				newBall();
				state.player0.score++;
				ballVecX *= -1;
			}
		}

		// Check for Y position "bounce"
		if (state.ballY < 0)
		{
			state.ballY = 0;
			ballVecY *= -1;
		}
		else if (state.ballY >= FIELDY - BALLY)
		{
			state.ballY = FIELDY - BALLY - 1;
			ballVecY *= -1;
		}
	}

	// If the game is over...
	if ((state.player0.score > 10 || state.player1.score > 10) && state.gamePhase == RUNNING)
	{
		state.gamePhase = GAMEOVER;
		timer = 0;
	}
	if (state.gamePhase == GAMEOVER)
	{
		if (timer > 30)
		{
			initState();
			state.gamePhase = RUNNING;
		}
	}
}

// Initializes the state of the game.
void Server::initState()
{
	playerAddress[0].sin_addr.s_addr = INADDR_NONE;
	playerAddress[1].sin_addr.s_addr = INADDR_NONE;
	playerTimer[0] = playerTimer[1] = 0;
	noOfPlayers = 0;

	state.gamePhase = DISCONNECTED;

	state.player0.y = 0;
	state.player1.y = FIELDY - PADDLEY - 1;
	state.player0.score = state.player1.score = 0;
	state.player0.keyUp = state.player0.keyDown = false;
	state.player1.keyUp = state.player1.keyDown = false;

	newBall(); // Get new random ball position

	// Get a new random ball vector that is reasonable
	ballVecX = (rand() % 10) - 5;
	if ((ballVecX >= 0) && (ballVecX < 2))
		ballVecX = 2;
	if ((ballVecX < 0) && (ballVecX > -2))
		ballVecX = -2;

	ballVecY = (rand() % 10) - 5;
	if ((ballVecY >= 0) && (ballVecY < 2))
		ballVecY = 2;
	if ((ballVecY < 0) && (ballVecY > -2))
		ballVecY = -2;


}

// Places the ball randomly within the middle half of the field.
void Server::newBall()
{
	// (randomly in 1/2 playable area) + (1/4 playable area) + (left buffer) + (half ball)
	state.ballX = (rand() % ((FIELDX - 2 * PADDLEX - BALLX) / 2)) +
						((FIELDX - 2 * PADDLEX - BALLX) / 4) + PADDLEX + (BALLX / 2);

	// (randomly in 1/2 playable area) + (1/4 playable area) + (half ball)
	state.ballY = (rand() % ((FIELDY - BALLY) / 2)) + ((FIELDY - BALLY) / 4) + (BALLY / 2);
}
