#pragma once

namespace Tga
{
	class SpriteDrawer;
	class DebugDrawer;
}

class Player;
struct UpdatePlayerData
{
	Tga::Vector2f aPosition;
};

class PlayerManager
{
public:
	PlayerManager() = default;
	~PlayerManager() = default;

	void Init();

	Player* CreatePlayer(const unsigned int aServerID, const Tga::Color& aColour, const Tga::Vector2f aStartPosition = { 0.0f, 0.0f }, const bool isLocalPlayer = false);
	bool DeletePlayer(const unsigned int anID);

	void AssignLocalPlayer(Player* aPlayer);
	Player* GetLocalPlayer();

	void Render(Tga::SpriteDrawer& aSpriteDrawer);
	void DebugRender(Tga::DebugDrawer& aSpriteDrawer);

	bool UpdatePlayer(const unsigned int aServerID, const UpdatePlayerData& someData);
private:
	std::map<unsigned int, Player*> myPlayers;
	Player* myLocalPlayer = nullptr;
};

