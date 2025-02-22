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
    bool Initialize();
    bool InitTexture();
    void Update() override;
    void UpdatePhysics();
    void Draw() override;

    // __ Game loop __ 
    
    //void HandleInput();

    void Render();
    void CreatePipelineState();

    void SetDeltaTime(float deltaTime) { mDeltaTime = deltaTime; }
    void SetScene(Scene* scene) { mScene = scene; }

    EntityManager* GetEntityManager() { return m_entityManager; }
    MeshFactory* GetFactory() { return m_meshFactory; }

    friend class Scene;

private:
    //ComPtr<ID3D12PipelineState> mPSO;
    D3D12_DEPTH_STENCIL_DESC m_depthStencilDesc;
    Camera m_Camera;

    MeshFactory* m_meshFactory;
    EntityManager* m_entityManager;

    Scene* mScene;

    float mDeltaTime; //    /!\/!\/!\/!\    A UTILISER DANS LA BOUCLE DE JEU    /!\/!\/!\/!\/

    // Textures:
    ComPtr<ID3D12Resource> m_Texture;
    ComPtr<ID3D12Resource> m_TextureUploadHeap;
    ComPtr<ID3D12DescriptorHeap> m_SrvHeap;

};