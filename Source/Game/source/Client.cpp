// Client.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include "stdafx.h"
#include "Client.h"
#include <iostream>
#include "Message.h"
#include "Player.h"
#include "PlayerManager.h"
#include "EntityFactory.h"
#include "NetworkDebugStatManager.h"
// Allows us to gracefully exit the program loop.
std::atomic<bool> isRunning = true;
std::atomic<bool> hasMessage = false;

Client::~Client()
{
	Shutdown();
}

void Client::Init(PlayerManager& aPlayerManager, EntityFactory& aEntityFactory, TNP::NetworkDebugStatManager& aStatManager)
{
	myStatManager = &aStatManager;
	myPlayerManager = &aPlayerManager;
	myEntityFactory = &aEntityFactory;
}

void Client::AssignPlayer(Player& aPlayer)
{
	myPlayer = &aPlayer;
	myPlayer->SetUsername(myClientName);
}

int Client::Start()
{
	std::cout << "Starting Winsock...";
	if (WSAStartup(MAKEWORD(2, 2), &myWinSockData) != 0)
	{
		std::cout << " FAIL!" << std::endl;
		std::cout << "Error: " << WSAGetLastError() << std::endl;
		return C_FAILED;
	}
	std::cout << "OK!" << std::endl;

	myUDPSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (myUDPSocket == INVALID_SOCKET)
	{
		std::cout << "Failed to create udpSocket." << std::endl;
		std::cout << "Error: " << WSAGetLastError() << std::endl;
		return C_FAILED;
	}

	// Set udpSocket to non blocking
	// This way recvfrom will not wait until it gets a message
	u_long iMode = true;
	ioctlsocket(myUDPSocket, FIONBIO, &iMode);

	// Tell the system where the server is that we want to talk with.
	myServerAddress.sin_family = AF_INET;                    // Internetwork. Required for TCP/UDP/etc

	// Unlike on the Server we cannot specify INADDR_ANY since we need to know
	// which destination IP address to use.
	InetPton(AF_INET, TEXT(SERVER_ADDRESS), &myServerAddress.sin_addr.s_addr);

	myServerAddress.sin_port = htons(SERVER_PORT);   // The port that the server is listening on
	// htons converts the byte order from host order to
	// network order.

	// Why no bind on client? We leave that to the operating system!
	// By calling sendto we cause Windows to implicitly bind the udpSocket
	// by filling in all the information and giving us a random local port.
	// This then allows us to use recvfrom as we please because the udpSocket
	// is already bound for us in an appropriate way.

	//if (C_FAIL(StartInputThread()))
	//{
	//	std::cout << "Input thread failed" << std::endl;
	//	return C_FAILED;
	//}


	return C_SUCCESS;
}

int Client::Connect()
{
	bool hasConnected = false;

	while (!hasConnected)
	{
		std::cout << "Enter a username: ";
		while (!hasMessage)
		{
			std::cin.getline(myMessage, NETMESSAGE_SIZE);
			hasMessage = true;
		}
		std::string messageStr(myMessage);
		if (messageStr.size() <= USERNAME_MAX_LENGTH)
		{
			TNP::ClientJoin message;
			message.messageID = myMessageCounter(static_cast<int>(TNP::MessageType::clientJoin));
			//message.username = { 0 };

			strcpy_s(message.username, USERNAME_MAX_LENGTH, messageStr.c_str());

			myClientName = messageStr;

			if (C_FAIL(SendClientMessage(message, sizeof(message))))
			{
				return C_CONNECTION_FAILED;
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(333));

			unsigned int counter = 0;
			const unsigned int maxCount = 20;

			while (hasConnected == false)
			{
				if (counter >= maxCount)
				{
					std::cout << "FAILED TO CONNECT TO SERVER, PLZ RESTART PROGRAM AND RETRY!\n";
					return C_CONNECTION_FAILED;
				}
				RecieveMessageFromServer();

				if (hasJoined)
				{
					hasConnected = true;
					break;
				}

				std::this_thread::sleep_for(std::chrono::milliseconds(500));

				std::cout << "Failed to connect to server, retrying...\n";

				if (C_FAIL(SendClientMessage(message, sizeof(message))))
				{
					return C_CONNECTION_FAILED;
				}
				counter++;
			}


			/*
				To do in future here.
				The message will send even if the server is not responding leaving the client to think that it is connected.
				We should wait until server has sent an ACK message before we try to send other messages to the server.
				- Olle
			*/
			hasConnected = true;
			std::cout << "\n\n\nConnected to server!" << std::endl;
			ZeroMemory(mySocketBuffer, NETMESSAGE_SIZE);
			hasMessage = false;
		}
		else
		{
			std::cout << "\nUsername to long. (It has to be under 32 characters)" << std::endl;
			hasMessage = false;
		}
	}
	return C_SUCCESS;
}

