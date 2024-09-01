#pragma once
#include <Windows.h>
#include <bitset>

namespace CommonUtilities
{
	class InputHandler
	{
	public: 
		InputHandler();
		~InputHandler();
		void Update();
		bool UpdateEvents(UINT message, WPARAM wParam, LPARAM lParam);

		bool IsKeyDown(const int aKeyCode) const; // Is the key currently pressed down
		bool IsKeyHeld(const int aKeyCode) const; // Is the key currently held down
		bool IsKeyPressed(const int aKeyCode) const; // Is the key pressed compared to last frame
		bool IsKeyJustReleased(const int aKeyCode) const; // Was the key just released
		bool IsCursorLocked() const;

		POINT GetMousePosition() const;
		POINT GetMouseMovement() const;
		short GetMouseDeltaMovement() const;
		void SetMousePosition(POINT aPosition) const;
		void ConfineCursor(RECT& aRect, HWND& aM_hwnd);
		void UnluckCurstor();
	private:
		const static int ourMaxKeys = 256;
		std::bitset<ourMaxKeys> myCurrentKeyDowns;
		std::bitset<ourMaxKeys> myLastKeyDowns;
		std::bitset<ourMaxKeys> myHeldKeys;

		POINT myCurrentMousePosition;
		POINT myLastMousePosition;

		short myCurrentMouseZDelta = 0;
		short myMouseZDeltaMovement = 0;

		bool myIsCursorLocked = false;
	};
}