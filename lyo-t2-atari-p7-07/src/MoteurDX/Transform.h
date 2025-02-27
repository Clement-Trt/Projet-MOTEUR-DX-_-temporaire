#pragma once

class Transform
{
public:
    // lance initialisation a la creation
    Transform();

    // Initialisation par defaut
    void Identity();

    void ResetRotation();

    // Mes a jour la matrice
    void UpdateMatrix();

    // Effectue la rotation Z X Y
    void Rotation(float roll, float pitch, float yaw);

    // Effectue le deplacement Z X Y
    void Move(float dirFront, float dirRight, float dirUp);

    // Effectue le scaling
    void Scale(float scaleX, float scaleY, float scaleZ);

    // Recup la matrice
    DirectX::XMFLOAT4X4& GetMatrix() { return matrix; }

    DirectX::XMFLOAT3& GetScale() { return vScale; }

    //DirectX::XMMATRIX& GetXMMatrix();

    DirectX::XMFLOAT3& GetPositionF3() { return vPosition; }
    float GetPositionX() { return vPosition.x; }
    float GetPositionY() { return vPosition.y; }
    float GetPositionZ() { return vPosition.z; }

    void AddToGlobalPosX(float deltaX);
    void AddToGlobalPosY(float deltaY);
    void AddToGlobalPosZ(float deltaZ);

private:

    // m : matrice, v : vector, q : quaternion
    DirectX::XMFLOAT3 vPosition; // vector pos
    DirectX::XMFLOAT3 vScale; // vector scale

    DirectX::XMFLOAT4 qRotation; // quaternion rotation
    DirectX::XMFLOAT4X4 mRotation; // matrice rotation

    DirectX::XMFLOAT3 vFront;
    DirectX::XMFLOAT3 vRight;
    DirectX::XMFLOAT3 vUp;

    DirectX::XMFLOAT4X4 matrix; // matrice contenant les coordonees et rotation

    DirectX::XMFLOAT3 m_oldPosition;
};

