#include "stdafx.h"
#include "Player.h"
#include <tge/engine.h>
#include <tge/graphics/GraphicsEngine.h>
#include <tge/drawers/SpriteDrawer.h>
#include <tge/drawers/DebugDrawer.h>
#include <tge/texture/TextureManager.h>

Player::Player()
{
	myIsPlayerOne = true;
	myMovementSpeed = 300.f;
	myPosition = { 0,0 };
}

void Player::Init(const unsigned int anID, const Tga::Color& aColour, Tga::Vector2f aStartPosition, bool aIsPlayer)
{
	myServerPID = anID;

	myPosition = aStartPosition;
	myIsPlayerOne = aIsPlayer;
	auto& engine = *Tga::Engine::GetInstance();
	mySpriteData.sharedDataBody.myTexture = engine.GetTextureManager().GetTexture(L"Sprites/noHat.dds");
	mySpriteData.sharedDataHat.myTexture = engine.GetTextureManager().GetTexture(L"Sprites/hatAndMust.dds");


	mySpriteData.instanceDataHat.mySize = mySpriteData.sharedDataHat.myTexture->CalculateTextureSize() / 3.0f;
	mySpriteData.instanceDataBody.mySize = mySpriteData.sharedDataHat.myTexture->CalculateTextureSize() / 3.0f;
	//mySpriteData.instanceData.myPivot = { mySpriteData.instanceData.mySize.x / 2, mySpriteData.instanceData.mySize.y / 2 };
	
	mySpriteData.instanceDataHat.myPosition = myPosition;
	mySpriteData.instanceDataBody.myPosition = myPosition;

	mySpriteData.instanceDataHat.myColor = aColour;

	if (aIsPlayer)
	{
		myUsernameData.text.SetColor({ 0, 1, 0, 1 });
	}

}

void Player::SetUsername(std::string aUsername)
{
	myUsernameData.text.SetText(aUsername);
	myUsernameData.offset = { -myUsernameData.text.GetWidth() / 2, mySpriteData.instanceDataHat.mySize.y / 2 };
}

void Player::Update(const float& aDeltaTime, PlayerControllerData& aPlayerControllerData)
{
	myPosition += aPlayerControllerData.inputDirection * aDeltaTime * myMovementSpeed;
}

void Player::Render(Tga::SpriteDrawer& aSpriteDrawer)
{
	mySpriteData.instanceDataHat.myPosition = myPosition;
	mySpriteData.instanceDataBody.myPosition = myPosition;
	myUsernameData.text.SetPosition(myPosition + myUsernameData.offset);

	aSpriteDrawer.Draw(mySpriteData.sharedDataBody, mySpriteData.instanceDataBody);
	aSpriteDrawer.Draw(mySpriteData.sharedDataHat, mySpriteData.instanceDataHat);
	myUsernameData.text.Render();
}

void Player::DebugRender(Tga::DebugDrawer& aDebugDrawer)
{
	aDebugDrawer.DrawCircle(mySpriteData.instanceDataHat.myPosition + mySpriteData.instanceDataHat.myPivot, 10.f, { 0,1,1,1 });
}

void Player::ExecuteCommand(ePlayerCommands aCommand)
{
	switch (aCommand)
	{
	case ePlayerCommands::Move:
	{

		break;
	}
	case ePlayerCommands::Interact:
	{

		break;
	}	
	case ePlayerCommands::SetColor:
	{

		break;
	}	
	default:
		break;
	}

}
