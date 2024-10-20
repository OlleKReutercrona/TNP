#include "stdafx.h"
#include "NetworkDebugStatManager.h"

TNP::NetworkDebugStatManager::NetworkDebugStatManager()
{
}

TNP::NetworkDebugStatManager::~NetworkDebugStatManager()
{
}

void TNP::NetworkDebugStatManager::Update(const float aDT)
{
	static float time = 0.0;

	time += aDT;
	if (time < 1.0f) { return; }
	time = 0.0f;

	auto timeNow = std::chrono::high_resolution_clock::now();

	myStats.messageStats.clear();

	int sentDataTotal = UnpackSentMessages();
	int receivedDataTotal = UnpackReceivedMessages();

	myStats.sentMessages = (int)mySentMessages.size();
	myStats.receivedMessages = (int)myReceivedMessages.size();

	myStats.receivedDPS = (float)receivedDataTotal / myMessageSaveTime;
	myStats.sentDPS = (float)sentDataTotal / myMessageSaveTime;

	myStats.RTT = UnpackPingMessages();
}

void TNP::NetworkDebugStatManager::StoreMessage(const TNP::Message& aMessage, const int aSize)
{
	if (mySentMessages.count(aMessage.messageID) > 0) { return; }

	auto& msg = mySentMessages[aMessage.messageID];

	msg.message.type = aMessage.type;
	msg.message.messageID = aMessage.messageID;
	msg.messageSize = aSize;
	msg.timeSent = std::chrono::high_resolution_clock::now();
}

void TNP::NetworkDebugStatManager::StoreReceivedMessage(const TNP::Message& aMessage, const int aSize)
{
	auto& debugMsg = myReceivedMessages.emplace_back();

	debugMsg.message.type = aMessage.type;
	debugMsg.message.messageID = aMessage.messageID;
	debugMsg.messageSize = aSize;
	debugMsg.timeSent = std::chrono::high_resolution_clock::now();
}

void TNP::NetworkDebugStatManager::StorePingMessage(const TNP::Message& aMessage)
{
	if (myUnackedPingMessages.count(aMessage.messageID) > 0)
	{
		return;
	}

	auto& statMSG = myUnackedPingMessages[aMessage.messageID];

	statMSG.timeSent = std::chrono::high_resolution_clock::now();
}

void TNP::NetworkDebugStatManager::ReceivePingMessage(const int aMessageID)
{
	if (myUnackedPingMessages.count(aMessageID) == 0)
	{
		return;
	}

	auto& ackedMSG = myAckedPingMessages[aMessageID];
	ackedMSG.ping = std::chrono::high_resolution_clock::now() - myUnackedPingMessages[aMessageID].timeSent;
	ackedMSG.timeSent = std::chrono::high_resolution_clock::now();

	myUnackedPingMessages.erase(aMessageID);
}

void TNP::NetworkDebugStatManager::RegisterMessageAsPacketLoss(const int aMessageID)
{
	if (mySentMessages.count(aMessageID) == 0)
	{
		return;
	}

	mySentMessages.at(aMessageID).isMissing = true;
}

