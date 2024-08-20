#include "stdafx.h"
#include "Server\Server.h"
#include <Message.h>
#include <iostream>
#include <ws2tcpip.h>
#include <tge/Timer.h>

#define LISTEN_PORT 42000

// Allows us to gracefully exit the program loop.
std::atomic<bool> isRunning = true;

const int Server::CreateID(const int aPort)
{
	// This func is funky and not good but I dont care :)))))

	if (myPortToID.size() > 1000) return -1;
	if (myPortToID.count(aPort) > 0) return -1;

	while (true)
	{
		int id = rand() % 10000 + 100;

		if (myConnectedClients.count(id) < 1)
		{
			myPortToID[aPort] = id;
			return id;
		}
	}

	return -1;
}

int Server::Start()
{
	std::cout << "HELLO I AM A SERVER! \n";
	std::cout << "Starting Winsock...";
	if (WSAStartup(MAKEWORD(2, 2), &myWinSockData) != 0)
	{
		std::cout << " FAIL!" << std::endl;
		std::cout << "Error: " << WSAGetLastError() << std::endl;
		return SERVER_FAILED;
	}
	std::cout << "GREAT SUCCESS!" << std::endl;



	myUDPSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (myUDPSocket == INVALID_SOCKET)
	{
		std::cout << "Failed to create socket." << std::endl;
		std::cout << "Error: " << WSAGetLastError() << std::endl;
		return SERVER_FAILED;
	}
	std::cout << "Successfully created a Socket!" << std::endl;

	// Populate the server address information
	myBindAddressInformation.sin_family = AF_INET;                    // Internetwork. Required for TCP/UDP/etc
	myBindAddressInformation.sin_addr.s_addr = INADDR_ANY;            // Don't care which IP address to use. Any available.
	// Computers usually have at least two when connected
	// to a network. One loopback and one network address.
	// 127.0.0.1 - Built-in loopback adapter.
	// e.g. 192.168.0.1 - Class-C address from a router.

	myBindAddressInformation.sin_port = htons(LISTEN_PORT);   // The port we want to listen for connections on.
	// htons converts the byte order from host order to
	// network order.

	// We need to bind to a specific address and port combination. This tells the
	// operating system that we want communication on that combination to go to
	// this program.
	if (bind(myUDPSocket, reinterpret_cast<sockaddr*>(&myBindAddressInformation), sizeof myBindAddressInformation) == SOCKET_ERROR)
	{
		std::cout << "Failed to bind socket." << std::endl;
		std::cout << "Error: " << WSAGetLastError() << std::endl;
		return SERVER_FAILED;
	}

	std::cout << "Successfully bound socket!" << std::endl;


	return SERVER_STARTED;
}

int Server::StartInputThread(SOCKET& aSocket)
{
	myInputThread = std::thread([aSocket]
		{
			while (isRunning)
			{
				char c[NETMESSAGE_SIZE];
				std::cin.getline(c, NETMESSAGE_SIZE);
				if (strcmp(c, "q") == 0)
				{
					isRunning = false;
					// This will cause the socket operations to abort and release the socket.
					// It will fire a SOCKET_ERROR result from recvfrom if it's presently waiting.
					closesocket(aSocket);
				}
				if (strcmp(c, "cls") == 0)
				{
					system("cls");
					std::cout << "Message Log: " << std::endl;
				}
			}
		});

	return INPUT_THREAD_QUIT;
}

