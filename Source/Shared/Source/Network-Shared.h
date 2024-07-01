#pragma once

#define NETMESSAGE_SIZE 512
#define USERNAME_MAX_LENGTH 32
#define MESSAGE_MAX_SIZE 256

enum class eNetworkCodes
{
	failed,
	connectionFailed,
	success,
	quit
};

static inline const char* EnumToString(const eNetworkCodes aCode)
{
	switch (aCode)
	{
	case eNetworkCodes::failed:
		return "Failed";
	case eNetworkCodes::connectionFailed:
		return "Connection Failed";	
	case eNetworkCodes::success:
		return "Success";	
	case eNetworkCodes::quit:
		return "Quit";
	default:
		break;
	}
}

static inline bool Failed(const eNetworkCodes aCode)
{
	return aCode == eNetworkCodes::failed;
}

static inline bool Failed(const int aCode)
{
	return aCode == -1;
}