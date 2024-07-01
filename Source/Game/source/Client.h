#pragma once
#pragma message ("In Client")

#include <thread>

#include "WinSock2.h"
#include "WS2tcpip.h"
#include "Network-Shared.h"
#include "Message.h"

#include <unordered_map>
#include <string>


#define SERVER_ADDRESS "127.0.0.1"
#define SERVER_PORT 42000

#define C_FAIL(result) result == C_FAILED

#define C_FAILED -1
#define C_CONNECTION_FAILED -2
#define C_SUCCESS 1
#define C_QUIT 2


class Player;
 
namespace TNP
{
	struct Message;
}

// This is jank and should maybe be taken care of by server but probably not
//struct MessageCounter
//{
//	MessageCounter() : myMessages((int)TNP::MessageType::count) {}
//
//	int operator()(const int anID) 
//	{
//		assert(anID >= 0 && anID < myMessages.size() && "Index out of range");
//
//		return myMessages[anID];
//	}
//
//	std::vector<int> myMessages;
//};

class Client
{
public:
	Client() = default;
	~Client();

	int Start();
	int Connect();

	int StartInputThread();
	int Run();

	int RecieveMessageFromServer();

	void AssignPlayer(Player& aPlayer);

	int Shutdown();

	int SendPositionMessage();


	const inline bool GetHasJoined()
	{
		return hasJoined;
	}

private:
	int SendClientMessage(const TNP::Message& aMSG, const int aSize);

	int HandleRecievedMessage();

	Player* myPlayer;

	std::string myClientName = "xXx_DragonSlayer_xXx";

	std::thread myInputThread;

	std::unordered_map<int, std::string> myConnectedClients;

	// Networking
	SOCKET myUDPSocket;
	WSADATA myWinSockData;

	//MessageCounter myMessageCounter;

	sockaddr_in myServerAddress;
	char mySocketBuffer[NETMESSAGE_SIZE];
	char myMessage[NETMESSAGE_SIZE]{ 0 };
	

	bool hasJoined = false;

};