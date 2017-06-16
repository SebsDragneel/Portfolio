#include "TCPChatServer.h"

TCPChatServer::TCPChatServer(ChatLobby& chat_int) : chat_interface(chat_int)
{

}
TCPChatServer::~TCPChatServer(void)
{

}
bool TCPChatServer::init(uint16_t port)
{
	AvIDList.push_back(3);

	AvIDList.push_back(2);

	AvIDList.push_back(1);

	AvIDList.push_back(0);

	LSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (LSocket == INVALID_SOCKET)
		return false;

	sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = inet_addr("127.0.0.1");
	service.sin_port = htons(port);

	if (bind(LSocket, (sockaddr*)&service, sizeof(service)) == SOCKET_ERROR)
		return false;

	if (listen(LSocket, 1) == SOCKET_ERROR)
		return false;

	chat_interface.DisplayString("NOW HOSTING SERVER");

	return true;
}
bool TCPChatServer::run(void)
{
	fd_set ReadySet;

	ZeroMemory(&ReadySet, sizeof(ReadySet));
	FD_SET(LSocket, &ReadySet);

	for (int i = 0; i < UserList.size(); i++)
		FD_SET(UserList[i].mSocket, &ReadySet);

	select(0, &ReadySet, NULL, NULL, 0);

	if (FD_ISSET(LSocket, &ReadySet))
	{
		ASocket = accept(LSocket, NULL, NULL);
		if (ASocket == INVALID_SOCKET)
			return false;

		uint16_t Length = 18;
		recv(ASocket, (char *)&Length, 2, 0);

		uint8_t msg_type = cl_reg;
		recv(ASocket, (char *)&msg_type, 1, 0);

		char name[17];
		recv(ASocket, name, 17, 0);

		if (UserList.size() < MAX_CLIENTS)
		{
			UserList.push_back(user());

			UserList.back().mSocket = ASocket;

			Length = 2;
			send(UserList.back().mSocket, (char *)&Length, 2, 0);

			msg_type = sv_cnt;
			send(UserList.back().mSocket, (char *)&msg_type, 1, 0);

			Length = AvIDList.back();

			AvIDList.pop_back();

			send(UserList.back().mSocket, (char *)&Length, 1, 0);

			UserList.back().ID = Length;

			UserList.back().name = name;
		}
		else
		{
			Length = 1;
			send(ASocket, (char *)&Length, 2, 0);

			msg_type = sv_full;
			send(ASocket, (char *)&msg_type, 1, 0);
		}
		return true;
	}

	for (int i = 0; i < UserList.size(); i++)
	{
		if (FD_ISSET(UserList[i].mSocket, &ReadySet))
		{
			uint16_t Len = 0;
			recv(UserList[i].mSocket, (char *)&Len, 2, 0);

			uint8_t msg_type = 0;
			recv(UserList[i].mSocket, (char *)&msg_type, 1, 0);

			switch (msg_type)
			{
			case sv_cl_msg:
			{
				recv(UserList[i].mSocket, (char *)&msg_type, 1, 0);

				char *buffer = new char[Len - 2];
				recv(UserList[i].mSocket, buffer, Len - 2, 0);

				msg_type = sv_cl_msg;

				for (int j = 0; j < UserList.size(); j++)
				{
					send(UserList[j].mSocket, (char *)&Len, 2, 0);

					send(UserList[j].mSocket, (char *)&msg_type, 1, 0);

					send(UserList[j].mSocket, (char *)&UserList[i].ID, 1, 0);

					send(UserList[j].mSocket, buffer, Len - 2, 0);
				}
				delete[] buffer;
				break;
			}
			case sv_cl_close:
			{
				recv(UserList[i].mSocket, (char *)&msg_type, 1, 0);
				AvIDList.push_back(msg_type);

				Len = 2;

				msg_type = sv_remove;

				for (int j = 0; j < UserList.size(); j++)
				{
					send(UserList[j].mSocket, (char *)&Len, 2, 0);

					send(UserList[j].mSocket, (char *)&msg_type, 1, 0);

					send(UserList[j].mSocket, (char *)&UserList[i].ID, 1, 0);
				}
				closesocket(UserList[i].mSocket);
				UserList.erase(UserList.begin() + i);

				break;
			}
			case cl_get:
			{
				Len = 2 + (UserList.size() * 18);
				send(UserList[i].mSocket, (char *)&Len, 2, 0);

				msg_type = sv_list;
				send(UserList[i].mSocket, (char *)&msg_type, 1, 0);

				msg_type = UserList.size();
				send(UserList[i].mSocket, (char *)&msg_type, 1, 0);

				for (int j = 0; j < UserList.size(); j++)
				{
					send(UserList[i].mSocket, (char *)&UserList[j].ID, 1, 0);

					send(UserList[i].mSocket, UserList[j].name.c_str(), 17, 0);
				}

				for (int j = 0; j < UserList.size(); j++)
				{
					if (j != i)
					{
						Len = 19;
						send(UserList[j].mSocket, (char *)&Len, 2, 0);

						msg_type = sv_add;
						send(UserList[j].mSocket, (char *)&msg_type, 1, 0);

						send(UserList[j].mSocket, (char *)&UserList[i].ID, 1, 0);

						send(UserList[j].mSocket, UserList[i].name.c_str(), 17, 0);
					}
				}
				return true;

				break;
			}
			default:
				return false;
				break;
			}
		}
	}
	return true;
}
bool TCPChatServer::stop(void)
{
	uint16_t Length = 2;

	uint8_t msg_type = sv_cl_close;

	for (unsigned int j = 0; j < UserList.size(); j++)
	{
		send(UserList[j].mSocket, (char *)&Length, 2, 0);

		send(UserList[j].mSocket, (char *)&msg_type, 1, 0);

		send(UserList[j].mSocket, (char *)&UserList[0].ID, 1, 0);
	}

	shutdown(LSocket, 0);
	shutdown(ASocket, 0);

	for (unsigned int i = 0; i < UserList.size(); i++)
		shutdown(UserList[i].mSocket, 0);

	closesocket(LSocket);
	closesocket(ASocket);

	for (unsigned int i = 0; i < UserList.size(); i++)
		closesocket(UserList[i].mSocket);

	return true;
}