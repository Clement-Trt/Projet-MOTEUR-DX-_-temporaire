#pragma once
#include <Windows.h>

class InputManager
{
public:
	// Touche apuyee en continu
	static bool GetKeyIsPressed(char key);

	// Touche appuyee une fois
	static bool GetKeyDown(char key);

private:
	static inline bool wasKeyPressed = false;

};

