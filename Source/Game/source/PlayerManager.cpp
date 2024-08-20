#include "stdafx.h"
#include "PlayerManager.h"
#include "Player.h"

void PlayerManager::Init()
{
}


Player* PlayerManager::CreatePlayer(const unsigned int aServerID, const Tga::Vector2f aStartPosition, const bool isLocalPlayer)
{
	myPlayers.insert(std::pair<unsigned int, Player*>(aServerID, new Player()));

	myPlayers.at(aServerID)->Init(aStartPosition, isLocalPlayer);


	return myPlayers.at(aServerID);
}

void PlayerManager::AssignLocalPlayer(Player* aPlayer)
{
	myLocalPlayer = aPlayer;
}

Player* PlayerManager::GetLocalPlayer()
{
	return myLocalPlayer;
}

bool PlayerManager::UpdatePlayer(const unsigned int aServerID, const UpdatePlayerData& someData)
{
	if (myPlayers.count(aServerID) == 0) return false;

	myPlayers.at(aServerID)->myPosition = someData.aPosition;

	return true;
}

bool PlayerManager::DeletePlayer(const unsigned int anID)
{
	if (myPlayers.count(anID) == 0) return false;

	delete myPlayers.at(anID);

	myPlayers.erase(anID);

	return true;
}

void PlayerManager::Render(Tga::SpriteDrawer& aSpriteDrawer)
{
	for (auto& [id, player] : myPlayers)
	{
		player->Render(aSpriteDrawer);
	}
}

void PlayerManager::DebugRender(Tga::DebugDrawer& aSpriteDrawer)
{
	for (auto& [id, player] : myPlayers)
	{
		player->DebugRender(aSpriteDrawer);
	}
}

