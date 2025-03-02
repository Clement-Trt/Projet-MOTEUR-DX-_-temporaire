#pragma once
#include "Components.h"


class InitDirect3DApp;

class CameraSystem
{
public:

    void Initialize(InitDirect3DApp* gameManager);

    void Update();


    DirectX::XMMATRIX DefaultView(/*CameraComponent* cam*/);

    DirectX::XMMATRIX GetViewMatrix() { return m_viewMatrix; }

    //DirectX::XMMATRIX GetViewMatrix(CameraComponent* camView);

    void SetViewMatrix(CameraComponent* camView, Transform* transform);

    void SetViewMatrix(Transform* transform);

    void SetViewMatrix(DirectX::XMMATRIX viewMatrix);

    void SetViewMatrix(CameraComponent* camView, DirectX::XMMATRIX viewMatrix);

    void ChangeView() { FPS = !FPS; TPS = !TPS; }

    void SetFPS() { FPS = true; TPS = false; }
    void SetTPS() { TPS = true; FPS = false; }
    void SetTPS_Lock(bool value) { TPS_Lock = value; }
private:

    InitDirect3DApp* m_gameManager;

    DirectX::XMMATRIX m_viewMatrix;

    bool FPS = false; // First Person Shooter
    bool TPS = false; // Third Person Shooter
    bool TPS_Lock = false; // Third Person Shooter
};

