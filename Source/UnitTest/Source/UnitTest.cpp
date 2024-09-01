#include "CppUnitTest.h"
#include <Server/Server.h>
#include <Client.h>
//#include <../Game/source/Client.h>
//#include "Client.h"
//#include "Server.h"
#include "Message.h"
#include <CircularBuffer.h>

#include <unordered_map>
//#include <WinSock2.h>


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

//struct ClientData
//{
//	int clientPort = -1;
//	unsigned int myServerID = 9999999;
//	std::string name = "";
//	//std::string connectionIP;
//	//sockaddr_in sockaddr;
//	TNP::CircularBuffer<TNP::ClientMovedMessage> myMessageBuffer = {};
//
//	float timeSinceLastMessage = 0.0f;
//
//	// Game Specific Data
//	Tga::Vector2f position = {};
//	int color = 0;
//};
//
//struct ClientPositionUpdateData
//{
//	int PID = 0;
//	Tga::Vector2f newPosition = {};
//};

void CreateClients(std::unordered_map<int, ClientData>& aMap, const int aNumberOfClients)
{
	for (int i = 0; i < aNumberOfClients; i++)
	{
		std::string name("Client_");
		name += std::to_string(i);

		ClientData client;
		client.myServerID = i;
		client.name = name;

		unsigned char red = (unsigned char)(rand() % 255);
		unsigned char blue = (unsigned char)(rand() % 255);
		unsigned char green = (unsigned char)(rand() % 255);
		unsigned char alpha = 255;

		int color = PackColors(red, blue, green, alpha);

		client.color = color;
		client.position = {
			(float)(rand() % 400),
			(float)(rand() % 400)
		};

		aMap[client.myServerID] = client;
	}
}


TNP::MessageType DetermineMessageType(const char* aMessage)
{
	return *(TNP::MessageType*)&aMessage[0];
}

