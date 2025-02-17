#include "pch.h"
#include "InputManager.h"

bool InputManager::GetKeyIsPressed(char key)
{
	if (GetAsyncKeyState(key) < 0)
		return true;
	return false;
}

bool InputManager::GetKeyDown(char key) {
	bool isKeyPressed = (GetAsyncKeyState(key) & 0x8000) != 0;
	if (isKeyPressed && !wasKeyPressed) {
		wasKeyPressed = true;
		return true;
	}
	if (!isKeyPressed) {
		wasKeyPressed = false;
	}

	return false;
}

// Cette fonction doit �tre appel�e chaque frame (avant de r�cup�rer le delta)
void InputManager::UpdateMouse(HWND hWnd)
{
	POINT currentPos;
	// R�cup�rer la position de la souris en coordonn�es �cran
	GetCursorPos(&currentPos);
	// Convertir en coordonn�es client
	ScreenToClient(hWnd, &currentPos);

	// Calculer le delta par rapport � la derni�re position
	s_MouseDelta.x = currentPos.x - s_LastMousePos.x;
	s_MouseDelta.y = currentPos.y - s_LastMousePos.y;

	// Mettre � jour la derni�re position
	s_LastMousePos = currentPos;
}

int InputManager::GetMouseDeltaX()
{
	return s_MouseDelta.x;
}

int InputManager::GetMouseDeltaY()
{
	return s_MouseDelta.y;
}