#pragma once
#include "Client.h"
#include "PlayerManager.h"
#include "EntityFactory.h"
#include "NetworkDebugStatManager.h"

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
private:
	TNP::NetworkDebugStatManager myStatManager;

	EntityFactory myEntityFactory;
	PlayerManager myPlayerManager;

	std::string myUserName = "";
	PlayerController* myController = nullptr;
	Client myClient;


	std::unordered_map<int, Player*> myOtherClients;

	Tga::InputManager* myInputManager;

	Tga::Text* myDisplayText;
	Tga::Text* myInputText;

	Tga::Vector2f myResolution;
	MessageThreadData myMessageThreadData;
	bool isRunning = true;
};