int Server::Run()
{
	if (S_FAIL(StartInputThread(myUDPSocket)))
	{
		return SERVER_FAILED;
	}

	std::cout << "\nMessage Log: " << std::endl;

	sockaddr_in clientAddressInformation = {};
	static int clientAddrSize = sizeof(clientAddressInformation);
	char mySocketBuffer[NETMESSAGE_SIZE];

	Timer timer;

	constexpr int tickRate = 64;
	constexpr float tickTimeStep = 1.0f / (float)tickRate;

	float timeSinceLastTick = 0.0f;

	while (isRunning)
	{
		timer.Update();


		// Clear the buffer.
		ZeroMemory(mySocketBuffer, NETMESSAGE_SIZE);

		// blocking receive. This function will block until a message is received.
		const int recv_len = recvfrom(myUDPSocket, mySocketBuffer, NETMESSAGE_SIZE, 0, (sockaddr*)&clientAddressInformation, &clientAddrSize);
		if (recv_len == SOCKET_ERROR)
		{
			std::cout << "Failed receiving data from socket." << std::endl;
			std::cout << "Error: " << WSAGetLastError() << std::endl;
		}

		if (recv_len > 0)
		{
			ProcessMessage(mySocketBuffer, clientAddressInformation);
		}

		timeSinceLastTick += timer.GetDeltaTime();

		if (timeSinceLastTick < tickTimeStep) { continue; }

		timeSinceLastTick = 0.0f;

		// Send messages
		{
			// Sync all clients
			SyncClients();
		}

	}

	return SERVER_QUIT;
}

void Server::ProcessMessage(const char* aMessage, sockaddr_in& someInformation)
{
	// Extract the address information from the incoming data.
			// 16 bytes is enough for an IPv4 address.
			// i.e. "xxx.xxx.xxx.xxx" + string terminator
	char clientAddress[16]{ 0 };
	inet_ntop(AF_INET, &someInformation.sin_addr, &clientAddress[0], 16);
	const int clientPort = ntohs(someInformation.sin_port);

	TNP::MessageType type = DetermineMessageType(aMessage);

	switch (type)
	{
	case TNP::MessageType::error:
		break;
	case TNP::MessageType::clientJoin:
	{
		TNP::ClientJoin message;
		message.Deserialize(aMessage);

		//TNP::ClientJoin message = *(TNP::ClientJoin*)&aMessage;

		const int clientID = CreateID(clientPort);

		ClientData client;
		client.myServerID = clientID;
		client.name = message.username;
		client.sockaddr = someInformation;

		myConnectedClients[clientID] = client;

		TNP::ServerClientJoined msg;
		msg.id = clientID;

		strcpy_s(msg.username, USERNAME_MAX_LENGTH, client.name.c_str());

		//memcpy(msg.username, client.name.c_str(), sizeof(char) * client.name.length());
		//msg.username = client.name;

		std::cout << "Client " << client.name << " has joined the server." << std::endl;

		SendMessageToAllClients(msg, sizeof(msg), clientID);

		if (myConnectedClients.size() > 1)
		{
			TNP::ServerConnectedClientData connectionMessage;
			connectionMessage.numberOfClients = (int)myConnectedClients.size() - 1;

			//char* ptr = &connectionMessage.clients[0];

			int index = 0;

			for (auto& conClient : myConnectedClients)
			{
				if (conClient.second.myServerID == clientID) continue;

				memcpy(&connectionMessage.clients[index], &conClient.second.myServerID, sizeof(int));
				index += sizeof(int);

				char* username = (char*)conClient.second.name.c_str();
				memcpy(&connectionMessage.clients[index], username, USERNAME_MAX_LENGTH);
				index += USERNAME_MAX_LENGTH;
			}

			SendMessageToAClient(connectionMessage, sizeof(connectionMessage), client.myServerID);
		}

		break;
	}
	case TNP::MessageType::clientDisconnect:
	{
		TNP::ClientDisconnect message;
		message.Deserialize(aMessage);
		//TNP::ClientDisconnect message = *(TNP::ClientDisconnect*)&aMessage;

		TNP::ServerClientDisconnected msg;
		msg.id = myPortToID[clientPort];

		HandleClientDisconnect(message, clientPort);

		SendMessageToAllClients(msg, sizeof(msg));

		break;
	}
	case TNP::MessageType::clientMessage:
	{
		// Maybe not used after assignment 2

		TNP::ClientMessage message;
		message.Deserialize(aMessage);
		//TNP::ClientMessage message = *(TNP::ClientMessage*)&aMessage;

		const int clientID = myPortToID.at(clientPort);

		std::string printmsg = "[" + myConnectedClients.at(clientID).name + "] " + message.message;

		TNP::ServerClientMessage msg;
		msg.id = clientID;

		strcpy_s(msg.message, MESSAGE_MAX_SIZE, message.message);

		//msg.msgSize = sizeof(printmsg);
		//memcpy(msg.message, printmsg.data(), sizeof(printmsg));

		std::cout << printmsg << std::endl;

		SendMessageToAllClients(msg, sizeof(msg), clientID);

		break;
	}
	case TNP::MessageType::clientSendPosition:
	{
		TNP::ClientMovedMessage message;
		message.Deserialize(aMessage);

		const int clientID = myPortToID.at(clientPort);

		// Verify that client position is okay
		if (true)
		{
			// Position is okay, maybe handle it?

			myConnectedClients.at(clientID).position = message.position;

			//std::cout << "Client " << clientID << " position: " <<  message.position.x << ", " << message.position.y << "\n";
			break;
		}

		// Position was not okay, handle it?

		// TODO - HANDLE IT OLLE


		break;
	}
	default:
		std::cout << "Failed to determine message type \n";
	}
}

