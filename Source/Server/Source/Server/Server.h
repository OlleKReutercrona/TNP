#pragma once
#include <map>
#include <unordered_map>
#include <string>
#include <future>
#include <CircularBuffer.h>

#include <WinSock2.h>
#include "Network-Shared.h"
#include "Message.h"
#include "Server/ServerEntityFactory.h"

#include <chrono>

struct ClientData;
class UnAckedMessage;

struct ClientData
{
	int clientPort = 0;
	int myServerID = 0;
	std::string name = "";
	//std::string connectionIP;
	sockaddr_in sockaddr = {};
	TNP::CircularBuffer<TNP::ClientMovedMessage> myMessageBuffer = {};

	std::map<unsigned int, UnAckedMessage> myUnackedMessages = {};

	float timeSinceLastMessage = 0.0f;

	// Game Specific Data
	Tga::Vector2f position = {};
	int color = 0;

	int messageCounter = 0;
};

class UnAckedMessage
{
public:
	UnAckedMessage(const std::shared_ptr<TNP::Message> aMessage, const int aMessageSize) : myMessage(aMessage), mySentTime(std::chrono::high_resolution_clock::now()), myMesssageSize(aMessageSize) {}

	double GetTimeSince()
	{
		return std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - mySentTime).count();
	}
	double GetTimeSince(const std::chrono::high_resolution_clock::time_point& aTimePoint)
	{
		return std::chrono::duration<double>(aTimePoint - mySentTime).count();
	}

	float myTimeSinceFirstAttempt = 0.0f;
	float myTimeSinceLastAttempt = 0.0f;
	const std::chrono::high_resolution_clock::time_point mySentTime;
	const std::shared_ptr<TNP::Message> myMessage;
	const int myMesssageSize;
	unsigned int myAttempts = 0;
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

	int SendMessageToAllClients(TNP::Message& aMessage, const int aMessageSize, const int aClientToSkip = -1);
	int SendMessageToAClient(TNP::Message& aMessage, const int aMessageSize, const int aClientID);
#undef SendMessage
	bool SendMessage(TNP::Message& aMessage, const int aMessageSize, ClientData& aClient);
	void SyncClients();

	void CheckForClientDisconnect();

	void HandleUnAckedMessages(const float aDT);

	int myEntityIds = 0;
	server::ServerEntityFactory myEntityFactory;

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
	const double myUnAckedMessageRetryTime = 0.2f;
};