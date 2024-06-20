#pragma once
#include <tge/math/vector2.h>
#include <tge/sprite/sprite.h>
#include <tge/drawers/SpriteDrawer.h>
#include <tge/drawers/DebugDrawer.h>
#include <tge/text/text.h>
#include "PlayerController.h"


enum class ePlayerCommands
{
	Move,
	Interact,
	SetColor
};


struct UsernameData
{
	std::string username;
	Tga::Text text;
	Tga::Vector2f offset;
};

struct SpriteData 
{
	Tga::SpriteSharedData sharedDataHat;
	Tga::Sprite2DInstanceData instanceDataHat;

	Tga::SpriteSharedData sharedDataBody;
	Tga::Sprite2DInstanceData instanceDataBody;
};

class Player 
{
public:
	Player();
	~Player() = default;
	void Init(Tga::Vector2f aStartPosition, bool aIsPlayerOne);
	void SetUsername(std::string aUsername);


	void Update(const float& aDeltaTime, PlayerControllerData& aPlayerControllerData);
	void Render(Tga::SpriteDrawer& aSpriteDrawer);
	void DebugRender(Tga::DebugDrawer& aDebugDrawer);



	void ExecuteCommand(ePlayerCommands aCommand);





	const bool& GetIsPlayerOne()
	{
		return myIsPlayerOne;
	}

private:
	Tga::Vector2f myPosition;	
	SpriteData mySpriteData;
	UsernameData myUsernameData;
	float myMovementSpeed;
	bool myIsPlayerOne;

};
