#pragma once
#include "Message.h"

#include <chrono>
#include <unordered_map>

class UnAckedMessage
{
public:
	UnAckedMessage(const std::shared_ptr<TNP::Message> aMessage, const int aMessageSize) : myMessage(aMessage), mySentTime(std::chrono::high_resolution_clock::now()), myMesssageSize(aMessageSize) {}

	double GetTimeSince()
	{
		return std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - mySentTime).count();
	}
	double GetTimeSince(const std::chrono::high_resolution_clock::time_point& aTimePoint)
	{
		return std::chrono::duration<double>(aTimePoint - mySentTime).count();
	}

	float myTimeSinceFirstAttempt = 0.0f;
	float myTimeSinceLastAttempt = 0.0f;
	const std::chrono::high_resolution_clock::time_point mySentTime;
	const std::shared_ptr<TNP::Message> myMessage;
	const int myMesssageSize;
	unsigned int myAttempts = 0;
};

struct AckedMessage
{
	AckedMessage(const int aMessageID) : messageID(aMessageID) {}

	const int messageID = -1;
	float timeSinceAck = 0.0f;
};