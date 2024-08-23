#include "stdafx.h"
#include "EntityFactory.h"
#include "Entity.h"

#include <tge/texture/TextureManager.h>
#include <tge/engine.h>
#include <tge/graphics/GraphicsEngine.h>
#include <tge/drawers/SpriteDrawer.h>


void EntityFactory::Init()
{
	Tga::Engine& engine = *Tga::Engine::GetInstance();

	mySprites[EntityType::flower].myTexture = engine.GetTextureManager().GetTexture(L"Sprites/Flower.dds");
}

Entity* EntityFactory::CreateEntity(const EntityType aType, const Tga::Vector2f aPosition)
{
	auto& list = myEntities[aType];
	const unsigned int id = (unsigned int)list.size() + 1;

	list.insert(std::pair<unsigned, Entity*>(id, new Entity(id, mySprites[aType], aPosition)));

	list.at(id)->Init();

	return list.at(id);
}

Entity* EntityFactory::GetEntity(const EntityType aType, const unsigned int anID)
{
	auto& list = myEntities.at(aType);

	return list.at(anID);
}

bool EntityFactory::DeleteEntity(const EntityType aType, const unsigned int anID)
{
	auto& list = myEntities.at(aType);

	if (list.count(anID) > 0)
	{
		delete list.at(anID);
		list.erase(anID);

		return true;
	}
	
	return false;
}

void EntityFactory::Render(Tga::SpriteDrawer& aSpriteDrawer)
{
	for (auto& [type, list] : myEntities)
	{
		for (auto& [id, entity] : list)
		{
			entity->Render(aSpriteDrawer);
		}
	}
}
