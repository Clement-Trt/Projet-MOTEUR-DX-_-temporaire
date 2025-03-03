#include "pch.h"
#include "InputManager.h"

bool InputManager::GetKeyIsPressed(char key) 
{
	return (GetAsyncKeyState(static_cast<unsigned char>(key)) < 0);
}

bool InputManager::GetKeyDown(char key) 
{
	// Utiliser la touche comme index
	unsigned char ucKey = static_cast<unsigned char>(key);
	bool isKeyPressed = (GetAsyncKeyState(ucKey) & 0x8000) != 0;
	if (isKeyPressed && !wasKeyPressed[ucKey]) {
		wasKeyPressed[ucKey] = true;
		return true;
	}
	if (!isKeyPressed) {
		wasKeyPressed[ucKey] = false;
	}
	return false;
}


// Cette fonction doit etre appelee chaque frame (avant de recuperer le delta)
void InputManager::UpdateMouse(HWND hWnd)
{
	if (m_cursorLockedAndInvisible == true)
	{
		// Obtenir les dimensions de la fenetre client
		RECT rect;
		GetClientRect(hWnd, &rect);
		POINT center;
		center.x = (rect.right - rect.left) / 2;
		center.y = (rect.bottom - rect.top) / 2;

		// Convertir le centre en coordonnees ecran
		POINT centerScreen = center;
		ClientToScreen(hWnd, &centerScreen);
		POINT currentPos;
		// Recuperer la position de la souris en coordonnees ecran
		GetCursorPos(&currentPos);

		// Calculer le delta par rapport au centre
		s_MouseDelta.x = currentPos.x - centerScreen.x;
		s_MouseDelta.y = currentPos.y - centerScreen.y;

		// Reinitialiser la souris au centre de l'ecran
		SetCursorPos(centerScreen.x, centerScreen.y);
	}

	else
	{
		POINT currentPos;
		// Recuperer la position de la souris en coordonnees ecran
		GetCursorPos(&currentPos);
		// Convertir en coordonnees client
		ScreenToClient(hWnd, &currentPos);

		// Calculer le delta par rapport a la derniere position
		s_MouseDelta.x = currentPos.x - s_LastMousePos.x;
		s_MouseDelta.y = currentPos.y - s_LastMousePos.y;

		// Mettre a jour la derniere position
		s_LastMousePos = currentPos;
	}
}

int InputManager::GetMouseDeltaX()
{
	return s_MouseDelta.x;
}

int InputManager::GetMouseDeltaY()
{
	return s_MouseDelta.y;
}

void InputManager::SetCursorLockedAndInvisible(bool CursorLockedAndInvisible) 
{ 
	m_cursorLockedAndInvisible = CursorLockedAndInvisible; 
}
