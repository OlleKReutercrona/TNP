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
#include "UnackedMessage.h"

struct ClientData;

struct ClientData
{
	int clientPort = 0;
	int myServerID = 0;
	std::string name = "";
	sockaddr_in sockaddr = {};
	TNP::CircularBuffer<TNP::ClientMovedMessage> myMessageBuffer = {};

	/* Container for received message. 
	Key1: messagetype
	Value1: map with messageIds
	Key2: messageId
	Value2: time since received
	*/
	std::map<TNP::MessageType, std::map<int, float>> myReceivedMessages;
	std::map<unsigned int, UnAckedMessage> myUnackedMessages = {};

	float timeSinceLastMessage = 0.0f;

	// Game Specific Data
	Tga::Vector2f position = {};
	int color = 0;

	int messageCounter = 0;
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

	int SendMessageToAllClients(TNP::Message& aMessage, const int aMessageSize, const int aClientToSkip = -1);
	int SendMessageToAClient(TNP::Message& aMessage, const int aMessageSize, const int aClientID);
#undef SendMessage
	bool SendMessage(TNP::Message& aMessage, const int aMessageSize, ClientData& aClient);
	void SendAckMessage(ClientData& aClient, const int anAckID);
	void UpdateAckedMessages(const float aDT);
	void SyncClients();

	void CheckForClientDisconnect();

	void SyncAllEntitiesToJoinedClient(int clientID);

	void HandleUnAckedMessages(const float aDT);

	void UpdateReceivedMessagesTimer(const float aDT);

	ClientData* GetClientByPort(const int aPort);

	int myEntityIds = 0;
	server::ServerEntityFactory myEntityFactory;

	// Data members
	std::map<int, int> myPortToID = {};
	std::unordered_map<int, ClientData> myConnectedClients = {};
	std::unordered_map<int, ClientPositionUpdateData> myUpdateData = {};
	std::map<unsigned int, AckedMessage> myAckedMessages;

	std::thread myInputThread = {};
	std::thread myMessageThread = {};
	std::mutex myClientDataMutex;
	std::mutex myAckMutex;

	// Network members
	SOCKET myUDPSocket = {};
	WSADATA myWinSockData = {};

	sockaddr_in myBindAddressInformation = {};

	const float myClientDisconnectTime = 5.0f;
	const double myUnAckedMessageRetryTime = 0.2f;
	const float myAckMessageSaveTime = 1.0f;
	const float myReceivedMessageSaveTime = 2.5f;
};