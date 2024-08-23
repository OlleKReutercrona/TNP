#pragma once

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


	void Render(Tga::SpriteDrawer& aSpriteDrawer);
private:
	const unsigned int myEntityID = 0;
	Tga::Vector2f myPosition = {};

	Tga::SpriteSharedData& mySharedData;
	Tga::Sprite2DInstanceData mySpriteInstance;
};