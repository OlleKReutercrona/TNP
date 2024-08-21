#include "stdafx.h"

#include "PlayerController.h"
#include "Player.h"
#include "Global.h"


PlayerController::PlayerController(Player* aPlayer)
{
	myPlayer = aPlayer;
	if (myPlayer->GetIsPlayerOne())
	{
		myInputMapper.up = 0x57;
		myInputMapper.down = 0x53;
		myInputMapper.left = 0x41;
		myInputMapper.right = 0x44;
	}
	else
	{
		myInputMapper.up = 0x26;
		myInputMapper.down = 0x28;
		myInputMapper.left = 0x25;
		myInputMapper.right = 0x27;
	}
}

PlayerController::~PlayerController()
{
}

void PlayerController::Update(const float& aDeltaTime)
{

	UpdateControllerData();




	myPlayer->Update(aDeltaTime, myPlayerControllerData);

}

void PlayerController::UpdateControllerData()
{
	myPlayerControllerData.inputDirection = { 0,0 };

	if (isWindowActive == 0) return;

	if (GetAsyncKeyState(myInputMapper.up))
		myPlayerControllerData.inputDirection += { 0, 1 };
	if (GetAsyncKeyState(myInputMapper.down))
		myPlayerControllerData.inputDirection += { 0, -1 };
	if (GetAsyncKeyState(myInputMapper.left))
		myPlayerControllerData.inputDirection += { -1, 0 };
	if (GetAsyncKeyState(myInputMapper.right))
		myPlayerControllerData.inputDirection += {  1, 0 };

	myPlayerControllerData.inputDirection.Normalize();
}
