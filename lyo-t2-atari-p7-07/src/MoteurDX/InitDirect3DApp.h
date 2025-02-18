#pragma once

#include "WindowDX.h";
#include "Camera.h";

class GameManager;
class MeshFactory;
//class Camera;

class InitDirect3DApp : public WindowDX
{
public:
    InitDirect3DApp(HINSTANCE hInstance);
    bool Initialize();
    void Update() override;
    void Draw() override;

    MeshFactory* GetFactory() { return m_meshFactory; }

private:
    ComPtr<ID3D12PipelineState> mPSO;
    Camera m_Camera;
    MeshFactory* m_meshFactory;

    GameManager* m_GM;
};