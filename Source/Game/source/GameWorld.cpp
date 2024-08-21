#include "stdafx.h"

#include "GameWorld.h"
#include <tge/graphics/GraphicsEngine.h>
#include <tge/drawers/SpriteDrawer.h>
#include <tge/texture/TextureManager.h>
#include <tge/drawers/DebugDrawer.h>

#include <tge/text/text.h>

#include "Player.h"
#include "PlayerController.h"

#include <chrono>
#include <thread>

GameWorld::GameWorld()
{
}

GameWorld::~GameWorld()
{
	isRunning = false;
	if (C_FAIL(myClient.Shutdown()))
	{
		std::cout << "SHUTDOWN FAILED" << std::endl;
	}
	myRecieveMessageThread.join();
}

void GameWorld::Init()
{
	//auto& engine = *Tga::Engine::GetInstance();

	//Tga::Vector2ui resUI = engine.GetRenderSize();
	
	
	myPlayerManager.Init();
	myClient.Init(myPlayerManager);

	if (C_FAIL(myClient.Start()))
	{
		std::cout << "START FAILED" << std::endl;
	}


	//Message Recieve Thread



	StartRecieveMessageThread();

	if (C_FAIL(myClient.Connect()))
	{
		std::cout << "CONNECT FAILED" << std::endl;
		isRunning = false;
		PostQuitMessage(0);
		return;
	}

	myController = new PlayerController(myPlayerManager.GetLocalPlayer());
}
void GameWorld::Update(float aTimeDelta)
{



	//if (!myClient.GetHasJoined())
	//{
	//	// LOGIN LOGIK
	//}
	//else
	//{
	//	// GAMEPLAY LOGIC
	//}


	// SKICKA MEDDELANDEN
	myController->Update(aTimeDelta);
	//myClient.SendPositionMessage();



	// Recieve message from server THREAD
	// handle
	myClient.RecieveMessageFromServer();

	// Input 
	// Sends messages to server for verification

}

void GameWorld::Render()
{
	auto& engine = *Tga::Engine::GetInstance();
	Tga::SpriteDrawer& spriteDrawer(engine.GetGraphicsEngine().GetSpriteDrawer());
	// Game update
	{
		//myPlayer->Render(spriteDrawer);
		myPlayerManager.Render(spriteDrawer);
	}

	if (!_RENDERDEBUG)
		return;

	Tga::DebugDrawer& debugDrawer(engine.GetDebugDrawer());
	{
		//myPlayer->DebugRender(debugDrawer);
		myPlayerManager.DebugRender(debugDrawer);
	}
}

void GameWorld::StartRecieveMessageThread()
{
	myRecieveMessageThread = std::thread([&]
		{
			constexpr int tickRate = 64;
			constexpr float tickTimeStep = 1.0f / (float)tickRate;

			float timeSinceLastTick = 0.0f;

			// very basic async input setup... we read input on a different thread
			while (isRunning)
			{

				// cant check deltatime here since this while loop often time will be faster than the Engine
				// Therefor we need to check our own time in this loop
				// Todo -> check our own time.
				timeSinceLastTick += Tga::Engine::GetInstance()->GetDeltaTime();

				if (!myClient.GetHasJoined())
					continue;
			
				// Return if 
				if (timeSinceLastTick < tickTimeStep) { continue; }

				timeSinceLastTick = 0.0f;
	
				// Handle messages to send
				{
					// Send Client position
					myClient.SendPositionMessage();
				}


				// Handle recieved messages
				{

				}


				// Does buffer have data?
				//Yes?
					//Recieve Message
				//No?
					//continue;
			}
			std::cout << "Recieve Message Thread Done!" << std::endl;
		});
}
