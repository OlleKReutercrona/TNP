#pragma once
#include <map>
#include "Server/ServerEntity.h"

namespace server
{
	class ServerEntityFactory
	{
	public:
		ServerEntityFactory() = default;
		~ServerEntityFactory() = default;

		void Init();
		ServerEntity* CreateEntity(const EntityType aType);
		bool DestroyEntity(const EntityType aType, const unsigned int anId);

		ServerEntity* GetEntity(const EntityType aType, const unsigned int anId);

		std::map<EntityType, std::map<unsigned int, ServerEntity>> GetEntities();

	private:
		std::map<EntityType, std::map<unsigned int, ServerEntity>> myEntities;
	};
}