int Client::Connect(const std::string& aUsername)
{
	if (isConnected)
	{
		ZeroMemory(mySocketBuffer, NETMESSAGE_SIZE);

		hasJoined = true;
		return C_SUCCESS;
	}


	if (aUsername.size() <= USERNAME_MAX_LENGTH)
	{
		TNP::ClientJoin message;
		message.messageID = myMessageCounter(static_cast<int>(TNP::MessageType::clientJoin));

		strcpy_s(message.username, USERNAME_MAX_LENGTH, aUsername.c_str());

		myClientName = aUsername;

		if (C_FAIL(SendClientMessage(message, sizeof(message))))
		{
			return C_CONNECTION_FAILED;
		}

		return C_CONNECTING;
	}
	else
	{
		std::cout << "\nUsername to long. (It has to be under 32 characters)" << std::endl;
		hasMessage = false;
		return C_UNAME_TOO_LONG;
	}
}

int Client::Run()
{
	std::cout << "Please enter a message (write quit to quit): \n";

	while (isRunning)
	{
		if (hasMessage)
		{
			if (strcmp(myMessage, "quit") == 0)
			{
				std::cout << "You have disconnected!" << std::endl;
				std::cout << "Press any key to close program." << std::endl;

				isRunning = false;
				break;
			}

			TNP::ClientMessage message;
			memcpy(message.message, myMessage, MESSAGE_MAX_SIZE);
			//message.message = myMessage;
			message.messageID = myMessageCounter(static_cast<int>(TNP::MessageType::clientMessage));

			if (C_FAIL(SendClientMessage(message, sizeof(message))))
			{
				std::cout << "Failed to send message" << std::endl;

				isRunning = false;
				break;
			}

			hasMessage = false;
		}

		ZeroMemory(mySocketBuffer, NETMESSAGE_SIZE);
		int addrServerSize = sizeof(myServerAddress);
		const int recv_len = recvfrom(myUDPSocket, mySocketBuffer, NETMESSAGE_SIZE, 0, (sockaddr*)&myServerAddress, &addrServerSize);

		if (recv_len == SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK)
		{
			std::cout << "Failed receiving data from udpSocket." << std::endl;
			std::cout << "Error: " << WSAGetLastError() << std::endl;
		}

		if (recv_len > 0)
		{
			if (C_FAIL(HandleRecievedMessage()))
			{
				std::cout << "Fail logged." << std::endl;
			}
		}
	}

	TNP::ClientDisconnect discMessage;
	discMessage.messageID = myMessageCounter(static_cast<int>(TNP::MessageType::clientDisconnect));

	if (SendClientMessage(discMessage, sizeof(discMessage)))
	{
		return EXIT_FAILURE;
	}

	return C_QUIT;
}

