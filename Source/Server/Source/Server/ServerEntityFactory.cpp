#include "ServerEntityFactory.h"

server::ServerEntity* server::ServerEntityFactory::CreateEntity(const EntityType aType)
{
    static unsigned int uId = 0;

    myEntities[aType].insert(std::pair<unsigned int, ServerEntity>(uId, ServerEntity(uId, aType)));

    return &myEntities.at(aType).at(uId++);
}

bool server::ServerEntityFactory::DestroyEntity(const EntityType aType, const unsigned int anId)
{
    if (myEntities.at(aType).count(anId) > 0)
    {
        myEntities.at(aType).erase(anId);
        return true;
    }

    return false;
}

server::ServerEntity* server::ServerEntityFactory::GetEntity(const EntityType aType, const unsigned int anId)
{
    if (myEntities.at(aType).count(anId) > 0)
    {
        return &myEntities.at(aType).at(anId);
    }

    return nullptr;
}

std::map<server::EntityType, std::map<unsigned int, server::ServerEntity>> server::ServerEntityFactory::GetEntities()
{
    return myEntities;
}
