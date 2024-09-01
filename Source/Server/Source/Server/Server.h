#pragma once
#include <map>
#include <unordered_map>
#include <string>
#include <future>
#include <CircularBuffer.h>

#include <WinSock2.h>
#include "Network-Shared.h"
#include "Message.h"

struct ClientData
{
	int clientPort = 0;
	int myServerID = 0;
	std::string name = "";
	//std::string connectionIP;
	sockaddr_in sockaddr = {};
	TNP::CircularBuffer<TNP::ClientMovedMessage> myMessageBuffer = {};

	float timeSinceLastMessage = 0.0f;

	// Game Specific Data
	Tga::Vector2f position = {};
	int color = 0;
};

struct ClientPositionUpdateData
{
	int PID;
	Tga::Vector2f newPosition;
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
	int StartMessageInThread();

	int Run();
	int Shutdown();


	std::string UT_ProcessMessageClientJoin(const char* aMessage);


private:
	void ProcessMessage(const char* aMessage, sockaddr_in& someInformation);

	TNP::MessageType DetermineMessageType(const char* aMessage);

	void HandleClientDisconnect(const ClientData& aClient);

	//void HandleClientJoined(TNP::ClientJoin& aMessage, const ClientData& someClientData);

	int SendMessageToAllClients(const TNP::Message& aMessage, const int aMessageSize, const int aClientToSkip = -1);
	int SendMessageToAClient(const TNP::Message& aMessage, const int aMessageSize, const int aClientID);
	void SyncClients();


	int myEntityIds = 0;


	// Data members
	std::map<int, int> myPortToID = {};
	std::unordered_map<int, ClientData> myConnectedClients = {};

	//std::map<int, TNP::CircularBuffer <TNP::Message>> myBuffers = {};
	std::unordered_map<int, ClientPositionUpdateData> myUpdateData = {};
	
	std::thread myInputThread = {};
	std::thread myMessageThread = {};
	std::mutex myClientDataMutex;

	// Network members
	SOCKET myUDPSocket = {};
	WSADATA myWinSockData = {};

	sockaddr_in myBindAddressInformation = {};

	const float myClientDisconnectTime = 1.0f;
};