int Client::RecieveMessageFromServer()
{
	ZeroMemory(mySocketBuffer, NETMESSAGE_SIZE);
	int addrServerSize = sizeof(myServerAddress);
	const int recv_len = recvfrom(myUDPSocket, mySocketBuffer, NETMESSAGE_SIZE, 0, (sockaddr*)&myServerAddress, &addrServerSize);

	if (recv_len == SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK)
	{
		std::cout << "Failed receiving data from udpSocket." << std::endl;
		std::cout << "Error: " << WSAGetLastError() << std::endl;
	}

	if (recv_len > 0)
	{
		if (C_FAIL(HandleRecievedMessage()))
		{
			std::cout << "Fail logged." << std::endl;
		}
		return C_SUCCESS;
	}
	return C_QUIT;
}

int Client::Shutdown()
{
	closesocket(myUDPSocket);
	WSACleanup();
	isRunning = false;

	std::cout << "Client has shutdown\n";

	return C_QUIT;
}

int Client::SendPositionMessage()
{
	if (myPlayer == nullptr) return 0;


	TNP::ClientMovedMessage message;

	message.playerID = myPlayer->GetPID();
	message.position = myPlayer->GetPosition();
	message.messageID = myMessageCounter((int)TNP::MessageType::clientSendPosition);

	return SendClientMessage(message, sizeof(message));
}

int Client::SendStoredMessages()
{
	std::lock_guard<std::mutex> lock(mySendMessagesMutex);

	for (int i = 0; i < myMessagesToSend.size(); i++)
	{
		SendClientMessage(*myMessagesToSend[i].message, myMessagesToSend[i].messageSize);

		delete myMessagesToSend[i].message;
		myMessagesToSend.erase(myMessagesToSend.begin() + i);
		--i;
	}
	//RemoveStoredMessages();
	return C_SUCCESS;
}

void Client::RemoveStoredMessages()
{
	for (int i = 0; i < myMessagesToSend.size(); i++)
	{
		delete myMessagesToSend[i].message;
	}
	myMessagesToSend.clear();

}


void Client::StoreFlowerSpawnMessage()
{
	std::lock_guard<std::mutex> lock(mySendMessagesMutex);

	TNP::ClientSpawnFlower* message = new TNP::ClientSpawnFlower();
	message->position = myPlayer->GetPosition();
	message->messageID = myMessageCounter((int)TNP::MessageType::clientSpawnFlower);

	myStatManager->StoreMessage(*message, sizeof(*message));

	MessageToSendData messageToSendData(message, sizeof(TNP::ClientSpawnFlower));

	myMessagesToSend.push_back(messageToSendData);
}

void Client::StoreDestroyFlowerMessage(int aID)
{
	std::lock_guard<std::mutex> lock(mySendMessagesMutex);

	TNP::ClientDestroyFlower* message = new TNP::ClientDestroyFlower();
	message->id = aID;
	message->messageID = myMessageCounter((int)TNP::MessageType::clientDestoryFlower);

	myStatManager->StoreMessage(*message, sizeof(*message));

	MessageToSendData messageToSendData(message, sizeof(TNP::ClientDestroyFlower));

	myMessagesToSend.push_back(messageToSendData);
}

bool Client::HasMessagesToSend()
{
	return !myMessagesToSend.empty();
}

void Client::StorePlayerCommands(std::vector<PlayerCommandData>& someCommands)
{
	for (int i = 0; i < someCommands.size(); i++)
	{
		switch (someCommands[i].playerCommand)
		{
		case ePlayerCommands::Move:
		{

			break;
		}
		case ePlayerCommands::Interact:
		{

			break;
		}
		case ePlayerCommands::SpawnFlower:
		{
			StoreFlowerSpawnMessage();
			break;
		}	
		case ePlayerCommands::DestroyFlower:
		{
			std::cout << "Destroy flower \n";
			StoreDestroyFlowerMessage(someCommands[i].ID);
			break;
		}
		case ePlayerCommands::SetColor:
		{

			break;
		}
		default:
			break;
		}
	}
}

