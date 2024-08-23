#pragma once

namespace Tga
{
	class SpriteDrawer;
	class DebugDrawer;
}

enum class EntityType
{
	error,
	flower,
	count,
};

class Entity;

class EntityFactory
{
public:
	EntityFactory() = default;
	~EntityFactory() = default;

	void Init();

	Entity* CreateEntity(const EntityType aType, const Tga::Vector2f aPosition = {0.0f ,0.0f});
	Entity* GetEntity(const EntityType aType, const unsigned int anID);
	bool DeleteEntity(const EntityType aType, const unsigned int anID);

	void Render(Tga::SpriteDrawer& aSpriteDrawer);
private:

	std::map<EntityType, Tga::SpriteSharedData> mySprites = {};

	std::map<EntityType, std::unordered_map<unsigned int, Entity*>> myEntities = {};
};

