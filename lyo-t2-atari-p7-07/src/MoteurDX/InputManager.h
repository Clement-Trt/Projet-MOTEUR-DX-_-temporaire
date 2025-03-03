#pragma once

class InputManager
{
public:
	// Touche apuyee en continu
	static bool GetKeyIsPressed(char key);

	// Touche appuyee une fois
	static bool GetKeyDown(char key);

	// Ajout de la gestion de la souris :
	static void UpdateMouse(HWND hWnd); // Met a jour le delta de la souris (a appeler chaque frame)
	static int GetMouseDeltaX();
	static int GetMouseDeltaY();

private:
	//256 pour couvrir toutes les touches
	static inline bool wasKeyPressed[256] = { false };

	static inline POINT s_LastMousePos = { 0, 0 };
	static inline POINT s_MouseDelta = { 0, 0 };
};