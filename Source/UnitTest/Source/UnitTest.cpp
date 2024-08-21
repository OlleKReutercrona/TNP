#include "CppUnitTest.h"
#include <Server/Server.h>
#include <Client.h>


using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace TNP_UT
{
	TEST_CLASS(MESSSAGE_TEST) 
	{
public:

	TEST_METHOD(MESSAGE_CLIENT_JOINED) 
	{
		//Client* client = new Client();
		Server* server = new Server();

		TNP::ClientJoin message;
		std::string userName = "Olle";


		strcpy_s(message.username, USERNAME_MAX_LENGTH, userName.c_str());
		std::string clientname = server->UT_ProcessMessageClientJoin((char*)&message);
		Assert::AreEqual(userName, clientname);
		//Assert::AreEqual((std::string)"Olle", userName);


	}

	};
}