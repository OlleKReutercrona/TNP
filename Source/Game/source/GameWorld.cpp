#include "stdafx.h"

#include "GameWorld.h"
#include <tge/graphics/GraphicsEngine.h>
#include <tge/drawers/SpriteDrawer.h>
#include <tge/texture/TextureManager.h>
#include <tge/drawers/DebugDrawer.h>
#include <tge/text/text.h>
#include <tge/input/InputManager.h>

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
}

void GameWorld::Init(Tga::InputManager& aInputManager)
{
	myInputManager = &aInputManager;


	myEntityFactory.Init();
	myPlayerManager.Init();

	myClient.Init(myPlayerManager, myEntityFactory, myStatManager);

	if (C_FAIL(myClient.Start()))
	{
		std::cout << "START FAILED" << std::endl;
	}
	std::cout << "Client has started" << std::endl;
	std::cout << "Client trying to connect..." << std::endl;


	// Text Init
	{
		myDisplayText = new Tga::Text(L"Text/arial.ttf", Tga::FontSize_36);
		myInputText = new Tga::Text(L"Text/arial.ttf", Tga::FontSize_24);

		Tga::Engine& engine = *Tga::Engine::GetInstance();
		Tga::Vector2ui uiRes = engine.GetRenderSize();
		myResolution = { (float)uiRes.x, (float)uiRes.y };


		myDisplayText->SetText("Please enter a username:");
		myDisplayText->SetPosition({ myResolution.x * 0.5f - myDisplayText->GetWidth() / 2.0f, myResolution.y * 0.7f - myDisplayText->GetHeight() / 2.0f});
		
		myInputText->SetText("");
		myInputText->SetPosition({ myResolution.x * 0.5f, myResolution.y * 0.5f });
	}
}

void GameWorld::Update(float aTimeDelta)
{
	if (ConnectClient(aTimeDelta) == false)
	{
		return;
	}
	// SKICKA MEDDELANDEN
	myController->Update(aTimeDelta);
	if (myPlayerManager.GetLocalPlayer()->HasCommands())
	{
		myClient.StorePlayerCommands(myPlayerManager.GetLocalPlayer()->GetCommands());
		myPlayerManager.GetLocalPlayer()->ClearCommands();
	}
	

	// Recieve message from server THREAD
	myClient.Run(aTimeDelta);

	// Input 
	// Sends messages to server for verification

	myStatManager.Update(aTimeDelta);
}

bool GameWorld::ConnectClient(const float aTimeDelta)
{
	if (myClient.HasJoined()) { return true;  }


	static bool connecting = false;

	if (connecting == false)
	{
		std::string input = myInputManager->GetPressedKeys();

		if (myInputManager->IsKeyPressed(VK_BACK) && myUserName.size() > 0)
		{
			myUserName.erase(myUserName.end() - 1);
		}
		myUserName += input;
		std::cout << input;

		if (myInputManager->IsKeyPressed(VK_RETURN))
		{
			connecting = true;
		}

		myInputText->SetText(myUserName);
		myInputText->SetPosition({ myResolution.x * 0.5f - myInputText->GetWidth() / 2.0f, myResolution.y * 0.5f - myInputText->GetHeight() / 2.0f });
	}

	if (connecting)
	{
		myClient.RecieveMessageFromServer();


		constexpr float waitTime = 0.5f;
		static float myTime = waitTime;

		if (myTime < waitTime)
		{
			myTime += aTimeDelta;
			return false;
		}

		myTime = 0.0f;

		int result = myClient.Connect(myUserName);
		switch (result)
		{
		case C_FAILED: {
			std::cout << "CONNECT FAILED" << std::endl;
			isRunning = false;
			PostQuitMessage(0);
			return false;
		}
		case C_CONNECTION_FAILED: {
			std::cout << "CONNECT FAILED" << std::endl;
			isRunning = false;
			PostQuitMessage(0);
			return false;
		}
		case C_SUCCESS: {
			std::cout << "Connected to server!\n\n";


			myController = new PlayerController(myPlayerManager.GetLocalPlayer(), myEntityFactory);
			return true;
		}
		case C_QUIT: {
			std::cout << "Quiting...\n";
			return false;
		}
		case C_UNAME_TOO_LONG: {
			std::cout << "Username too long\n";
			connecting = false;
			myTime = waitTime;
			return false;
		}
		case C_CONNECTING: {
			std::cout << "Connecting...\n";
			return false;
		}
		}
	}

	return false;
}

void GameWorld::Render()
{
	auto& engine = *Tga::Engine::GetInstance();
	Tga::SpriteDrawer& spriteDrawer(engine.GetGraphicsEngine().GetSpriteDrawer());
	// Game update
	{
		myEntityFactory.Render(spriteDrawer);
		myPlayerManager.Render(spriteDrawer);

		if (myClient.HasJoined() == false)
		{
			myInputText->Render();
			myDisplayText->Render();
		}
	}
	myStatManager.DisplayDebugStats();

	if (!_RENDERDEBUG)
		return;

	Tga::DebugDrawer& debugDrawer(engine.GetDebugDrawer());
	{
		myPlayerManager.DebugRender(debugDrawer);
	}

}