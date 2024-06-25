#include "Server\Server.h"
#include <iostream>

#include "Message.h"
#include "CircularBuffer.h"

int main()
{
	//TNP::CircularBuffer<TNP::Message> cBuffer;

	//for (int i = 0; i < 10; i++)
	//{
	//	TNP::ClientMessage* msg = new TNP::ClientMessage();

	//	std::string messageStr = "Hej " + std::to_string(i);
	//	for (int j = 0; j < messageStr.length(); j++)
	//	{
	//		msg->message[j] = messageStr[j];
	//	}

	//	cBuffer.Add(msg);
	//}

	//auto& buffer = cBuffer.GetAll();
	//for (int i = 0; i < NUM_OF_OBJECTS; i++)
	//{
	//	TNP::Message* msg = buffer[i];
	//	TNP::ClientMessage* clientMsg = static_cast<TNP::ClientMessage*>(msg);

	//	std::string str(clientMsg->message);

	//	std::cout << ("Slot %i", i) << " has value: "  << str << "\n";
	//}

	Server server;
	//int ID = server.CreateID(0);
	//std::cout << ID << "\n";

	if (S_FAIL(server.Start()))
	{
		std::cout << "!!!!!!!!!!!!   Server failed to start.   !!!!!!!!!!!!!!!" << std::endl;
	}

	std::cout << "Server is active!" << std::endl;

	if (S_FAIL(server.Run()))
	{
		std::cout << "!!!!!!!!!!!!   Server failed to Run.   !!!!!!!!!!!!!!!" << std::endl;
	}

	if (S_FAIL(server.Shutdown()))
	{
		std::cout << "!!!!!!!!!!!!   Server failed to shutdown.   !!!!!!!!!!!!!!!" << std::endl;
	}

	return 0;
}