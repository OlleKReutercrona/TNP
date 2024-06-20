#pragma once
#include <tge/math/vector2.h>

class Player;

struct PlayerControllerData
{
	Tga::Vector2f inputDirection = { 0,0 };
};

struct InputMapper
{
	int up;
	int down;
	int left;
	int right;
};

class PlayerController 
{
public:
	PlayerController(Player* aPlayer);
	~PlayerController();


	void Update(const float& aDeltaTime);


	const Player& GetPlayer()
	{
		return *myPlayer;
	}

private:
	void UpdateControllerData();

private:
	
	PlayerControllerData myPlayerControllerData;
	InputMapper myInputMapper;
	Player* myPlayer;
};