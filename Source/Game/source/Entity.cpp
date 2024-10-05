#include "stdafx.h"
#include "Entity.h"
#include <tge/drawers/SpriteDrawer.h>
#include <tge/texture/texture.h>

void Entity::Init()
{
	mySpriteInstance.mySize = mySharedData.myTexture->CalculateTextureSize() / 3.0f;
}

void Entity::SetPosition(const Tga::Vector2f aPosition)
{
	myPosition = aPosition;
	mySpriteInstance.myPosition = aPosition;
}

Tga::Vector2f Entity::GetPosition()
{
	return myPosition;
}

void Entity::Render(Tga::SpriteDrawer& aSpriteDrawer)
{
	aSpriteDrawer.Draw(mySharedData, mySpriteInstance);
}
