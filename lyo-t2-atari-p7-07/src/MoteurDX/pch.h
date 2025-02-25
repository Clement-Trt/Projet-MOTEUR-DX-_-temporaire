#pragma once

// Mettre include generaux en premier
#include <iostream>
#include <string>
#include <memory>
#include <windows.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <wrl.h>
#include <string>
#include <d3dcompiler.h>
#include <vector>
#include <DirectXMath.h>

// Debug
#include <cassert> 
#include <timeapi.h>

#include "resource.h"
#include "MeshFactory.h"
#include "EntityManager.h"

//#ifdef _DEBUG
//#include <crtdbg.h>
//#define DEBUG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
//#define new DEBUG_NEW
//#endif
