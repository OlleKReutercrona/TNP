#include <tge/engine.h>
#include <tge/graphics/GraphicsEngine.h>
#include <tge/drawers/SpriteDrawer.h>
#include <tge/texture/TextureManager.h>

#include "Player.h"
Player::Player()
{
	
}

void Player::Init(Tga::Vector2f aStartPosition)
{
	myPosition = aStartPosition;

	auto& engine = *Tga::Engine::GetInstance();
	mySpriteData.sharedData.myTexture = engine.GetTextureManager().GetTexture(L"Sprites/CuteLittleMexican.dds");


	mySpriteData.instanceData.mySize = mySpriteData.sharedData.myTexture->CalculateTextureSize() / 3.0f;
	
	mySpriteData.instanceData.myPosition = myPosition;
	myMovementSpeed = 300.f;

}

void Player::Update(const float& aDeltaTime, PlayerControllerData& aPlayerControllerData)
{
	myPosition += aPlayerControllerData.inputDirection * aDeltaTime * myMovementSpeed;
	//std::cout << "InputDir: X" << aPlayerControllerData.inputDirection.x << " | " << aPlayerControllerData.inputDirection.y << std::endl;
	//std::cout << "DeltaTime: X" << aDeltaTime << std::endl;

	mySpriteData.instanceData.myPosition = myPosition;

}

void Player::Render(Tga::SpriteDrawer& aSpriteDrawer)
{
	aSpriteDrawer.Draw(mySpriteData.sharedData, mySpriteData.instanceData);
}
