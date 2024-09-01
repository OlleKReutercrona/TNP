#include "stdafx.h"
#include "GameWorld.h"
#include "Global.h"

#include <atlconv.h>
#include <atlstr.h>
#include <processenv.h>
#include <tge/settings/settings.h>
#include <tge/error/ErrorManager.h>
#include <shellapi.h>

//#include "CommonUtilities/include/InputHandler.h"
#include "tge/input/InputManager.h"

void Go(const int xWinPos = CW_USEDEFAULT, const int yWinPos = CW_USEDEFAULT);

//CommonUtilities::InputHandler gInputHandler;
Tga::InputManager* gInputManager = nullptr;

int main(const int /*argc*/, const char* /*argc*/[])
{
	LPWSTR* szArgList = nullptr;
	int argCount = 0;
	szArgList = CommandLineToArgvW(GetCommandLine(), &argCount);
	int myWindowPosX = 0;
	int myWindowPosY = 0;

	for (int i = 0; i < argCount; i++)
	{
		std::string simpleString = CW2A(szArgList[i]);

		std::cout << simpleString << "\n";

		if (simpleString.find("AllocConsole") != std::string::npos)
		{
			AllocConsole();
			FILE* fp;
			freopen_s(&fp, "CONOUT$", "w", stdout);
		}
		if (simpleString.find("WINPOSX") != std::string::npos)
		{
			std::string xStr = simpleString;
			size_t index = xStr.find("WINPOSX");
			xStr.erase(0, index + 8);

			index = xStr.find_last_of('|');
			xStr.erase(index, xStr.size() - 1);

			myWindowPosX = std::stoi(xStr, nullptr, 0);
		}
		if (simpleString.find("WINPOSY") != std::string::npos)
		{
			std::string yStr = simpleString;
			size_t index = yStr.find("WINPOSY");
			yStr.erase(0, index + 8);

			myWindowPosY = std::stoi(yStr, nullptr, 0);
		}
	}

	std::cout << "Starting client...\n";
	Go(myWindowPosX, myWindowPosY);
}



LRESULT WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	lParam;
	wParam;
	hWnd;

	gInputManager->UpdateEvents(message, wParam, lParam);

	switch (message)
	{
		// this message is read when the window is closed
	case WM_DESTROY:
	{
		// close the application entirely
		PostQuitMessage(0);
		return 0;
	}
	case WM_ACTIVATE:
	{
		isWindowActive = wParam;
	}

	}

	return 0;
}


void Go(const int xWinPos, const int yWinPos)
{
	Tga::LoadSettings(TGE_PROJECT_SETTINGS_FILE);

	Tga::EngineConfiguration winconf;

	winconf.myApplicationName = L"TGE - Amazing Game";
	winconf.myWinProcCallback = [](HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) { return WinProc(hWnd, message, wParam, lParam); };
	winconf.myEnableVSync = false;

	winconf.myWindowSize = { 800, 800 };
	winconf.myTargetSize = { 800, 800 };
	winconf.myRenderSize = { 800, 800 };

	winconf.myWindowPosition = { xWinPos, yWinPos };
	std::cout << "Windowpos = x: " << xWinPos << " y: " << yWinPos << "\n";

#ifdef _DEBUG
	winconf.myActivateDebugSystems = Tga::DebugFeature::Fps;

	//winconf.myActivateDebugSystems = Tga::DebugFeature::Fps | Tga::DebugFeature::Mem | Tga::DebugFeature::Filewatcher | Tga::DebugFeature::Cpu | Tga::DebugFeature::Drawcalls | Tga::DebugFeature::OptimizeWarnings;
#else
	winconf.myActivateDebugSystems = Tga::DebugFeature::Filewatcher;
#endif

	if (!Tga::Engine::Start(winconf))
	{
		ERROR_PRINT("Fatal error! Engine could not start!");
		system("pause");
		return;
	}

	{
		gInputManager = new Tga::InputManager(*Tga::Engine::GetInstance()->GetHWND());


		GameWorld gameWorld;
		gameWorld.Init(*gInputManager);

		Tga::Engine& engine = *Tga::Engine::GetInstance();



		while (engine.BeginFrame())
		{
			gInputManager->Update();

			gameWorld.Update(engine.GetDeltaTime());


			gameWorld.Render();

			engine.EndFrame();
		}
	}


	delete gInputManager;
	gInputManager = nullptr;
	Tga::Engine::GetInstance()->Shutdown();
}

