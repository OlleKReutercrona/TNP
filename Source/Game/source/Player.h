#pragma once
#include <tge/math/vector2.h>
#include <tge/sprite/sprite.h>
#include <tge/text/text.h>
#include "PlayerController.h"

namespace Tga
{
	class SpriteDrawer;
	class DebugDrawer;
}

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
	friend class PlayerManager;
public:
	Player();
	~Player() = default;
	void Init(const unsigned int anID, const Tga::Color& aColour, Tga::Vector2f aStartPosition, bool aIsPlayerOne);
	void SetUsername(std::string aUsername);


	void Update(const float& aDeltaTime, PlayerControllerData& aPlayerControllerData);
	void Render(Tga::SpriteDrawer& aSpriteDrawer);
	void DebugRender(Tga::DebugDrawer& aDebugDrawer);



	void ExecuteCommand(ePlayerCommands aCommand);



	const inline int GetPID() { return myServerPID; }
	const inline Tga::Vector2f& GetPosition() { return myPosition; }

	const bool& GetIsPlayerOne()
	{
		return myIsPlayerOne;
	}

	int debugColor = 0;
	Tga::Color debugCColor;
private:

	Tga::Vector2f myPosition;	
	SpriteData mySpriteData;
	UsernameData myUsernameData;
	float myMovementSpeed;
	int myServerPID = -1;
	bool myIsPlayerOne;

};
