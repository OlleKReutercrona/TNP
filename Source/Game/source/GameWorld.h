#pragma once
#pragma message ("In GameWorld")
//#include "Client.h"
#include <thread>
class PlayerController;
class Player;

#define _RENDERDEBUG 0


struct MessageThreadData
{
};


class Client;

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
	


	Player* myPlayer;
	PlayerController* myController;
	Client* myClient;

	MessageThreadData myMessageThreadData;

	std::unordered_map<int, Player*> myOtherClients;
	std::thread myRecieveMessageThread;

	bool isRunning = true;

};