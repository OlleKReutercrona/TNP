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

	int sentDataTotal = 0;
	for (auto it = mySentMessages.begin(); it != mySentMessages.end();)
	{
		DebugMessage& message = it->second;

		auto timeSince = std::chrono::duration<double>(timeNow - message.timeSent).count();

		if (timeSince >= myMessageSaveTime)
		{
			it = mySentMessages.erase(it);
			continue;
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

	int receivedDataTotal = 0;
	for (int i = 0; i < myReceivedMessages.size(); i++)
	{
		DebugMessage& message = myReceivedMessages[i];

		if (std::chrono::duration(timeNow - message.timeSent).count() >= myMessageSaveTime)
		{
			myReceivedMessages.erase(myReceivedMessages.begin() + i);
			--i;
		}

		receivedDataTotal += message.messageSize;

		auto& typeStas = myStats.messageStats[message.message.type];
		typeStas.receivedMessages++;
		typeStas.receivedData += message.messageSize;
	}

	myStats.sentMessages = (int)mySentMessages.size();
	myStats.receivedMessages = (int)myReceivedMessages.size();

	myStats.receivedDPS = receivedDataTotal / myMessageSaveTime;
	myStats.sentDPS = sentDataTotal / myMessageSaveTime;
}

void TNP::NetworkDebugStatManager::StoreMessage(const TNP::Message& aMessage, const int aSize)
{
	if (mySentMessages.count(aMessage.messageID) > 0) { return; }

	if (static_cast<int>(aMessage.type) > static_cast<int>(TNP::MessageType::count))
	{
		int hej = 0;
		hej++;
	}

	auto& msg = mySentMessages[aMessage.messageID];
	//msg.message = aMessage;
	msg.message.type = aMessage.type;
	msg.message.messageID = aMessage.messageID;
	msg.messageSize = aSize;
	msg.timeSent = std::chrono::high_resolution_clock::now();
}

void TNP::NetworkDebugStatManager::StoreReceivedMessage(const TNP::Message& aMessage, const int aSize)
{
	auto& debugMsg = myReceivedMessages.emplace_back();

	if (static_cast<int>(aMessage.type) > static_cast<int>(TNP::MessageType::count))
	{
		int hej = 0;
		hej++;
	}

	debugMsg.message.type = aMessage.type;
	debugMsg.message.messageID = aMessage.messageID;
	debugMsg.messageSize = aSize;
	debugMsg.timeSent = std::chrono::high_resolution_clock::now();
}

void TNP::NetworkDebugStatManager::DisplayDebugStats()
{
	ImGui::Begin("Network debug stats");
	{
		ImGui::Text("Ping: %lld", myStats.RTT.count());
		ImGui::Text("Sent data per second: %i", myStats.sentDPS);
		ImGui::Text("Received data per second: %i", myStats.receivedDPS);
		ImGui::Text("Sent messages: %i", myStats.sentMessages);
		ImGui::Text("Received messages: %i", myStats.receivedMessages);
		ImGui::Text("Packed loss: %i %", myStats.packetLoss);

		ImGui::Spacing();

		ImGui::Text("Stats per message type");
		for (auto& [type, stat] : myStats.messageStats)
		{
			if (ImGui::TreeNode(MessageTypeNames[static_cast<int>(type)]))
			{
				ImGui::Text("Sent data per second: %i", stat.sentData / myMessageSaveTime);
				ImGui::Text("Received data per second: %i", stat.receivedData / myMessageSaveTime);
				ImGui::Spacing();
				ImGui::Text("Sent messages: %i", stat.sentMessages);
				ImGui::Text("Received messages: %i", stat.receivedMessages);

				ImGui::TreePop();
			}
		}
	}
	ImGui::End();
}
