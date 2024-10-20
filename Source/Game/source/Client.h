#pragma once
#include <thread>

#include "WinSock2.h"
#include "WS2tcpip.h"
#include "Network-Shared.h"
#include "PlayerCommands.h"
#include "Message.h"

#include <unordered_map>
#include <string>

#include <mutex>

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
class PlayerController;
class EntityFactory;
 
namespace TNP
{
	struct Message;
}


struct MessageToSendData
{
	MessageToSendData(TNP::Message* aMessage, int aMessageSize) : message(aMessage), messageSize(aMessageSize)
	{}

	TNP::Message* message;
	int messageSize = 0;
};


struct AckedMessage
{
	AckedMessage(const int aMessageID) : messageID(aMessageID) {}

	const int messageID = -1;
	float timeSinceAck = 0.0f;
};

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

	void Init(PlayerManager& aPlayerManager, EntityFactory& aEntityFactory);

	int Start();
	int Connect();
	int Connect(const std::string& aUsername);

	int StartInputThread();
	int Run();

	int RecieveMessageFromServer();

	void AssignPlayer(Player& aPlayer);

	int Shutdown();

	int SendPositionMessage();

	int SendStoredMessages();
	void RemoveStoredMessages();

	void StoreFlowerSpawnMessage();

	void StoreDestroyFlowerMessage(int aID);

	bool HasMessagesToSend();

	void StorePlayerCommands(std::vector<PlayerCommandData>& someCommands);

	const inline bool IsConnected()
	{
		return isConnected;
	}

	const inline bool HasJoined()
	{
		return hasJoined;
	}

	void UpdateAckedMessages(const float aDT);
private:
	int SendClientMessage(const TNP::Message& aMSG, const int aSize);

	int HandleRecievedMessage();

	void SendAckMessage(const TNP::Message& aMessage);


	PlayerManager* myPlayerManager;
	EntityFactory* myEntityFactory;

	Player* myPlayer;

	std::string myClientName = "xXx_DragonSlayer_xXx";

	std::thread myInputThread;

	std::unordered_map<int, std::string> myConnectedClients;

	std::vector<MessageToSendData> myMessagesToSend;

	const float myAckMessageSaveTime = 1.0f;
	std::map<unsigned int, AckedMessage> myAckedMessages;

	// Networking
	SOCKET myUDPSocket;
	WSADATA myWinSockData;

	MessageCounter myMessageCounter;

	sockaddr_in myServerAddress;
	char mySocketBuffer[NETMESSAGE_SIZE];
	char myMessage[NETMESSAGE_SIZE]{ 0 };
	
	std::mutex myAckMutex;

	bool hasJoined = false;
	bool isConnected = false;

};