void TNP::NetworkDebugStatManager::DisplayDebugStats()
{
	auto printDataFormated = [](const float aSize, std::string& aSizeStr)
		{
			float newSize = (float)aSize;
			int counter = 0;
			while (newSize / 1000.0f > 1.0f)
			{
				counter++;
				newSize /= 1000.0f;
			}

			switch (counter)
			{
			case 0:
			{
				aSizeStr = "Bytes";
				break;
			}
			case 1:
			{
				aSizeStr = "KB";
				break;
			}
			case 2:
			{
				aSizeStr = "MB";
				break;
			}
			default:
			{
				aSizeStr = "GB";
				break;
			}
			}

			return newSize;
		};


	ImGui::Begin("Network debug stats");
	{
		ImGui::Text("Ping: %4.2f ms", myStats.RTT);
		std::string sizeStr;
		float size = printDataFormated(myStats.sentDPS, sizeStr);
		ImGui::Text("Sent data per second (%s): %4.2f", sizeStr.c_str(), size);

		size = printDataFormated(myStats.receivedDPS, sizeStr);
		ImGui::Text("Received data per second (%s): %4.2f", sizeStr.c_str(), size);
		ImGui::Text("Sent messages per second: %.1f", (float)myStats.sentMessages / myMessageSaveTime);
		ImGui::Text("Received messages per second: %.1f", (float)myStats.receivedMessages / myMessageSaveTime);
		ImGui::Text("Packed loss: %i%%", myStats.packetLoss);

		ImGui::Spacing();

		ImGui::Text("Stats per message type");
		for (auto& [type, stat] : myStats.messageStats)
		{
			if (ImGui::TreeNode(MessageTypeNames[static_cast<int>(type)]))
			{
				size = printDataFormated((float)stat.sentData / myMessageSaveTime, sizeStr);
				ImGui::Text("Sent data per second (%s): %4.2f", sizeStr.c_str(), size);
				size = printDataFormated((float)stat.receivedData / myMessageSaveTime, sizeStr);
				ImGui::Text("Received data per second (%s): %4.2f", sizeStr.c_str(), size);
				ImGui::Spacing();
				ImGui::Text("Sent messages per second: %.1f", stat.sentMessages / myMessageSaveTime);
				ImGui::Text("Received messages per second: %.1f", stat.receivedMessages / myMessageSaveTime);

				ImGui::TreePop();
			}
		}
	}
	ImGui::End();
}

int TNP::NetworkDebugStatManager::UnpackReceivedMessages()
{
	auto timeNow = std::chrono::high_resolution_clock::now();

	int receivedDataTotal = 0;
	{
		for (int i = 0; i < myReceivedMessages.size(); i++)
		{
			DebugMessage& message = myReceivedMessages[i];

			auto timeSlice = std::chrono::duration<double>(timeNow - message.timeSent).count() / 1000.0f;

			if (timeSlice >= myMessageSaveTime)
			{
				myReceivedMessages.erase(myReceivedMessages.begin() + i);
				--i;
			}

			receivedDataTotal += message.messageSize;

			auto& typeStas = myStats.messageStats[message.message.type];
			typeStas.receivedMessages++;
			typeStas.receivedData += message.messageSize;
		}
	}
	return receivedDataTotal;
}

int TNP::NetworkDebugStatManager::UnpackSentMessages()
{
	auto timeNow = std::chrono::high_resolution_clock::now();

	int sentDataTotal = 0;
	int missingPackeges = 0;
	for (auto it = mySentMessages.begin(); it != mySentMessages.end();)
	{
		DebugMessage& message = it->second;

		auto timeSince = std::chrono::duration<double>(timeNow - message.timeSent).count() / 1000.0f;

		if (timeSince >= myMessageSaveTime)
		{
			it = mySentMessages.erase(it);
			continue;
		}

		if (message.isMissing)
		{
			missingPackeges++;
		}

		sentDataTotal += message.messageSize;

		auto& typeStas = myStats.messageStats[message.message.type];
		typeStas.sentMessages++;
		typeStas.sentData += message.messageSize;

		if (it == mySentMessages.end())
		{
			break;
		}

		++it;
	}

	myStats.packetLoss = (int)(((float)missingPackeges / (float)mySentMessages.size()) * 100);

	return sentDataTotal;
}

double TNP::NetworkDebugStatManager::UnpackPingMessages()
{
	auto timeNow = std::chrono::high_resolution_clock::now();

	double totalPing = 0.0f;
	int total = 0;

	for (auto it = myAckedPingMessages.begin(); it != myAckedPingMessages.end();)
	{
		PingMessage& message = it->second;

		auto timeSince = std::chrono::duration<double>(timeNow - message.timeSent).count() / 1000.0f;

		if (timeSince >= myMessageSaveTime)
		{
			it = myAckedPingMessages.erase(it);
			continue;
		}

		totalPing += message.ping.count();
		total++;

		++it;
	}

	return totalPing / (double)total;
}

