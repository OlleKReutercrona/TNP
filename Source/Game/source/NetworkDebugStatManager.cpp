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

	myStats.receivedDPS = (float)receivedDataTotal / myMessageSaveTime;
	myStats.sentDPS = (float)sentDataTotal / myMessageSaveTime;
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
			case 0 :
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
		ImGui::Text("Ping: %lld", myStats.RTT.count());
		std::string sizeStr;
		float size = printDataFormated(myStats.sentDPS, sizeStr);
		ImGui::Text("Sent data per second (%s): %4.2f", sizeStr.c_str(), size);

		size = printDataFormated(myStats.receivedDPS, sizeStr);
		ImGui::Text("Received data per second (%s): %4.2f", sizeStr.c_str(), size);
		ImGui::Text("Sent messages per second: %.1f", (float)myStats.sentMessages / myMessageSaveTime);
		ImGui::Text("Received messages per second: %.1f", (float)myStats.receivedMessages / myMessageSaveTime);
		ImGui::Text("Packed loss: %i %", myStats.packetLoss);

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
