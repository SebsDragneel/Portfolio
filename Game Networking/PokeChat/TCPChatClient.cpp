#include "TCPChatClient.h"

TCPChatClient::TCPChatClient(ChatLobby& chat_int) : chat_interface(chat_int)
{

}
TCPChatClient::~TCPChatClient(void)
{

}
bool TCPChatClient::init(std::string name, std::string ip_address, uint16_t port)
{
	isShutdown = false;


	mClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (mClient == INVALID_SOCKET)
		return false;
	
	sockaddr_in adress;
	adress.sin_family = AF_INET;
	adress.sin_addr.s_addr = inet_addr(ip_address.c_str());
	adress.sin_port = htons(port);
	if (connect(mClient, (SOCKADDR*)&adress, sizeof(adress)) < 0)
		return false;
	else
	chat_interface.DisplayString("CONNECTED TO SERVER");


	uint16_t Length = 18;
	send(mClient, (char*)&Length,2, 0);
	uint8_t msg_type = cl_reg;
	send(mClient, (char*)&msg_type, 1, 0);
	send(mClient, name.c_str(), 17, 0);
		
	return true;
}
bool TCPChatClient::run(void)
{
		uint16_t Len;
		tcp_recv_whole(mClient, (char*)&Len, 2, 0);
		char* buffer = new char[Len];
		tcp_recv_whole(mClient, buffer, 1, 0);

		uint8_t Type = *buffer;
		switch (NET_MESSAGE_TYPE(Type))
		{
			case sv_cnt:
			{
				tcp_recv_whole(mClient, (char*)&ID, 1, 0);
				uint16_t  Length = 1;
				send(mClient, (char*)&Length, 2, 0);
				uint8_t msg_type = cl_get;
				send(mClient, (char*)&msg_type, 1, 0);
				break;
			}
			case sv_full:
			{
				uint16_t  Length = 1;
				send(mClient, (char*)&Length, 2, 0);
				uint8_t msg_type = sv_full;
				send(mClient, (char*)&msg_type, 1, 0);
				chat_interface.DisplayString("CHAT IS FULL");

				break;

			}
			case sv_list:
			{
				uint8_t numberinlist;
				recv(mClient, buffer, 1, 0);
				numberinlist = *buffer;
				for (uint8_t i = 0; i < numberinlist; i++)
				{
					uint8_t ClientID;
					recv(mClient, (char*)&ClientID, 1, 0);
					recv(mClient, buffer, 17, 0);
					
					char* mTemp = new char[17];
					strcpy(mTemp, buffer);
					std::string name(mTemp);
					chat_interface.AddNameToUserList(name, ClientID);	
					delete[] mTemp;
				}
				chat_interface.DisplayString("REGISTERED IN SERVER");
				chat_interface.DisplayString("RECEIVED USER LIST");
				
				break;
			}
			case sv_add:
			{
				uint8_t ClientID;
				recv(mClient, (char*)&ClientID, 1, 0);
				recv(mClient, buffer, 17, 0);
				std::string name = buffer;
				chat_interface.AddNameToUserList(name, ClientID);
				chat_interface.DisplayString(name + " JOINED");

				break;

			}
			case sv_cl_close:
			{
				isShutdown = true;
				stop();
				return false;
				break;
			}
			case sv_cl_msg:
			{			
				uint8_t ClientID;
				recv(mClient, buffer,1, 0);
				ClientID = *buffer;
				recv(mClient, buffer, Len - 2, 0);
				std::string name = buffer;
				chat_interface.AddChatMessage(name, ClientID);
				break;
			}
			case sv_remove:
			{
				uint8_t ClientID;
				recv(mClient, buffer, 1, 0);
				ClientID = *buffer;
				chat_interface.DisplayString(" A Guy LEFT SERVER");
				chat_interface.RemoveNameFromUserList(ClientID);
				break;
			}
		default:
			return false;
			break;
	}

		delete[] buffer;
		return true;

}
bool TCPChatClient::send_message(std::string message)
{
		uint16_t Length = (uint16_t)message.length() + 3;
		uint8_t msg_type = sv_cl_msg;

		send(mClient, (char*)&Length, 2, 0);
		send(mClient, (char*)&msg_type, 1, 0);
		send(mClient, (char*)&ID, 1, 0);
		send(mClient, message.c_str(), Length - 2, 0);

		return true;
}
bool TCPChatClient::stop(void)
{
	if(isShutdown)
	{
		for  (unsigned int i = 0; (i < 4); i++)
		{
			chat_interface.RemoveNameFromUserList(i);
		}
	}
	else
	{
		uint16_t Length = 2;
		uint8_t msg_type = sv_cl_close;
		send(mClient, (char*)&Length, 2, 0);
		send(mClient, (char*)&msg_type, 1, 0);
		send(mClient, (char*)&ID, 1, 0);
	}
	shutdown(mClient, SD_BOTH);
	closesocket(mClient);
	return true;
} 