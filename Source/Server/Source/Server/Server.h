#pragma once
#include <map>
#include <unordered_map>
#include <string>
#include <future>

#include "WinSock2.h"
#include "WS2tcpip.h"
#include "../Shared/Source/Network-Shared.h"

struct ClientData
{
	int myServerID;
	std::string name;
	//std::string connectionIP;
	sockaddr_in sockaddr;
};

#define SERVER_FAILED -1
#define SERVER_STARTED 1
#define SERVER_QUIT 2
#define SERVER_NEWCLIENT 3

#define INPUT_THREAD_FAILED -1;
#define INPUT_THREAD_QUIT 1;

#define S_FAIL(result) result == SERVER_FAILED

namespace TNP
{
	enum class MessageType;
	struct Message;
	struct ClientJoin;
	struct ClientDisconnect;
	struct ClientMessage;
}

class Server
{
public:
	Server() = default;
	~Server() = default;

	//void Init();

	const int CreateID(const int aPort);

	int Start();
	int StartInputThread(SOCKET& aSocket);

	int Run();
	int Shutdown();

private:
	TNP::MessageType DetermineMessageType();

	void HandleClientDisconnect(TNP::ClientDisconnect& aMessage, const int aClientPort);

	//void HandleClientJoined(TNP::ClientJoin& aMessage, const ClientData& someClientData);

	int SendMessageToAllClients(const TNP::Message& aMessage, const int aMessageSize, const int aClientToSkip = -1);
	int SendMessageToAClient(const TNP::Message& aMessage, const int aMessageSize, const int aClientID);

	void DEBUGMessageValidator(const char*& aMessage, const int aSize);

	// Data members
	std::map<int, int> myPortToID;
	std::unordered_map<int, ClientData> myConnectedClients;

	std::thread myInputThread;

	// Network members
	SOCKET myUDPSocket;
	WSADATA myWinSockData;

	sockaddr_in myBindAddressInformation;
	sockaddr_in myClientAddressInformation;

	int myAddrClientSize = sizeof(myClientAddressInformation);
	char mySocketBuffer[NETMESSAGE_SIZE];
};