#pragma once
#include "Components.h"


class InitDirect3DApp;

class CameraSystem
{
public:

    void Initialize(InitDirect3DApp* gameManager);

    void Update();


    DirectX::XMMATRIX DefaultView(/*CameraComponent* cam*/);


    //DirectX::XMMATRIX GetViewMatrix(CameraComponent* camView);

    void SetViewMatrix(CameraComponent* camView, Transform* transform);

    void SetViewMatrix(CameraComponent* camView, DirectX::XMMATRIX viewMatrix);
private:

    InitDirect3DApp* m_gameManager;
};

