#include "pch.h"
#include "main.h"

#include "SceneTest.h"

#include "InitDirect3DApp.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
    SceneTest* scene = new SceneTest;
    InitDirect3DApp theApp(hInstance);
    if (!theApp.Initialize(scene))
        return 0;
    return theApp.Run();
}