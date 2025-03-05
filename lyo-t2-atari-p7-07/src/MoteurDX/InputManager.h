#pragma once

class InputManager
{
public:
	// Touche apuyee en continu
	static bool GetKeyIsPressed(char key);

	// Touche appuyee une fois
	static bool GetKeyDown(char key);

	static void UpdateMouse(HWND hWnd);
	static int GetMouseDeltaX();
	static int GetMouseDeltaY();

	static void SetCursorLockedAndInvisible(bool CursorLockedAndInvisible);

private:
	static inline bool ms_wasKeyPressed[256] = { false };

	static inline POINT ms_LastMousePos = { 0, 0 };
	static inline POINT ms_MouseDelta = { 0, 0 };

	static inline bool ms_cursorLockedAndInvisible = false;
};