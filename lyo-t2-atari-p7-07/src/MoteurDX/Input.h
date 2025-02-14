#pragma once
//#include <Windows.h>

class Input
{
public:
	// Touche apuyee en continu
	static bool GetKeyIsPressed(char key);

	// Touche appuyee une fois
	static bool GetKeyDown(char key);


private:
	//static inline bool wasKeyPressed = false;
};

// LE KEY DOIT ETRE REMPLACER PAR 'A','B',ect.. EN MAJUSCULE. POUR LES TOUCHES SPECIALS VERIFIER LA LIB DOC WINDOW GetAsyncKeyState.