int Server::SendMessageToAllClients(const TNP::Message& aMessage, const int aMessageSize, const int aClientToSkip)
{
	// Need to check if message size is greater than max size 
	// if it is it needs to be packed in two messages

	const char* message = (char*)&aMessage;

	//DEBUGMessageValidator(message, aMessageSize);

	// Redist message to other clients
	for (auto& client : myConnectedClients)
	{
		// The client that sent the message doesn't need to get it again
		if (client.second.myServerID == aClientToSkip) continue;

		// Send it back
		if (sendto(myUDPSocket, message, aMessageSize, 0, reinterpret_cast<sockaddr*>(&client.second.sockaddr), sizeof(client.second.sockaddr)) == SOCKET_ERROR)
		{
			std::cout << "Failed to send message\n";
			std::cout << "Error: " << WSAGetLastError() << std::endl;
			isRunning = false;
			break;
		}
	}

	return 0;
}

int Server::SendMessageToAClient(const TNP::Message& aMessage, const int aMessageSize, const int aClientID)
{
	ClientData client = myConnectedClients[aClientID];

	const char* message = (char*)&aMessage;

	if (sendto(myUDPSocket, message, aMessageSize, 0, reinterpret_cast<sockaddr*>(&client.sockaddr), sizeof(client.sockaddr)) == SOCKET_ERROR)
	{
		std::cout << "Failed to send message\n";
		std::cout << "Error: " << WSAGetLastError() << std::endl;
		return SERVER_FAILED;
	}

	return 0;
}

void Server::SyncClients()
{
	// See if any clients moved since last check

	TNP::UpdateClientsMessage message;

	char* ptr = &message.data[0];

	// temp send all clients positions
	for (auto& [playerID, client] : myConnectedClients)
	{
		memcpy(ptr, &client.myServerID, sizeof(int));
		ptr += sizeof(int);

		memcpy(ptr, &client.position, sizeof(Tga::Vector2f));
		ptr += sizeof(Tga::Vector2f);

		//auto& data = message.myData.emplace_back();

		//data.playerID = client.myServerID;
		//data.position = client.position;
	}

	message.numberOfClients = (int)myConnectedClients.size();

	SendMessageToAllClients(message, sizeof(message));
}

int Server::Shutdown()
{
	myInputThread.join();
	WSACleanup();

	return SERVER_QUIT;
}



TNP::MessageType Server::DetermineMessageType(const char* aMessage)
{
	return *(TNP::MessageType*)&aMessage[0];
}


void Server::HandleClientDisconnect(TNP::ClientDisconnect& /*aMessage*/, const int aClientPort)
{
	ClientData client = myConnectedClients.at(myPortToID[aClientPort]);

	std::cout << "[SERVER] User '" << client.name << "' has disconnected from the server." << std::endl;

	// Remove cached data of client
	myConnectedClients.erase(client.myServerID);
	myPortToID.erase(aClientPort);
}