int Client::SendClientMessage(const TNP::Message& aMSG, const int aSize)
{
	// Unsure whether sending a reference to TNP::Message here leaves out other derived data and only send the message type...

	const char* msg = (char*)&aMSG;

	if (sendto(myUDPSocket, msg, aSize, 0, reinterpret_cast<sockaddr*>(&myServerAddress), sizeof(myServerAddress)) == SOCKET_ERROR)
	{
		std::cout << "Error: " << WSAGetLastError() << std::endl;
		return C_FAILED;
	}

	return C_SUCCESS;
}

int Client::HandleRecievedMessage()
{
	TNP::MessageType type = (TNP::MessageType)mySocketBuffer[0];

	TNP::Message* baseMessage = (TNP::Message*)mySocketBuffer;

	// If message is already acked, don't do it again
	if (myAckedMessages.count(baseMessage->messageID) > 0)
	{
		return 0;
	}

	int size = -1;

	switch (type)
	{
	case TNP::MessageType::error:
	{
		std::cout << "[ERROR] FAILED TO UNPACK MESSAGE (Message had type ERROR)" << std::endl;
		return C_FAILED;
	}
	case TNP::MessageType::serverClientJoined:
	{
		// Alternativly:
		TNP::ServerClientJoined* msg = (TNP::ServerClientJoined*)mySocketBuffer;
		msg->id;
		msg->username;

		size = sizeof(*msg);

		const int clientID = msg->id;

		myConnectedClients[clientID] = msg->username;

		Tga::Color color;
		UnpackColors(msg->color, color.myR, color.myG, color.myB, color.myA);

		std::cout << "Client " << msg->username << " joined the server!" << std::endl;
		Player* p = myPlayerManager->CreatePlayer(clientID, color);
		p->SetUsername(msg->username);
		p->debugColor = msg->color;
		p->debugCColor = color;
		break;
	}
	case TNP::MessageType::serverClientDisconnected:
	{
		TNP::ServerClientDisconnected* msg = (TNP::ServerClientDisconnected*)mySocketBuffer;

		size = sizeof(*msg);
		const int clientID = msg->id;

		std::cout << "Client " << myConnectedClients[clientID] << " disconnected the server!" << std::endl;

		myConnectedClients.erase(clientID);
		myPlayerManager->DeletePlayer(clientID);
		break;
	}
	case TNP::MessageType::serverClientMessage:
	{
		TNP::ServerClientMessage* msg = (TNP::ServerClientMessage*)mySocketBuffer;

		size = sizeof(*msg);
		std::string printStr = "[" + myConnectedClients[msg->id] + "] " + msg->message;

		std::cout << printStr << std::endl;
		break;
	}
	case TNP::MessageType::serverConnectedClientData:
	{
		TNP::ServerConnectedClientData* msg = (TNP::ServerConnectedClientData*)mySocketBuffer;

		size = sizeof(*msg);

		// flag game that connection was successful 
		isConnected = true;


		// Local Player init
		{
			Tga::Color color;
			UnpackColors(msg->myColour, color.myR, color.myG, color.myB, color.myA);

			myPlayer = myPlayerManager->CreatePlayer(msg->clientID, color, { 400,400 }, true);
			myPlayer->SetUsername(myClientName);
			myPlayer->debugColor = msg->myColour;
			myPlayer->debugCColor = color;
		}


		std::cout << "Connected clients: " << std::endl;

		char* ptr = msg->clients;
		for (int i = 0; i < msg->numberOfClients; i++)
		{
			// ID
			int id;
			{
				memcpy(&id, ptr, sizeof(int));

				ptr += sizeof(int);
			}

			// Color
			int color = 0;
			{
				memcpy(&color, ptr, sizeof(int));

				ptr += sizeof(int);
			}

			// Position
			Tga::Vector2f pos;
			{
				memcpy(&pos, ptr, sizeof(Tga::Vector2f));

				ptr += sizeof(Tga::Vector2f);
			}

			// Username
			char username[USERNAME_MAX_LENGTH] = { 0 };
			{
				ZeroMemory(username, USERNAME_MAX_LENGTH);

				memcpy(username, ptr, USERNAME_MAX_LENGTH);

				ptr += USERNAME_MAX_LENGTH;
			}


			// Use unpacked data
			{
				std::string name(username);

				myConnectedClients[id] = name;

				std::cout << name << std::endl;

				Tga::Color newColor;

				UnpackColors(color, newColor.myR, newColor.myG, newColor.myB, newColor.myA);

				Player* p = myPlayerManager->CreatePlayer(id, newColor, pos);
				p->SetUsername(name);
				p->debugColor = msg->myColour;
				p->debugCColor = newColor;
			}
		}

		break;
	}
	case TNP::MessageType::updateClients:
	{
		//TNP::UpdateClientsMessage* msg = (TNP::UpdateClientsMessage*)mySocketBuffer;

		// This needs to sync with all other clients so client should probably not recieve messages
		// Todo -> create a networkHandler or manager or whatever that recieves messages and parses 
		// them out to the correct reciever -- Olle

		TNP::UpdateClientsMessage* msg = (TNP::UpdateClientsMessage*)(mySocketBuffer);

		size = sizeof(*msg);
		TNP::UpdateClientsMessage::UnpackedUpdateClientMessage deserMsg(*msg);

		for (int i = 0; i < deserMsg.numberOfClients; i++)
		{
			if (deserMsg.clients[i].playerID == myPlayer->GetPID())
			{
				continue;
			}

			myPlayerManager->UpdatePlayer(deserMsg.clients[i].playerID, { deserMsg.clients[i].position });
		}

		//char* ptr = msg->data;

		//for (int i = 0; i < msg->numberOfClients; i++)
		//{

		//	unsigned int id = 0;
		//	memcpy(&id, ptr, sizeof(unsigned int));
		//	ptr += sizeof(unsigned int);

		//	if (id == (unsigned int)myPlayer->GetPID())
		//	{
		//		ptr += sizeof(Tga::Vector2f);
		//		continue;
		//	}

		//	Tga::Vector2f pos;
		//	memcpy(&pos, ptr, sizeof(Tga::Vector2f));
		//	ptr += sizeof(Tga::Vector2f);

		//	myPlayerManager->UpdatePlayer(id, { pos });
		//}


		break;
	}
	case TNP::MessageType::serverSpawnFlower:
	{
		TNP::ServerSpawnFlower* msg = (TNP::ServerSpawnFlower*)(mySocketBuffer);

		size = sizeof(*msg);
		myEntityFactory->CreateEntity(EntityType::flower, msg->id, msg->position);
		
		break;
	}
	case TNP::MessageType::serverDestroyFlower:
	{
		TNP::ServerDestroyFlower* msg = (TNP::ServerDestroyFlower*)(mySocketBuffer);

		size = sizeof(*msg);
		myEntityFactory->DeleteEntity(EntityType::flower, msg->id);
		
		break;
	}
	default:
		break;
	}

	myStatManager->StoreReceivedMessage(*baseMessage, size);

	SendAckMessage(*baseMessage);

	return C_SUCCESS;
}

void Client::SendAckMessage(const TNP::Message& aMessage)
{
	std::lock_guard<std::mutex> guard(myAckMutex);

	myAckedMessages.insert(std::pair<unsigned int, AckedMessage>(aMessage.messageID, { aMessage.messageID }));

	TNP::AckMessage ackMessage(aMessage.messageID);
	ackMessage.messageID = myMessageCounter(static_cast<int>(TNP::MessageType::ackMessage));

	SendClientMessage(ackMessage, sizeof(TNP::AckMessage));
}

void Client::UpdateAckedMessages(const float aDT)
{
	std::lock_guard<std::mutex> guard(myAckMutex);

	for (auto it = myAckedMessages.begin(); it != myAckedMessages.end();)
	{
		it->second.timeSinceAck += aDT;
		if (it->second.timeSinceAck >= myAckMessageSaveTime)
		{
			it = myAckedMessages.erase(it);
			continue;
		}

		++it;
	}
}