namespace TNP_UT
{
	TEST_CLASS(MESSSAGE_TEST)
	{
	public:

		TEST_METHOD(CLIENT_TO_SERVER_CLIENT_JOINED)
		{
			std::string username = "xX_DragonSlayer94_Xx";

			TNP::ClientJoin serializeMessage;
			strcpy_s(serializeMessage.username, USERNAME_MAX_LENGTH, username.c_str());

			const char* msg = (char*)&serializeMessage;

			TNP::ClientJoin message;
			message.Deserialize(msg);

			Assert::AreEqual((int)TNP::MessageType::clientJoin, (int)message.type);

			Assert::AreEqual(serializeMessage.username, message.username);
		}

		TEST_METHOD(CLIENT_TO_SERVER_CLIENT_MOVED)
		{
			Tga::Vector2f newPosition = { 10.f, 5.f };
			int playerIDToMove = 2;

			TNP::ClientMovedMessage serializeMessage;
			serializeMessage.playerID = playerIDToMove;
			serializeMessage.position = newPosition;

			const char* msg = (char*)&serializeMessage;

			TNP::ClientMovedMessage message;
			message.Deserialize(msg);

			Assert::AreEqual((int)TNP::MessageType::clientSendPosition, (int)message.type);

			Assert::AreEqual(serializeMessage.playerID, message.playerID);
			Assert::AreEqual(serializeMessage.position.x, message.position.x);
			Assert::AreEqual(serializeMessage.position.y, message.position.y);
		}

		TEST_METHOD(SERVER_TO_CLIENTS_NEW_CLIENT_JOINED)
		{
			int idOfNewClient = 5;
			int colorOfNewClient = 5;
			std::string usernameOfNewClient = "xX_DragonSlayer94_Xx";

			TNP::ServerClientJoined serializeMessage;
			serializeMessage.id = idOfNewClient;
			serializeMessage.color = colorOfNewClient;
			strcpy_s(serializeMessage.username, USERNAME_MAX_LENGTH, usernameOfNewClient.c_str());

			const char* msg = (char*)&serializeMessage;

			TNP::ServerClientJoined message;
			message.Deserialize(msg);

			Assert::AreEqual((int)TNP::MessageType::serverClientJoined, (int)message.type);

			Assert::AreEqual(serializeMessage.id, message.id);
			Assert::AreEqual(serializeMessage.color, message.color);
			Assert::AreEqual(serializeMessage.username, message.username);
		}

		TEST_METHOD(SERVER_TO_CLIENTS_CLIENT_DISCONNECTED)
		{
			int idOfDisconnectedClient = 5;

			TNP::ServerClientDisconnected serializeMessage;
			serializeMessage.id = idOfDisconnectedClient;

			const char* msg = (char*)&serializeMessage;

			TNP::ServerClientDisconnected message;
			message.Deserialize(msg);

			Assert::AreEqual((int)TNP::MessageType::serverClientDisconnected, (int)message.type);

			Assert::AreEqual(serializeMessage.id, message.id);
		}

		TEST_METHOD(MESSAGE_NEW_CLIENT_CONNECTED)
		{
			std::unordered_map<int, ClientData> myConnectedClients = {};

			int numberOfClients = 3;

			CreateClients(myConnectedClients, numberOfClients);

			char* msg = nullptr;
			{
				TNP::ServerConnectedClientData serverMsg;

				serverMsg.clientID = myConnectedClients.at((int)myConnectedClients.size() - 1).myServerID;; // <- Receiving clients id
				serverMsg.myColour = myConnectedClients.at((int)myConnectedClients.size() - 1).color;
				serverMsg.numberOfClients = (int)myConnectedClients.size() - 1;

				int index = 0;
				for (auto& [id, client] : myConnectedClients)
				{
					if (id == numberOfClients - 1) continue;

					serverMsg.SerializeData(index, client.myServerID, client.color, client.position, client.name);
				}

				msg = (char*)&serverMsg;
			}
			// SEND MESSAGE ->>>>>>>>>>>>>



			// Unpack message <<<<<<<<<<<<<-

			TNP::MessageType type = DetermineMessageType(msg);

			Assert::AreEqual((int)TNP::MessageType::serverConnectedClientData, (int)type);

			TNP::ServerConnectedClientData* recMsg = (TNP::ServerConnectedClientData*)msg;

			TNP::ServerConnectedClientData::UnpackedServerConnectedClientData deSerMessage;

			deSerMessage.Unpack(*recMsg);

			ClientData& client = myConnectedClients.at(2);

			Assert::AreEqual(client.myServerID, (int)deSerMessage.clientID);
			Assert::AreEqual(client.color, deSerMessage.myColour);
			Assert::AreEqual(numberOfClients - 1, deSerMessage.numberOfClients);

			for (unsigned int i = 0; i < deSerMessage.myData.size(); i++)
			{
				TNP::ServerConnectedClientData::ClientData& data = deSerMessage.myData[i];
				Assert::IsFalse(myConnectedClients.count(data.id) == 0);

				ClientData& cData = myConnectedClients.at(data.id);

				Assert::AreEqual((int)cData.myServerID, data.id);
				Assert::AreEqual(cData.color, data.color);
				Assert::AreEqual(cData.position.x, data.position.x);
				Assert::AreEqual(cData.position.y, data.position.y);
				Assert::AreEqual(cData.name.c_str(), data.username);
			}

		}


		TEST_METHOD(SERVER_TO_CLIENT_UPDATE_CLIENT_MESSAGE)
		{
			std::unordered_map<int, ClientData> myConnectedClients = {};

			int numberOfClients = 3;

			char* msg = nullptr;

			CreateClients(myConnectedClients, numberOfClients);

			{

				TNP::UpdateClientsMessage message;

				message.numberOfClients = (int)myConnectedClients.size();

				int index = 0;

				for (auto& [id, client] : myConnectedClients)
				{
					message.Serialize(index, client.myServerID, client.position);
				}

				msg = (char*)&message;
			}

			// SEND MESSSAGE ->>>>>>>>>>>>>>>>>



			// RECEIVED MESSSAGE <<<<<<<<<<<<<<<<-

			TNP::MessageType type = DetermineMessageType(msg);

			Assert::AreEqual((int)TNP::MessageType::updateClients, (int)type);

			TNP::UpdateClientsMessage* recMsg = (TNP::UpdateClientsMessage*)msg;

			TNP::UpdateClientsMessage::UnpackedUpdateClientMessage deSerMessage(*recMsg);

			for (int i = 0; i < deSerMessage.numberOfClients; i++)
			{
				TNP::UpdateClientsMessage::ClientData& client = deSerMessage.clients[i];
				ClientData& serverClient = myConnectedClients.at(client.playerID);


				Assert::AreEqual(client.playerID, serverClient.myServerID);
				Assert::AreEqual(client.position.x, serverClient.position.x);
				Assert::AreEqual(client.position.y, serverClient.position.y);
			}
		}
	};
}