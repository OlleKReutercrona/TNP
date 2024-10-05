#include <Windows.h>
#include <string>
#include <iostream>
#include <filesystem>
#include <thread>

void StartExe(LPCTSTR aExePath, int aInstaceIndex, const int aXPos, const int aYPos);

#define NUM_OF_CLIENTS 4

int main()
{
	const LPCTSTR clientExePath = L"\\TNP - Client_Debug.exe";
	const LPCTSTR serverExePath = L"\\Server\\TNP - Server_Debug.exe";

	std::filesystem::path path = std::filesystem::current_path();

	std::wstring localPathC(path.c_str());
	localPathC += clientExePath;
	
	std::wstring localPathS(path.c_str());
	localPathS += serverExePath;

	const LPCTSTR cExe = localPathC.c_str();
	const LPCTSTR sExe = localPathS.c_str();

	StartExe(sExe, 0, 0, 0);

	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	int xPos = 0;
	int yPos = 0;

	constexpr int xRes = 800;
	constexpr int yRes = 800;

	for (int i = 0; i < NUM_OF_CLIENTS; i++)
	{
		StartExe(cExe, i, xPos, yPos);

		if (i % 2)
		{
			xPos += xRes;
		}
		else
		{
			static bool add = true;
			if (add)
			{
				yPos += yRes;
			}
			else
			{
				yPos -= yRes;
			}

			add = !add;
		}
	}

	return 0;
}

void StartExe(LPCTSTR aExePath, int aInstaceIndex, const int aXPos, const int aYPos)
{

	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb += sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	//constexpr int xRes = 1280;
	//constexpr int yRes = 720;


	//int theXValue = ((aInstaceIndex) % 2);
	//aInstaceIndex += 1;
	//int theYValue = ((aInstaceIndex) / 2);

	//int xPos = theXValue * xRes;
	//int yPos = theYValue * yRes;

	std::cout << "Client " << aInstaceIndex << " start window at x: " << aXPos << " | y: " << aYPos << "\n\n";

	std::wstring command = L"AllocConsole";
	std::wstring commandWindowX = command + L"|WINPOSX:" + std::to_wstring(aXPos);
	std::wstring commandWindowY = commandWindowX + L"|WINPOSY:" + std::to_wstring(aYPos);

	CreateProcess(aExePath,
		&commandWindowY[0],
		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		NULL,
		&si,
		&pi
	);

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}