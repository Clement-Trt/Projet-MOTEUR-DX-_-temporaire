#pragma once

#include "WindowDX.h";
//#include "Camera.h";
#include "Scene.h";

//class GameManager;
class MeshFactory;
class Camera;

class TextureManager;

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
    //void SetCamera(Camera* camView) { m_Camera = camView; }

    EntityManager* GetEntityManager() { return m_entityManager; }
    MeshFactory* GetFactory() { return m_meshFactory; }

    CameraComponent* GetMainView() { return m_mainView; }

    friend class Scene;

private:
    D3D12_DEPTH_STENCIL_DESC m_depthStencilDesc;

    CameraComponent* m_mainView;

    MeshFactory* m_meshFactory;
    EntityManager* m_entityManager;
    TextureManager* m_textureManager;

    Scene* mScene;

    float mDeltaTime; //    /!\/!\/!\/!\    A UTILISER DANS LA BOUCLE DE JEU    /!\/!\/!\/!\/

    // Textures:
    ComPtr<ID3D12Resource> m_texture;
};