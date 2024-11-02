#include "stdafx.h"

#include "PlayerController.h"
#include "Player.h"
#include "Global.h"
#include "EntityFactory.h"
#include "Entity.h"

#include "imgui/imgui.h"

PlayerController::PlayerController(Player* aPlayer, EntityFactory& anEntityFactory)
{
	myPlayer = aPlayer;

	myInputMapper.up = 0x57;
	myInputMapper.down = 0x53;
	myInputMapper.left = 0x41;
	myInputMapper.right = 0x44;


	myEntityFactory = &anEntityFactory;
}

PlayerController::~PlayerController()
{
}

void PlayerController::Update(const float& aDeltaTime)
{

	UpdateControllerData();

	myPlayer->Update(aDeltaTime, myPlayerControllerData);

	if (myPlayerControllerData.hasMoved)
	{
		PlayerCommandData pcd;
		pcd.playerCommand = ePlayerCommands::Move;
		myPlayer->StoreCommand(pcd);
	}

	if (myPlayerControllerData.useAction)
	{
		myPlayerControllerData.useAction = false;
		PlayerCommandData pcd;
		pcd.playerCommand = ePlayerCommands::SpawnFlower;
		myPlayer->StoreCommand(pcd);
	}

	if (myPlayerControllerData.interactAction)
	{
		myPlayerControllerData.interactAction = false;
		int flowerID = GetClosetFlowerID(); // Return -1 if it did not find any flower close enough
		if (flowerID != -1)
		{
			PlayerCommandData pcd;
			pcd.playerCommand = ePlayerCommands::DestroyFlower;
			pcd.ID = flowerID;
			myPlayer->StoreCommand(pcd);
		}
	}

}

void PlayerController::UpdateControllerData()
{
	myPlayerControllerData.inputDirection = { 0,0 };

	if (isWindowActive == 0)
		return;

	if (GetAsyncKeyState(myInputMapper.up))
		myPlayerControllerData.inputDirection += { 0, 1 };
	if (GetAsyncKeyState(myInputMapper.down))
		myPlayerControllerData.inputDirection += { 0, -1 };
	if (GetAsyncKeyState(myInputMapper.left))
		myPlayerControllerData.inputDirection += { -1, 0 };
	if (GetAsyncKeyState(myInputMapper.right))
		myPlayerControllerData.inputDirection += {  1, 0 };

	if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_K, false))
	{
		myPlayerControllerData.useAction = true;
	}
	if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_L, false))
	{
		myPlayerControllerData.interactAction = true;
	}

	myPlayerControllerData.inputDirection.Normalize();

	myPlayerControllerData.hasMoved = false;

	if (!(myPlayerControllerData.inputDirection.x == 0 && myPlayerControllerData.inputDirection.y == 0))
	{
		myPlayerControllerData.hasMoved = true;
	}
}

int PlayerController::GetClosetFlowerID()
{
	std::unordered_map<unsigned int, Entity*> flowers = myEntityFactory->GetAllEntitiesOfType(EntityType::flower);

	int closestID = -1;
	float closest = pickUpDistance;
	for (auto& [id, flower] : flowers)
	{
		float distanceBetweenFlowerAndPlayer = Tga::Vector2f::Distance(myPlayer->GetPosition(), flower->GetPosition());
		if (closest > distanceBetweenFlowerAndPlayer)
		{
			closest = distanceBetweenFlowerAndPlayer;
			closestID = id;
		}
	}
	return closestID;
}
