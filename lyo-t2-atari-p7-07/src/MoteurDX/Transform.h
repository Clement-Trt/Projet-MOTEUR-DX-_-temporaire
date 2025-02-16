#pragma once

#include <DirectXMath.h>
#include <iostream>

class Transform
{
    // m : matrice, v : vector, q : quaternion
public:
    struct TRANSFORM
    {
        DirectX::XMFLOAT3 vPosition; // vector pos
        DirectX::XMFLOAT3 vScale; // vector scale

        DirectX::XMFLOAT4 qRotation; // quaternion rotation
        DirectX::XMFLOAT4X4 mRotation; // matrice rotation

        DirectX::XMFLOAT3 vDirection;
        DirectX::XMFLOAT3 vRight;
        DirectX::XMFLOAT3 vUp;

        DirectX::XMFLOAT4X4 matrix; // matrice contenant les coordonees et rotation

        // lance initialisation a la creation
        TRANSFORM() {
            Identity();
        }

        // Initialisation par defaut
        void Identity()
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

            vDirection = { 0,0,1 };
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

        // Mes a jour la matrice
        void UpdateMatrix()
        {
            DirectX::XMMATRIX matriceScale = DirectX::XMMatrixScaling(vScale.x, vScale.y, vScale.z);
            DirectX::XMMATRIX matriceRotation = DirectX::XMMatrixRotationQuaternion(XMLoadFloat4(&qRotation));
            DirectX::XMMATRIX matricePosition = DirectX::XMMatrixTranslation(vPosition.x, vPosition.y, vPosition.z);

            XMStoreFloat4x4(&mRotation, matriceRotation);

            // ordre important ! (Scale * Rotation * Position)
            DirectX::XMMATRIX newMatrice;

            newMatrice = matriceScale; // ne pas *= directement il faut initialisÅEune valeur de base !
            newMatrice *= matriceRotation;
            newMatrice *= matricePosition;

            DirectX::XMStoreFloat4x4(&matrix, newMatrice);
        }

        // Effectue la rotation
        void Rotation(float yaw, float pitch, float roll)
        {
            // quaternion contenant les 3 rotations
            DirectX::XMVECTOR qRotYPR = DirectX::XMQuaternionIdentity();

            // quaternion contenant une seule rotation
            DirectX::XMVECTOR qRotTmp;

            // Creer un quaternion pour chaque rotation (delta)
            qRotTmp = DirectX::XMQuaternionRotationAxis(XMLoadFloat3(&vDirection), roll);
            qRotYPR = DirectX::XMQuaternionMultiply(qRotYPR, qRotTmp);

            qRotTmp = DirectX::XMQuaternionRotationAxis(XMLoadFloat3(&vRight), pitch);
            qRotYPR = DirectX::XMQuaternionMultiply(qRotYPR, qRotTmp);

            qRotTmp = DirectX::XMQuaternionRotationAxis(XMLoadFloat3(&vUp), yaw);
            qRotYPR = DirectX::XMQuaternionMultiply(qRotYPR, qRotTmp);

            // Ajouter la rotation delta a la rotation actuelle de lobjet
            DirectX::XMVECTOR newQRotation = DirectX::XMQuaternionMultiply(XMLoadFloat4(&qRotation), qRotYPR);
            DirectX::XMStoreFloat4(&qRotation, newQRotation);

            // Convertir le quaternion en une matrice de rotation (magique)
            DirectX::XMMATRIX mMatriceRot = DirectX::XMMatrixRotationQuaternion(newQRotation);
            DirectX::XMStoreFloat4x4(&mRotation, mMatriceRot);

            // Mettre a jour les axes de notre objet (3 vecteurs) _11 _23 -> case de la matrice et table float4x4
            vRight = { mRotation._11, mRotation._12, mRotation._13 };
            vUp = { mRotation._21, mRotation._22, mRotation._23 };
            vDirection = { mRotation._31, mRotation._32, mRotation._33 };

            UpdateMatrix();

            // deja dans update
           // Toujours initialiser les matrices, les quaternions, les vecteursÅE
           // DirectX::XMMATRIX matScale = DirectX::XMMatrixScaling(vScale.x, vScale.y, vScale.z);
           // DirectX::XMMATRIX matTranslation = DirectX::XMMatrixTranslation(vPosition.x, vPosition.y, vPosition.z);
           // DirectX::XMMATRIX matRotation = DirectX::XMMatrixRotationQuaternion(XMLoadFloat4(&qRotation));
           //
           // // Combiner les matrices dans cet ordre : Scale * Rotation * Translation
           // DirectX::XMMATRIX matFinal;
           // matFinal = matScale;
           // matFinal *= matRotation;
           // matFinal *= matTranslation;
           //
           // // Stocker la matrice finale
           // XMStoreFloat4x4(&matrix, matFinal);
        }
    };

};

