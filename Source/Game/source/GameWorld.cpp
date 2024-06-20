#include "stdafx.h"

#include "GameWorld.h"
#include <tge/graphics/GraphicsEngine.h>
#include <tge/drawers/SpriteDrawer.h>
#include <tge/texture/TextureManager.h>
#include <tge/drawers/DebugDrawer.h>

#include <tge/text/text.h>

#include "Player.h"
#include "PlayerController.h"


GameWorld::GameWorld()
{}

GameWorld::~GameWorld() 
{}

void GameWorld::Init()  
{
	auto& engine = *Tga::Engine::GetInstance();

	Tga::Vector2ui resUI = engine.GetRenderSize();

	Tga::Vector2f startPosition = { (float)resUI.x / 2, (float)resUI.y / 2 };
	myPlayer = new Player();
	myPlayer->Init(startPosition);
	myController = new PlayerController(myPlayer,0);


}
void GameWorld::Update(float aTimeDelta)
{
	myController->Update(aTimeDelta);



	// Recieve message from server
	// handle

	// Input 
	// Sends messages to server for verification

}

void GameWorld::Render()
{
	auto &engine = *Tga::Engine::GetInstance();
	Tga::SpriteDrawer& spriteDrawer(engine.GetGraphicsEngine().GetSpriteDrawer());
	// Game update
	{
		myPlayer->Render(spriteDrawer);
	}
	//Tga::SpriteSharedData;
}