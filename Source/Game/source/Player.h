#pragma once
#include <tge/math/vector2.h>
#include <tge/sprite/sprite.h>
#include <tge/text/text.h>
#include "PlayerController.h"
#include "PlayerCommands.h"

namespace Tga
{
	class SpriteDrawer;
	class DebugDrawer;
}



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
	void Init(const unsigned int anID, const Tga::Color& aColour, Tga::Vector2f aStartPosition);
	void SetUsername(std::string aUsername);


	void Update(const float& aDeltaTime, PlayerControllerData& aPlayerControllerData);
	void Render(Tga::SpriteDrawer& aSpriteDrawer);
	void DebugRender(Tga::DebugDrawer& aDebugDrawer);

	void StoreCommand(PlayerCommandData aCommand);
	bool HasCommands();
	void ClearCommands();

	std::vector<PlayerCommandData>& GetCommands() { return myCommands; }

	const inline int GetPID() { return myServerPID; }
	const inline Tga::Vector2f& GetPosition() { return myPosition; }


	Tga::Color debugCColor;
	int debugColor = 0;
private:
	std::vector<PlayerCommandData> myCommands;
	Tga::Vector2f myPosition;	
	SpriteData mySpriteData;
	UsernameData myUsernameData;
	float myMovementSpeed;
	int myServerPID = -1;

};
