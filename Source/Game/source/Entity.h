#pragma once

#include <tge\text\text.h>

namespace Tga
{
	class SpriteDrawer;
	class DebugDrawer;
	struct SpriteSharedData;
}

class Entity
{
public:
	Entity(const unsigned int anID, Tga::SpriteSharedData& aSprite, const Tga::Vector2f aPosition) : mySharedData(aSprite), myEntityID(anID){ SetPosition(aPosition); }
	~Entity() = default;

	void Init();
	void SetPosition(const Tga::Vector2f aPosition);
	Tga::Vector2f GetPosition();

	void Render(Tga::SpriteDrawer& aSpriteDrawer);
private:
	const unsigned int myEntityID = 0;
	Tga::Vector2f myPosition = {};

	Tga::Text myText;

	Tga::SpriteSharedData& mySharedData;
	Tga::Sprite2DInstanceData mySpriteInstance;
};