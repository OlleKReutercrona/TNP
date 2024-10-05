#pragma once
#include <tge/math/vector2.h>

class Player;
class EntityFactory;

struct PlayerControllerData
{
	Tga::Vector2f inputDirection = { 0,0 };
	bool useAction = false;
	bool interactAction = false;
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
	PlayerController(Player* aPlayer, EntityFactory& anEntityFactory);
	~PlayerController();


	void Update(const float& aDeltaTime);


	const Player& GetPlayer()
	{
		return *myPlayer;
	}

private:
	void UpdateControllerData();

	int GetClosetFlowerID();

private:
	
	PlayerControllerData myPlayerControllerData;
	InputMapper myInputMapper;
	Player* myPlayer;
	HWND myHWND;




	EntityFactory* myEntityFactory;

	float pickUpDistance = 60.f;

};