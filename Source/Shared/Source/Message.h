#pragma once
#include <string>
#include "Network-Shared.h"

namespace TNP
{
	enum class MessageType
	{
		error,
		
		//Client							// Something went wrong
		clientJoin,							// Sent from client when requesting to join
		clientDisconnect,					// Sent from client to tell server that they disconnected
		clientMessage,						// Message sent from client to server
		clientSendPosition,					


		//Server
		serverConnectedClientData,
		serverClientJoined,					// Sent from server to all clients to allert them that another client joined
		serverClientDisconnected,			// Sent from server to all clients to allert them that another client disconnected
		serverClientMessage,				// Message sent from a client and then being forwarded from server to all the other clients
		serverBundle,

	};

	// ___int to char___
	// int i = 65
	// char c(i)

	// __char to int___
	// char c = 'A'
	// int i(c)


	struct Message
	{
		Message(const MessageType aType) : type(aType) {}

	public:
		const MessageType type;
	};


	struct ClientJoin : public Message
	{
		ClientJoin() : Message(MessageType::clientJoin) {}

		char username[USERNAME_MAX_LENGTH];
	};

	struct ClientDisconnect : public Message
	{
		ClientDisconnect() : Message(MessageType::clientDisconnect) {}
	};

	struct ClientMessage : public Message
	{
		ClientMessage() : Message(MessageType::clientMessage) {}

		char message[MESSAGE_MAX_SIZE];
	};

	struct ServerConnectedClientData : public Message
	{
		ServerConnectedClientData() : Message(MessageType::serverConnectedClientData) {}

		struct clientData
		{
			int id;
			char username[USERNAME_MAX_LENGTH];
			// 36 Bytes
		};

		int numberOfClients = 0;
		char clients[MESSAGE_MAX_SIZE];
		//std::vector<clientData> clients;
	};

	struct ServerClientJoined : public Message
	{
		ServerClientJoined() : Message(MessageType::serverClientJoined) {}

		int id = -1;
		char username[USERNAME_MAX_LENGTH];
	};

	struct ServerClientDisconnected : public Message
	{
		ServerClientDisconnected() : Message(MessageType::serverClientDisconnected) {}

		int id = -1;
	};

	struct ServerClientMessage : public Message
	{
		ServerClientMessage() : Message(MessageType::serverClientMessage) {}

		int id = -1;
		char message[MESSAGE_MAX_SIZE];
	};

	struct ServerMessageBundle : public Message
	{
		ServerMessageBundle() : Message(MessageType::serverBundle) {}

		int numberOfMessages;
		int messageIndex;
		char messages[NETMESSAGE_SIZE - sizeof(int) * 2 - sizeof(MessageType)];
	};
}