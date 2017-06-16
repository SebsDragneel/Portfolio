//TODO: Add client functionality that complies with ClientWrapper Usage.
#include "Client.h"


int Client::init(uint16_t port, char* address)
{
	isShutdown = false;
	int Last = -1;
	int Cycles = 0;
	//check address in the dotted quadrant
	unsigned int i = 0;

	while (address[i] != NULL)
	{
		if (address[i] == '.')
		{
			if (Last == i - 1)
				return ADDRESS_ERROR;
			Last = i;
			Cycles++;
		}
		if (i - Last > 4 || Cycles > 3)
			return ADDRESS_ERROR;

		i++;
	}
	if (Cycles < 3)
		return ADDRESS_ERROR;

	mClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (mClient == INVALID_SOCKET)
		return SETUP_ERROR;

	sockaddr_in name;
	name.sin_family = AF_INET;
	name.sin_addr.s_addr = inet_addr(address);
	name.sin_port = htons(port);

	if (connect(mClient, (SOCKADDR*)&name, sizeof(name)) != 0)
		return CONNECT_ERROR;
	
	return SUCCESS;
}
int Client::readMessage(char* buffer, int32_t size)
{
	int error = recv(mClient, buffer, 1, 0);
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
int Client::sendMessage(char* buffer, int32_t size)
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
void Client::stop()
{
	isShutdown = true;
	shutdown();
	closesocket(mClient);
}