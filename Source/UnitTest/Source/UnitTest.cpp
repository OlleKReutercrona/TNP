#include "CppUnitTest.h"
#include <Server/Server.h>
#include <Client.h>
//#include <../Game/source/Client.h>
//#include "Client.h"
//#include "Server.h"
#include "Message.h"
#include <CircularBuffer.h>

#include <unordered_map>
#include <WinSock2.h>


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

namespace TNP_UT
{
	TEST_CLASS(MESSSAGE_TEST) 
	{
public:

	TEST_METHOD(MESSAGE_CLIENT_JOINED) 
	{
		//Client* client = new Client();
		Server* server = new Server();

		TNP::ClientJoin message;
		std::string userName = "Olle";


		strcpy_s(message.username, USERNAME_MAX_LENGTH, userName.c_str());

		//const char* msg = (char*)&message;

		std::string clientname = server->UT_ProcessMessageClientJoin((char*)&message);
		Assert::AreEqual(userName, clientname);
		Assert::AreEqual((std::string)"Olle", userName);


	}
	TEST_METHOD(MESSAGE_NEW_CLIENT_CONNECTED)
	{
		//std::unordered_map<int, ClientData> myConnectedClients = {};

		//int numberOfClients = 3;

		//for (int i = 0; i < numberOfClients; i++)
		//{
		//	//std::string name("Client_");
		//	//name += i;

		//	ClientData client;
		//	client.myServerID = i;
		//	//client.name = name;

		//	unsigned char red = (unsigned char)(rand() % 255);
		//	unsigned char blue = (unsigned char)(rand() % 255);
		//	unsigned char green = (unsigned char)(rand() % 255);
		//	unsigned char alpha = 255;

		//	int color = PackColors(red, blue, green, alpha);

		//	client.color = color;
		//	client.position = {
		//		(float)(rand() % 400),
		//		(float)(rand() % 400 )
		//	};

		//	myConnectedClients[client.myServerID] = client;
		//}


		//TNP::ServerConnectedClientData message;
		//int index = 0;
		//for (auto& [id, client] : myConnectedClients)
		//{
		//	if (id == numberOfClients - 1) continue;

		//	message.SerializeData(index, client.myServerID, client.color, client.position, client.name);
		//}
		//// SEND MESSAGE ->>>>>>>>>>>>>



		//// Unpack message <<<<<<<<<<<<<-

		//TNP::DeSerServerConnectedClientData deSerMessage;

		//deSerMessage.Deserialize(message);

		//ClientData client = myConnectedClients.at(2);

		//Assert::AreEqual(deSerMessage.clientID, client.myServerID);
		//Assert::AreEqual(deSerMessage.myColour, client.color);
		//Assert::AreEqual(deSerMessage.numberOfClients, message.numberOfClients);

		//for (unsigned int i = 0; i < deSerMessage.myData.size(); i++)
		//{
		//	TNP::ServerConnectedClientData::clientData& data = deSerMessage.myData[i];
		//	Assert::IsFalse(myConnectedClients.count(data.id) == 0);

		//	ClientData& cData = myConnectedClients.at(data.id);

		//	Assert::AreEqual(data.id, (int)cData.myServerID);
		//	Assert::AreEqual(data.color, cData.color);
		//	Assert::AreEqual(data.position, cData.position);
		//	Assert::AreEqual(data.username, cData.name.c_str());
		//}

	}

	};
}