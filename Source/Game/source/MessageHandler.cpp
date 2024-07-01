#include "stdafx.h"
#include "MessageHandler.h"
#include <WS2tcpip.h>

TNP::MessageHandler::MessageHandler()
{
}

TNP::MessageHandler::~MessageHandler()
{
}

eNetworkCodes TNP::MessageHandler::Shutdown()
{
	closesocket(myUDPSocket);
	WSACleanup();
	isRunning = false;

	myRecieveThread.detach();

	return eNetworkCodes::quit;
}

eNetworkCodes TNP::MessageHandler::Connect(const std::string& aName)
{
	if (aName.length() > USERNAME_MAX_LENGTH) return eNetworkCodes::failed;

	TNP::ClientJoin message;

	strcpy_s(message.username, USERNAME_MAX_LENGTH, aName.c_str());

	const char* msg = (char*)&message;

	// Send a message to connect to server
	if (sendto(myUDPSocket, msg, sizeof(message), 0, reinterpret_cast<sockaddr*>(&myServerAddress), sizeof(myServerAddress)) == SOCKET_ERROR)
	{
		std::cout << "Error: " << WSAGetLastError() << std::endl;
		return eNetworkCodes::connectionFailed;
	}

	return eNetworkCodes::success;
}

eNetworkCodes TNP::MessageHandler::Start()
{
	std::cout << "Starting Winsock...";
	if (WSAStartup(MAKEWORD(2, 2), &myWinSockData) != 0)
	{
		std::cout << " FAIL!" << std::endl;
		std::cout << "Error: " << WSAGetLastError() << std::endl;
		return eNetworkCodes::failed;
	}
	std::cout << "OK!" << std::endl;

	myUDPSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (myUDPSocket == INVALID_SOCKET)
	{
		std::cout << "Failed to create udpSocket." << std::endl;
		std::cout << "Error: " << WSAGetLastError() << std::endl;
		return eNetworkCodes::failed;
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

	//if (CheckIfFailed(StartRecieveThread()))
	//{
	//    std::cout << "Recieve thread failed to start (also, how??)" << std::endl;
	//    return eNetworkCodes::failed;
	//}
	isRunning = true;

	return eNetworkCodes::success;
}

void TNP::MessageHandler::Subscribe(const TNP::NetworkedClient* aClient)
{
	if (myConnectedClients.contains(aClient->GetID())) { std::cout << "ID already subscribed!!"; return; }

	// Init ID
	myConnectedClients[aClient->GetID()] = {};
}

void TNP::MessageHandler::Unsubscribe(const NetworkedClient* aClient)
{
	if (!myConnectedClients.contains(aClient->GetID())) { std::cout << "ID NOT subscribed!!"; return; }

	// Remove
	myConnectedClients.erase(aClient->GetID());
}

void TNP::MessageHandler::Update()
{
	myTimer.Update();


	// Check for messages outside of the tickrate first
	{
		hasMessage = true;
		while (hasMessage)
		{
			ZeroMemory(mySocketBuffer, NETMESSAGE_SIZE);
			int addrServerSize = sizeof(myServerAddress);
			const int recv_len = recvfrom(myUDPSocket, mySocketBuffer, NETMESSAGE_SIZE, 0, (sockaddr*)&myServerAddress, &addrServerSize);

			if (recv_len == SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK)
			{
				std::cout << "Failed receiving data from udpSocket." << std::endl;
				std::cout << "Error: " << WSAGetLastError() << std::endl;
			}

			if (recv_len <= 0)
			{
				// If no message are recieved, stop looking
				hasMessage = false;
				break;
			}

			if (Failed(HandleRecievedMessage(mySocketBuffer)))
			{
				std::cout << "Failed to parse message." << std::endl;
			}
		}
	}



	// Tick based client actions (sending & recieving)
	{
		myTimeSinceLastTick += myTimer.GetDeltaTime();

		if (myTimeSinceLastTick < myTickTimeStep) { return; }

		myTimeSinceLastTick = 0.0f;

		// Send messages to clients
		UpdateClients();


		// Send messages to server
		mySendThread = std::thread(&MessageHandler::SendMessages, this);


		mySendThread.join();
	}
}

eNetworkCodes TNP::MessageHandler::SendMessages()
{
	for (unsigned int i = 0; i < myMessagesToSend.size(); i++)
	{
		eNetworkCodes code = SendMessage(&myMessagesToSend[i]);

		if (code == eNetworkCodes::failed)
		{
			// handle if message failed to send
		}
	}

	myMessagesToSend.clear();


	return eNetworkCodes::success;
}

eNetworkCodes TNP::MessageHandler::SendMessage(Package* aMessageToSend)
{
	const char* msg = (char*)aMessageToSend->message;

	if (sendto(myUDPSocket, msg, aMessageToSend->size, 0, reinterpret_cast<sockaddr*>(&myServerAddress), sizeof(myServerAddress)) == SOCKET_ERROR)
	{
		delete aMessageToSend->message;
		aMessageToSend->message = nullptr;

		std::cout << "Error: " << WSAGetLastError() << std::endl;
		return eNetworkCodes::failed;
	}

	delete aMessageToSend->message;
	aMessageToSend->message = nullptr;

	return eNetworkCodes::success;
}

void TNP::MessageHandler::UpdateClients()
{
	for (unsigned int i = 0; i < myRecievedMessages.size(); i++)
	{
		ParseMessage(myRecievedMessages[i]);
		delete myRecievedMessages[i];
	}

	myRecievedMessages.clear();
}

eNetworkCodes TNP::MessageHandler::QueueMessage(Package& aMessageToSend)
{
	myMessagesToSend.push_back(aMessageToSend);


	return eNetworkCodes::success;
}

eNetworkCodes TNP::MessageHandler::StartRecieveThread()
{
	myRecieveThread = std::thread([&]
		{
			while (isRunning)
			{
				// Recieve messages here!
				// Bit write it single threaded first
			}
		}
	);

	return eNetworkCodes::success;
}

eNetworkCodes TNP::MessageHandler::HandleRecievedMessage(const char* aMessage)
{
	TNP::MessageType type = (TNP::MessageType)aMessage[0];

	switch (type)
	{
	case TNP::MessageType::error:
		break;
	case TNP::MessageType::serverConnectedClientData:
	{
		// Copy message
		TNP::ServerConnectedClientData* msg = new TNP::ServerConnectedClientData();
		msg->Deserialize(mySocketBuffer);

		myRecievedMessages.push_back(msg);
		break;
	}
	case TNP::MessageType::serverClientJoined:
	{
		// Copy message
		TNP::ServerClientJoined* msg = new TNP::ServerClientJoined();
		msg->Deserialize(mySocketBuffer);

		myRecievedMessages.push_back(msg);
		break;
	}
	case TNP::MessageType::serverClientDisconnected:
	{
		// Copy message
		TNP::ServerClientDisconnected* msg = new TNP::ServerClientDisconnected();
		msg->Deserialize(mySocketBuffer);

		myRecievedMessages.push_back(msg);
		break;
	}
	case TNP::MessageType::serverClientMessage:
	{
		// Copy message
		TNP::ServerClientMessage* msg = new TNP::ServerClientMessage();
		msg->Deserialize(mySocketBuffer);

		myRecievedMessages.push_back(msg);
		break;
	}
	case TNP::MessageType::serverBundle:
	{
		// Copy message
		TNP::ServerMessageBundle* msg = new TNP::ServerMessageBundle(*(TNP::ServerMessageBundle*)mySocketBuffer);

		myRecievedMessages.push_back(msg);
		break;
	}
	case TNP::MessageType::updateClients:
	{		
		// Copy message
		TNP::UpdateClientsMessage* msg = new TNP::UpdateClientsMessage();
		msg->Deserialize(mySocketBuffer);

		myRecievedMessages.push_back(msg);

		break;
	}
	default:
		break;
	}


	return eNetworkCodes();
}

eNetworkCodes TNP::MessageHandler::ParseMessage(const TNP::Message* aMessage)
{
	TNP::MessageType type = aMessage->type;

	switch (type)
	{
	case TNP::MessageType::error:
	{
		std::cout << "[ERROR] FAILED TO UNPACK MESSAGE (Message had type ERROR)" << std::endl;
		return eNetworkCodes::failed;
	}
	case TNP::MessageType::serverClientJoined:
	{
		// Alternativly:
		TNP::ServerClientJoined* msg = (TNP::ServerClientJoined*)aMessage;

		// Create new client here
		// Subscribe(myClientFactory->CreateClient(msg->id, msg->username));

		msg->id;
		msg->username;

		//const int clientID = msg->id;


		// Old, remove this after client factory is done
		//myConnectedClients[clientID] = msg->username;
		//myConnectedClients[clientID] = msg->id;

		std::cout << "Client " << msg->username << " joined the server!" << std::endl;
		break;
	}
	case TNP::MessageType::serverClientDisconnected:
	{
		TNP::ServerClientDisconnected* msg = (TNP::ServerClientDisconnected*)mySocketBuffer;

		const int clientID = msg->id;

		myConnectedClients.at(clientID)->Disconnect();

		std::cout << "Client " << myConnectedClients[clientID] << " disconnected the server!" << std::endl;

		Unsubscribe(myConnectedClients.at(clientID));

		//myConnectedClients.erase(clientID);
		break;
	}
	//case TNP::MessageType::serverClientMessage:
	//{
	//	TNP::ServerClientMessage* msg = (TNP::ServerClientMessage*)mySocketBuffer;

	//	std::string printStr = "[" + myConnectedClients[msg->id].GetName() + "] " + msg->message;

	//	std::cout << printStr << std::endl;
	//	break;
	//}
	case TNP::MessageType::serverConnectedClientData:
	{
		TNP::ServerConnectedClientData* msg = (TNP::ServerConnectedClientData*)mySocketBuffer;

		std::cout << "Connected clients: " << std::endl;

		char* ptr = msg->clients;
		for (int i = 0; i < msg->numberOfClients; i++)
		{
			int id;
			memcpy(&id, ptr, sizeof(int));

			ptr += sizeof(int);

			char username[USERNAME_MAX_LENGTH];

			memcpy(username, ptr, USERNAME_MAX_LENGTH);

			ptr += USERNAME_MAX_LENGTH;

			std::string name(username);

			//Subscribe(myClientFactory->CreateClient(id, name));

			// myConnectedClients[id] = name;

			std::cout << name << std::endl;

		}

		break;
	}
	case TNP::MessageType::updateClients:
	{
		TNP::UpdateClientsMessage* msg = (TNP::UpdateClientsMessage*)mySocketBuffer;

		for (unsigned int i = 0; i < (unsigned int)msg->numberOfClients; i++)
		{
			ClientUpdatePackage package;
			package.position = msg->myData[i].position;

			myConnectedClients.at(msg->myData[i].playerID)->ServerUpdate(package);
		}


		break;
	}
	default:
		break;
	}

	return eNetworkCodes::success;
}
