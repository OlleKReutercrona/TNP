#pragma once
#include "Message.h"

#include <tge/Timer.h>

#include <Network-Shared.h>
#include <WinSock2.h>
#include "WS2tcpip.h"


#include <thread>
#include <unordered_map>


#define SERVER_ADDRESS "127.0.0.1"
#define SERVER_PORT 42000

struct ClientData
{
	int ID;
	std::string name;

	void operator=(const std::string& aString)
	{
		name = aString;
	}

	void operator=(const int anID)
	{
		ID = anID;
	}

	friend std::ostream& operator<<(std::ostream& os, const ClientData& aClient)
	{
		os << aClient.name;
		return os;
	}
};

struct Package
{
	TNP::Message* message;
	int size;
};



namespace TNP
{
	struct ClientUpdatePackage
	{
		Tga::Vector2f position;
	};

	class NetworkedClient
	{
	public:

		virtual void RecieveMessage(const TNP::ServerClientDisconnected& aMessage){ __noop; };
		virtual void RecieveMessage(const TNP::ClientMovedMessage& aMessage){ __noop; };
		virtual void ServerUpdate(const TNP::ClientUpdatePackage& aMessage){ __noop; };
		//virtual void RecieveMessage(const TNP::ServerClientDisconnected& aMessage){ __noop; };
		virtual void Disconnect() = 0;

		void operator=(const std::string& aString)
		{
			clientname = aString;
		}

		void operator=(const int anID)
		{
			clientID = anID;
		}

		friend std::ostream& operator<<(std::ostream& os, const NetworkedClient& aClient)
		{
			os << aClient.clientname;
			return os;
		}

		inline const int GetID() const { return clientID; }
		inline const std::string& GetName() const { return clientname; }

	private:
		int clientID = -1;
		std::string clientname;
	};


	class MessageHandler
	{
	public:
		MessageHandler();
		~MessageHandler();

		eNetworkCodes Start();
		eNetworkCodes Shutdown();
		eNetworkCodes Connect(const std::string& aName);

		// Maybe change this to client* or something so that messagehandler can give instructions from here
		void Subscribe	(const NetworkedClient* aClient);
		void Unsubscribe(const NetworkedClient* aClient);

		void Update();

	private:
		eNetworkCodes QueueMessage(Package& aMessageToSend);

		eNetworkCodes StartRecieveThread();
		eNetworkCodes ParseMessage();
		eNetworkCodes SendMessages();
#undef SendMessage
		eNetworkCodes SendMessage(Package* aMessageToSend);

		// might be redundant
		std::unordered_map<int, NetworkedClient*> myConnectedClients;


		// Probably will require rework since we dont want to save all messages
		std::unordered_map<int, std::vector<TNP::Message*>> myRecievedMessages;
		std::vector<Package> myMessagesToSend = {};

		// tickrate stuff
		Timer myTimer;
		const int myTickRate = 64;
		const float myTickTimeStep = 1.0f / (float)myTickRate;
		float myTimeSinceLastTick = 0.0f;


		// network properties
		std::thread myRecieveThread;
		std::atomic<bool> isRunning = true;
		std::atomic<bool> hasMessage = true;


		SOCKET myUDPSocket;
		WSADATA myWinSockData;
		sockaddr_in myServerAddress;
		char mySocketBuffer[NETMESSAGE_SIZE];

	};
}

