#include <tge/engine.h>
#include <tge/graphics/GraphicsEngine.h>
#include <tge/drawers/SpriteDrawer.h>
#include <tge/texture/TextureManager.h>

#include "Player.h"
Player::Player()
{
	myIsPlayerOne = true;
	myMovementSpeed = 300.f;
	myPosition = { 0,0 };
}

void Player::Init(Tga::Vector2f aStartPosition, bool aIsPlayer)
{
	myPosition = aStartPosition;
	myIsPlayerOne = aIsPlayer;
	auto& engine = *Tga::Engine::GetInstance();
	mySpriteData.sharedData.myTexture = engine.GetTextureManager().GetTexture(L"Sprites/CuteLittleMexican.dds");


	mySpriteData.instanceData.mySize = mySpriteData.sharedData.myTexture->CalculateTextureSize() / 3.0f;
	//mySpriteData.instanceData.myPivot = { mySpriteData.instanceData.mySize.x / 2, mySpriteData.instanceData.mySize.y / 2 };
	
	mySpriteData.instanceData.myPosition = myPosition;
	if (myIsPlayerOne)
		mySpriteData.instanceData.myColor = { 0,1,1,1 };
	else
		mySpriteData.instanceData.myColor = { 1,1,0,1 };
	

}

void Player::SetUsername(std::string aUsername)
{
	myUsernameData.text.SetText(aUsername);
	myUsernameData.offset = { -myUsernameData.text.GetWidth() / 2, mySpriteData.instanceData.mySize.y / 2 };
}

void Player::Update(const float& aDeltaTime, PlayerControllerData& aPlayerControllerData)
{
	myPosition += aPlayerControllerData.inputDirection * aDeltaTime * myMovementSpeed;
	//std::cout << "InputDir: X" << aPlayerControllerData.inputDirection.x << " | " << aPlayerControllerData.inputDirection.y << std::endl;
	//std::cout << "DeltaTime: X" << aDeltaTime << std::endl;

	mySpriteData.instanceData.myPosition = myPosition;
	myUsernameData.text.SetPosition(myPosition + myUsernameData.offset);

}

void Player::Render(Tga::SpriteDrawer& aSpriteDrawer)
{
	aSpriteDrawer.Draw(mySpriteData.sharedData, mySpriteData.instanceData);
	myUsernameData.text.Render();
}

void Player::DebugRender(Tga::DebugDrawer& aDebugDrawer)
{
	aDebugDrawer.DrawCircle(mySpriteData.instanceData.myPosition + mySpriteData.instanceData.myPivot, 10.f, { 0,1,1,1 });
}
