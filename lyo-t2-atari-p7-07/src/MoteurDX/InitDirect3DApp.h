#pragma once

#include "WindowDX.h";
#include "Camera.h";
#include "Scene.h";

//class GameManager;
class MeshFactory;
//class Camera;

class InitDirect3DApp : public WindowDX
{
public:
    InitDirect3DApp(HINSTANCE hInstance);
    bool Initialize(Scene* scene);
    void Update() override;
    void UpdatePhysics();
    void Draw() override;

    MeshFactory* GetFactory() { return m_meshFactory; }

    // __ Game loop __ 
    
    //void HandleInput();

    void Render();
    void CreatePipelineState();

    void SetDeltaTime(float deltaTime) { mDeltaTime = deltaTime; }
    void SetScene(Scene* scene) { mScene = scene; }

    EntityManager* GetEntityManager() { return mEM; }

    friend class Scene;

private:
    ComPtr<ID3D12PipelineState> mPSO;
    D3D12_DEPTH_STENCIL_DESC m_depthStencilDesc;
    Camera m_Camera;
    MeshFactory* m_meshFactory;

    EntityManager* mEM;

    Scene* mScene;

    float mDeltaTime; //    /!\/!\/!\/!\    A UTILISER DANS LA BOUCLE DE JEU    /!\/!\/!\/!\/

};