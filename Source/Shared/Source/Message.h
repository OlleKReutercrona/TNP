#pragma once
#include <string>
#include "tge/math/Vector.h"
#include "Network-Shared.h"

namespace TNP
{
	enum class MessageType
	{
		error,								// Something went wrong
		
		//Client							
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
		updateClients,

		count,

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
		int messageID = 0;
	};


	struct ClientJoin : public Message
	{
		ClientJoin() : Message(MessageType::clientJoin) {}

		void Deserialize(const char* aRawMessage)
		{
			TNP::ClientJoin* msg = (TNP::ClientJoin*)(aRawMessage);

			memcpy(this, msg, sizeof(ClientJoin));
		}

		char username[USERNAME_MAX_LENGTH] = { 0 };
	};

	struct ClientDisconnect : public Message
	{
		ClientDisconnect() : Message(MessageType::clientDisconnect) {}

		void Deserialize(const char* aRawMessage) 
		{
			TNP::ClientDisconnect* msg = (TNP::ClientDisconnect*)(aRawMessage);

			memcpy(this, msg, sizeof(ClientDisconnect));
		}
	};

	struct ClientMessage : public Message
	{
		ClientMessage() : Message(MessageType::clientMessage) {}

		void Deserialize(const char* aRawMessage) 
		{
			TNP::ClientMessage* msg = (TNP::ClientMessage*)(aRawMessage);

			memcpy(this, msg, sizeof(ClientMessage));
		}

		char message[MESSAGE_MAX_SIZE] = { 0 };
	};


	/*
		A Client moved
	*/
	struct ClientMovedMessage : public Message
	{
		ClientMovedMessage() : Message(MessageType::clientSendPosition) {}

		void Deserialize(const char* aRawMessage) 
		{
			TNP::ClientMovedMessage* msg = (TNP::ClientMovedMessage*)(aRawMessage);

			memcpy(this, msg, sizeof(ClientMovedMessage));
		}

		int playerID = -1;
		Tga::Vector2f position;
	};

	struct UpdateClientsMessage : public Message
	{
		UpdateClientsMessage() : Message(MessageType::updateClients) {}

		void Deserialize(const char* aRawMessage)
		{
			TNP::UpdateClientsMessage* msg = (TNP::UpdateClientsMessage*)(aRawMessage);

			memcpy(this, msg, sizeof(UpdateClientsMessage));
		}

		struct ClientData
		{
			int playerID = -1;
			Tga::Vector2f position = {-99999};
		};

		int numberOfClients = -1;
		char data[MAX_PLAYERS * (sizeof(int) + sizeof(Tga::Vector2f))];
		//std::vector<ClientData> myData{};
	};


	/*
		This message is sent to a client that just joined to server to get information about all other clients that are currently connected.
		Contains the data of all currently connected clients
	*/
	struct ServerConnectedClientData : public Message
	{
		ServerConnectedClientData() : Message(MessageType::serverConnectedClientData) {}

		void Deserialize(const char* aRawMessage) 
		{
			TNP::ServerConnectedClientData* msg = (TNP::ServerConnectedClientData*)(aRawMessage);

			memcpy(this, msg, sizeof(ServerConnectedClientData));
		}

		struct clientData
		{
			int id = -1;
			char username[USERNAME_MAX_LENGTH] = { 0 };
			// 36 Bytes
		};

		int numberOfClients = 0;
		char clients[MESSAGE_MAX_SIZE] = { 0 };
		//std::vector<clientData> clients;
	};


	/*
		Client joined
		- this message is sent to all other clients when a new client joins
	*/
	struct ServerClientJoined : public Message
	{
		ServerClientJoined() : Message(MessageType::serverClientJoined) {}

		void Deserialize(const char* aRawMessage) 
		{
			TNP::ServerClientJoined* msg = (TNP::ServerClientJoined*)(aRawMessage);

			memcpy(this, msg, sizeof(ServerClientJoined));
		}

		int id = -1;
		char username[USERNAME_MAX_LENGTH] = { 0 };
	};


	/*
		A Client disconnected
		- this message is sent to all other clients when a client disconnects
	*/
	struct ServerClientDisconnected : public Message
	{
		ServerClientDisconnected() : Message(MessageType::serverClientDisconnected) {}

		void Deserialize(const char* aRawMessage) 
		{
			TNP::ServerClientDisconnected* msg = (TNP::ServerClientDisconnected*)(aRawMessage);

			memcpy(this, msg, sizeof(ServerClientDisconnected));
		}

		int id = -1;
	};

	/*
		A Client sent a text message
		- This messag eis sent to all other clients
	*/
	struct ServerClientMessage : public Message
	{
		ServerClientMessage() : Message(MessageType::serverClientMessage) {}

		void Deserialize(const char* aRawMessage) 
		{
			TNP::ServerClientMessage* msg = (TNP::ServerClientMessage*)(aRawMessage);

			memcpy(this, msg, sizeof(ServerClientMessage));
		}

		int id = -1;
		char message[MESSAGE_MAX_SIZE] = { 0 };
	};

	/*
		A Bundle of messages
	*/
	struct ServerMessageBundle : public Message
	{
		ServerMessageBundle() : Message(MessageType::serverBundle) {}

		int numberOfMessages = -1;
		int messageIndex = -1;
		char messages[NETMESSAGE_SIZE] = { 0 };
	};
}