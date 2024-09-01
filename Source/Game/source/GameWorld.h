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

class GameWorld
{
public:
	GameWorld(); 
	~GameWorld();

	void Init();
	void Update(float aTimeDelta); 
	void Render();

	void StartRecieveMessageThread();

private:
	
	EntityFactory myEntityFactory;
	PlayerManager myPlayerManager;

	PlayerController* myController;
	Client myClient;

	MessageThreadData myMessageThreadData;

	std::unordered_map<int, Player*> myOtherClients;
	std::thread myRecieveMessageThread;

	bool isRunning = true;

};