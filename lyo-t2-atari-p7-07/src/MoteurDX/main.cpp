#include "pch.h"
#include "main.h"

//#include <iostream>
//#include <DirectXColors.h>
//#include "WindowDX.h"
//#include "Camera.h"
//#include "InputManager.h"
//#include "MeshFactory.h"
//
//#include <iostream>
//#include "GameManager.h"
//#include "Scene.h"
//#include "SandBoxScene.h"

#include "InitDirect3DApp.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
    InitDirect3DApp theApp(hInstance);
    if (!theApp.Initialize())
        return 0;
    return theApp.Run();
}