//TODO: Add server functionality that complies with ServerWrapper Usage.
#include "Server.h"
#include <winsock2.h>
#pragma comment(lib, "Ws2_32.lib")
#include <Windows.h>

int Server::init(uint16_t port)
{	
	isShutdown = false;
	 mSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	 if (mSocket == INVALID_SOCKET)
		 return SETUP_ERROR;

	 sockaddr_in name;
	 name.sin_family = AF_INET;
	 name.sin_addr.s_addr = INADDR_ANY;
	 name.sin_port = htons(port);

	 if (bind(mSocket, (SOCKADDR*)&name, sizeof(name)) != 0)
		 return BIND_ERROR;

	 if (listen(mSocket, SOMAXCONN) != 0)
		 return SETUP_ERROR;

	 mClient = accept(mSocket, NULL, NULL);
	 if (mClient == INVALID_SOCKET)
		 return SHUTDOWN;

	return SUCCESS;
}
int Server::readMessage(char* buffer, int32_t size)
{
	auto error = recv(mClient, buffer, 1, 0);
	/*auto ERROR = WSAGetLastError();*/

	int lenght = *buffer;
	int byterecvd = 0;

	if (isShutdown)
		return SHUTDOWN;
	if (error <= 0)
		return DISCONNECT;
	if (error > size)
		return PARAMETER_ERROR;

	while (byterecvd < lenght)
	{
		error = recv(mClient, buffer + byterecvd, lenght - byterecvd, 0);
		if (error <= 0)
			return DISCONNECT;
		byterecvd += error;
	}
	return SUCCESS;
}
int Server::sendMessage(char* buffer, int32_t size)
{
	if (size < 0 || size >255)
		return PARAMETER_ERROR;

	byte length = (byte)size;

	if (send(mClient, (char*)&length, 1, 0) < 0)
	{
		if (isShutdown)
			return SHUTDOWN;

		return DISCONNECT;
	}
	if (send(mClient, buffer, size, 0) < 0)
	{
		if (isShutdown)
			return SHUTDOWN;
	
		return DISCONNECT;
	}
	return SUCCESS;
}
void Server::stop()
{
	isShutdown = true;
	shutdown();
	closesocket(mSocket);
	closesocket(mClient);
}