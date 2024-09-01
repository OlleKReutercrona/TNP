#pragma once
#include <string>
#include <vector>
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

		void Serialize(int& anIndex, const int anID, const Tga::Vector2f aVector)
		{
			memcpy(&data[anIndex], &anID, sizeof(int));
			anIndex += sizeof(int);

			memcpy(&data[anIndex], &aVector, sizeof(Tga::Vector2f));
			anIndex += sizeof(Tga::Vector2f);
		}

		struct ClientData
		{
			int playerID = -1;
			Tga::Vector2f position = { -99999 };
		};

		int numberOfClients = -1;
		char data[MAX_PLAYERS * (sizeof(int) + sizeof(Tga::Vector2f))] = { 0 };


		struct UnpackedUpdateClientMessage
		{
			UnpackedUpdateClientMessage(UpdateClientsMessage& aMessage)
			{
				numberOfClients = aMessage.numberOfClients;

				char* ptr = &aMessage.data[0];

				for (int i = 0; i < numberOfClients; i++)
				{
					UpdateClientsMessage::ClientData& client = clients.emplace_back();

					memcpy(&client, ptr, sizeof(UpdateClientsMessage::ClientData));
					ptr += sizeof(UpdateClientsMessage::ClientData);
				}
			}

			int numberOfClients = -1;
			std::vector<ClientData> clients;
		};
	};


	/*
		@
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

		void SerializeData(int& index, const int& id, const int& color, const Tga::Vector2f& pos, const std::string& name)
		{
			memcpy(&clients[index], &id, sizeof(int));
			index += sizeof(int);

			memcpy(&clients[index], &color, sizeof(int));
			index += sizeof(int);

			memcpy(&clients[index], &pos, sizeof(Tga::Vector2f));
			index += sizeof(Tga::Vector2f);

			char* username = (char*)name.c_str();
			memcpy(&clients[index], username, USERNAME_MAX_LENGTH);
			index += USERNAME_MAX_LENGTH;
		}

		struct ClientData
		{
			int id = -1;
			int color = 0;
			Tga::Vector2f position;
			char username[USERNAME_MAX_LENGTH] = { 0 };
			// 48 Bytes
		};
		unsigned int clientID = 9999999;
		int myColour = 0;
		int numberOfClients = 0;
		char clients[MAX_PLAYERS * sizeof(ClientData)] = {0};
		//std::vector<clientData> clients;

		struct UnpackedServerConnectedClientData
		{
			unsigned int clientID = 9999999;
			int myColour = 0;
			int numberOfClients = 0;
			std::vector < ServerConnectedClientData::ClientData> myData;

			void Unpack(ServerConnectedClientData& message)
			{
				clientID = message.clientID;
				myColour = message.myColour;
				numberOfClients = message.numberOfClients;

				char* ptr = &message.clients[0];
				for (int i = 0; i < numberOfClients; i++)
				{
					ServerConnectedClientData::ClientData& data = myData.emplace_back();

					// Id
					{
						memcpy(&data.id, ptr, sizeof(int));
						ptr += sizeof(int);
					}

					// Color
					{
						memcpy(&data.color, ptr, sizeof(int));
						ptr += sizeof(int);
					}

					// Position
					{
						memcpy(&data.position, ptr, sizeof(Tga::Vector2f));
						ptr += sizeof(Tga::Vector2f);
					}

					// Username
					{
						memcpy(&data.username, ptr, USERNAME_MAX_LENGTH);
						ptr += USERNAME_MAX_LENGTH;
					}
				}
			}
		};
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
		int color;
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