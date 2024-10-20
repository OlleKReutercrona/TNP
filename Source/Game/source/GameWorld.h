#pragma once
#include "Client.h"
#include "PlayerManager.h"
#include "EntityFactory.h"

class PlayerController;
class Player;

#define _RENDERDEBUG 0


struct MessageThreadData
{
};

namespace Tga
{
	class InputManager;
	class Text;
}

class GameWorld
{
public:
	GameWorld(); 
	~GameWorld();

	void Init(Tga::InputManager& aInputManager);
	void Update(float aTimeDelta);
	bool ConnectClient(float aTimeDelta);

	void Render();

	void StartSendMessageThread();

private:
	
	EntityFactory myEntityFactory;
	PlayerManager myPlayerManager;

	std::string myUserName = "";
	PlayerController* myController = nullptr;
	Client myClient;

	MessageThreadData myMessageThreadData;

	std::unordered_map<int, Player*> myOtherClients;
	std::thread mySendMessageThread;

	bool isRunning = true;
	Tga::InputManager* myInputManager;

	Tga::Text* myDisplayText;
	Tga::Text* myInputText;

	Tga::Vector2f myResolution;
};