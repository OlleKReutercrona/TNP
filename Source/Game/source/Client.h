#pragma once
#include <thread>

#include "WinSock2.h"
#include "WS2tcpip.h"
#include "Network-Shared.h"

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

class Client
{
public:
	Client() = default;
	~Client();

	int Start();
	int Connect();

	int StartInputThread();
	int Run();

	void AssignPlayer(Player& aPlayer);

	int Shutdown();


	const bool& GetHasJoined()
	{
		return hasJoined;
	}

private:
	int SendClientMessage(const TNP::Message& aMSG, const int aSize);

	int HandleRecievedMessage();

	std::string myClientName = "xXx_DragonSlayer_xXx";

	std::thread myInputThread;

	std::unordered_map<int, std::string> myConnectedClients;

	// Networking
	SOCKET myUDPSocket;
	WSADATA myWinSockData;

	Player* myPlayer;

	sockaddr_in myServerAddress;
	char mySocketBuffer[NETMESSAGE_SIZE];
	char myMessage[NETMESSAGE_SIZE]{ 0 };
	

	bool hasJoined = false;

};