#pragma once

#include <DirectXMath.h>
#include <iostream>

class Transform
{
public:
    // lance initialisation a la creation
    Transform();

    // Initialisation par defaut
    void Identity();

    // Mes a jour la matrice
    void UpdateMatrix();

    // Effectue la rotation
    void Rotation(float yaw, float pitch, float roll);

    // Effectue le deplacement
    void Move(float yaw, float pitch, float roll);

    // Recup la matrice
    DirectX::XMFLOAT4X4& GetMatrix() { return matrix; };

private:

    // m : matrice, v : vector, q : quaternion
    DirectX::XMFLOAT3 vPosition; // vector pos
    DirectX::XMFLOAT3 vScale; // vector scale

    DirectX::XMFLOAT4 qRotation; // quaternion rotation
    DirectX::XMFLOAT4X4 mRotation; // matrice rotation

    DirectX::XMFLOAT3 vDirection;
    DirectX::XMFLOAT3 vRight;
    DirectX::XMFLOAT3 vUp;

    DirectX::XMFLOAT4X4 matrix; // matrice contenant les coordonees et rotation
};

