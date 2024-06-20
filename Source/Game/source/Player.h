#pragma once
#include <tge/math/vector2.h>
#include <tge/sprite/sprite.h>
#include <tge/drawers/SpriteDrawer.h>

#include "PlayerController.h"

struct SpriteData 
{
	Tga::SpriteSharedData sharedData;
	Tga::Sprite2DInstanceData instanceData;
};

class Player 
{
public:
	Player();
	~Player() = default;
	void Init(Tga::Vector2f aStartPosition);

	void Update(const float& aDeltaTime, PlayerControllerData& aPlayerControllerData);
	void Render(Tga::SpriteDrawer& aSpriteDrawer);


private:
	Tga::Vector2f myPosition;

	float myMovementSpeed;


	SpriteData mySpriteData;
};
