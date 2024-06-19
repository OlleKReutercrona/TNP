#include "../Server/Server.h"
#include <iostream>

int main()
{
	Server server;

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