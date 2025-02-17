#pragma once
#include <Windows.h>

class InputManager
{
public:
	// Touche apuyee en continu
	static bool GetKeyIsPressed(char key);

	// Touche appuyee une fois
	static bool GetKeyDown(char key);

	// Ajout de la gestion de la souris :
	static void UpdateMouse(HWND hWnd); // Met � jour le delta de la souris (� appeler chaque frame)
	static int GetMouseDeltaX();
	static int GetMouseDeltaY();

private:
	static inline bool wasKeyPressed = false;

	static inline POINT s_LastMousePos = { 0, 0 };
	static inline POINT s_MouseDelta = { 0, 0 };
};