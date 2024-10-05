#pragma once
#include "tge/math/vector2.h"

namespace server
{
	enum class EntityType
	{
		error,
		player,
		flower,

		count
	};

	class ServerEntity
	{
	public:
		ServerEntity(const unsigned int anId, const EntityType aType) : myId(anId), myType(aType) {};
		~ServerEntity() = default;

	public:		
		const unsigned int myId;
		const EntityType myType;

		Tga::Vector2f myPosition;
	private:
	};
}

