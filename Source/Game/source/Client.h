#pragma once
#include <thread>

#include "WinSock2.h"
#include "WS2tcpip.h"
#include "Network-Shared.h"

#include "Message.h"

#include <unordered_map>
#include <string>


#define SERVER_ADDRESS "127.0.0.1"
#define SERVER_PORT 42000

#define C_FAIL(result) result <= C_FAILED

#define C_UNAME_TOO_LONG -3
#define C_CONNECTION_FAILED -2
#define C_FAILED -1
#define C_CONNECTING 3
#define C_SUCCESS 1
#define C_QUIT 2


class Player;
 
namespace TNP
{
	struct Message;
}


// This is jank and should maybe be taken care of by server but probably not
struct MessageCounter
{
	MessageCounter() : myMessages((int)TNP::MessageType::count) {}

	int operator()(const int anID) 
	{
		assert(anID >= 0 && anID < myMessages.size() && "Index out of range");

		return myMessages[anID];
	}

	std::vector<int> myMessages;
};

class PlayerManager;

class Client
{
public:
	Client() = default;
	~Client();

	void Init(PlayerManager& aPlayerManager);

	int Start();
	int Connect();
	int Connect(const std::string& aUsername);

	int StartInputThread();
	int Run();

	int RecieveMessageFromServer();

	void AssignPlayer(Player& aPlayer);

	int Shutdown();

	int SendPositionMessage();

	const inline bool IsConnected()
	{
		return isConnected;
	}

	const inline bool HasJoined()
	{
		return hasJoined;
	}

private:
	int SendClientMessage(const TNP::Message& aMSG, const int aSize);

	int HandleRecievedMessage();

	PlayerManager* myPlayerManager;

	Player* myPlayer;

	std::string myClientName = "xXx_DragonSlayer_xXx";

	std::thread myInputThread;

	std::unordered_map<int, std::string> myConnectedClients;

	// Networking
	SOCKET myUDPSocket;
	WSADATA myWinSockData;

	MessageCounter myMessageCounter;

	sockaddr_in myServerAddress;
	char mySocketBuffer[NETMESSAGE_SIZE];
	char myMessage[NETMESSAGE_SIZE]{ 0 };
	

	bool hasJoined = false;
	bool isConnected = false;

};