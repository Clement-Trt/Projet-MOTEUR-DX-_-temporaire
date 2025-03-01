#pragma once
#include "Components.h"


class CameraSystem
{
public:
    static DirectX::XMMATRIX DefaultView(/*CameraComponent* cam*/);

   /* void SetPosition(float x, float y, float z);
    void Move(float rightDir, float upDir, float frontDir);
    void MoveRelative(float forwardDelta, float rightDelta, float upDelta);
    void Rotate(float pitch, float yaw);*/

    static DirectX::XMMATRIX GetViewMatrix(CameraComponent* camView);

    static void SetViewMatrix(CameraComponent* camView, Transform* transform);

    static void SetViewMatrix(CameraComponent* camView, CameraComponent* playerCam);

    //static void SetViewMatrix(CameraComponent* camViewFrom, CameraComponent* camViewTo);


    /*void SetViewMatrixT(DirectX::XMFLOAT4X4& transformMatrix) { matrix = transformMatrix; }
    void SetTransform(Transform* transformComponent) { transform = transformComponent; }*/

private:

};

