#pragma once

class PlayerController;
class Player;


#define _RENDERDEBUG 0

class GameWorld
{
public:
	GameWorld(); 
	~GameWorld();

	void Init();
	void Update(float aTimeDelta); 
	void Render();
private:

	Player* myPlayer;
	PlayerController* myController;

};