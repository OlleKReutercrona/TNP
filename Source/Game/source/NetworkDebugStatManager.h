#pragma once
#include "Message.h"
/*
o RTT (Round trip time)
o Skickad data per sekund i (bytes per sekund).
o Mottagen data per sekund i (bytes per sekund).
o Antal skickade meddelanden
o Antal mottagna meddelanden
o Packet loss i procent.
o Statistik per meddelande-typ
	 Skickad/mottagen data per sekund
	 Antal per sekund

 Spara data för senaste 10 s och ta genomsnittet
o I 60 fps, 60*10 paket som behöver sparas för debuggning
*/
namespace TNP
{
	struct DebugMessage
	{
		Message message;
		int messageSize = 0;
		std::chrono::high_resolution_clock::time_point timeSent = {};
		bool isMissing = false;
	};

	struct PingMessage : public DebugMessage
	{
		std::chrono::milliseconds ping = {};
	};

	struct NetworkMessageStats
	{
		int sentData = 0;
		int receivedData = 0;
		int receivedMessages = 0;
		int sentMessages = 0;
	};

	struct NetworkStats
	{
		double RTT;
		float sentDPS = 0.0f;
		float receivedDPS = 0.0f; 
		int sentMessages = 0;
		int receivedMessages = 0;
		int packetLoss = 0;
		std::map<MessageType, NetworkMessageStats> messageStats;
	};


	class NetworkDebugStatManager
	{ 
	public:
		NetworkDebugStatManager();
		~NetworkDebugStatManager();
		
		void Update(const float aDT);

		void StoreMessage(const TNP::Message& aMessage, const int aSize);
		void StoreReceivedMessage(const TNP::Message& aMessage, const int aSize);
		void StorePingMessage(const TNP::Message& aMessage);
		void ReceivePingMessage(const int aMessageID);
		void RegisterMessageAsPacketLoss(const int aMessageID);
		
		void DisplayDebugStats();
	private:
		int UnpackReceivedMessages();
		int UnpackSentMessages();
		double UnpackPingMessages();

		std::vector<DebugMessage> myReceivedMessages;
		std::map<int, DebugMessage> mySentMessages;
		std::map<int, DebugMessage> myUnackedPingMessages;
		std::map<int, PingMessage> myAckedPingMessages;
		NetworkStats myStats;

		int myTimeFilter = 1;
		float myMessageSaveTime = 2.0f;
		float myUpdateTime = 0.33f;
	};
}

