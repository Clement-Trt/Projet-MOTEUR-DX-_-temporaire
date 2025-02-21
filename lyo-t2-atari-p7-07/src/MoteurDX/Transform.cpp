#include "pch.h"

#include "Transform.h"

Transform::Transform() 
{
    Identity();
}

void Transform::UpdateMatrix() 
{
    DirectX::XMMATRIX matriceScale = DirectX::XMMatrixScaling(vScale.x, vScale.y, vScale.z);
    DirectX::XMMATRIX matriceRotation = DirectX::XMMatrixRotationQuaternion(XMLoadFloat4(&qRotation));
    DirectX::XMMATRIX matricePosition = DirectX::XMMatrixTranslation(vPosition.x, vPosition.y, vPosition.z);

    XMStoreFloat4x4(&mRotation, matriceRotation);

    // ordre important ! (Scale * Rotation * Position)
    DirectX::XMMATRIX newMatrice;

    newMatrice = matriceScale; // ne pas *= directement il faut initialisEune valeur de base !
    newMatrice *= matriceRotation;
    newMatrice *= matricePosition;

    DirectX::XMStoreFloat4x4(&matrix, newMatrice);
}

void Transform::Identity()
{
    vPosition = { 0,0,0 };
    vScale = { 1,1,1 };

    qRotation = { 0,0,0,1 };
    mRotation =
    {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };

    vFront = { 0,0,1 };
    vRight = { 1,0,0 };
    vUp = { 0,1,0 };

    matrix =
    {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };
}

void Transform::Rotation(float roll, float pitch, float yaw)
{
    // quaternion contenant les 3 rotations
    DirectX::XMVECTOR qRotYPR = DirectX::XMQuaternionIdentity();

    // quaternion contenant une seule rotation
    DirectX::XMVECTOR qRotTmp;

    // Creer un quaternion pour chaque rotation (delta)
    qRotTmp = DirectX::XMQuaternionRotationAxis(XMLoadFloat3(&vFront), roll);
    qRotYPR = DirectX::XMQuaternionMultiply(qRotYPR, qRotTmp);

    qRotTmp = DirectX::XMQuaternionRotationAxis(XMLoadFloat3(&vRight), pitch);
    qRotYPR = DirectX::XMQuaternionMultiply(qRotYPR, qRotTmp);

    qRotTmp = DirectX::XMQuaternionRotationAxis(XMLoadFloat3(&vUp), yaw);
    qRotYPR = DirectX::XMQuaternionMultiply(qRotYPR, qRotTmp);

    // Ajouter la rotation delta a la rotation actuelle de lobjet
    DirectX::XMVECTOR newQRotation = DirectX::XMQuaternionMultiply(XMLoadFloat4(&qRotation), qRotYPR);
    newQRotation = DirectX::XMQuaternionNormalize(newQRotation);

    DirectX::XMStoreFloat4(&qRotation, newQRotation);

    // Convertir le quaternion en une matrice de rotation (magique)
    DirectX::XMMATRIX mMatriceRot = DirectX::XMMatrixRotationQuaternion(newQRotation);
    DirectX::XMStoreFloat4x4(&mRotation, mMatriceRot);

    // Mettre a jour les axes de notre objet (3 vecteurs) _11 _23 -> case de la matrice et table float4x4
    vRight = { mRotation._11, mRotation._12, mRotation._13 };
    vUp = { mRotation._21, mRotation._22, mRotation._23 };
    vFront = { mRotation._31, mRotation._32, mRotation._33 };

    UpdateMatrix();
}

void Transform::Move(float dirFront, float dirRight, float dirUp)
{
    vPosition.z += dirFront * vFront.z;
    vPosition.x += dirFront * vFront.x;
    vPosition.y += dirFront * vFront.y;

    // Deplacement selon le vecteur vRight (droite/gauche)
    vPosition.z += dirRight * vRight.z;
    vPosition.x += dirRight * vRight.x;
    vPosition.y += dirRight * vRight.y;

    // Deplacement selon le vecteur vUp (haut/bas)
    vPosition.z += dirUp * vUp.z;
    vPosition.x += dirUp * vUp.x;
    vPosition.y += dirUp * vUp.y;

    UpdateMatrix();
}

void Transform::Scale(float scaleX, float scaleY, float scaleZ)
{
    vScale = DirectX::XMFLOAT3(scaleX, scaleY, scaleZ);

    UpdateMatrix();
}

//DirectX::XMMATRIX& Transform::GetXMMatrix()
//{
//    DirectX::XMMATRIX m = DirectX::XMLoadFloat4x4(&matrix);
//    return m;
//    // TODO: insérer une instruction return ici
//}

