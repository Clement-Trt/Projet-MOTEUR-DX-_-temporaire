#include "pch.h"
#include "Input.h"

// ---------------------------------------------- IL FAUT CONFIG NOTRE PCH.H AVANT

bool Input::GetKeyIsPressed(char key)
{
	//if (GetAsyncKeyState(key) < 0)
	//	return true;
	return false;
}

bool Input::GetKeyDown(char key) {
	//bool isKeyPressed = (GetAsyncKeyState(key) & 0x8000) != 0;
	//if (isKeyPressed && !wasKeyPressed) {
	//	wasKeyPressed = true;
	//	return true;
	//}
	//if (!isKeyPressed) {
	//	wasKeyPressed = false;
	//}

	return false;
}