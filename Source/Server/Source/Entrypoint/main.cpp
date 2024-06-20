#include "Server\Server.h"
#include <iostream>

int main()
{
/*	TestSomething test;
	test.Hej();*/

	Server server;
	int ID = server.CreateID(0);
	std::cout << ID << "\n";

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