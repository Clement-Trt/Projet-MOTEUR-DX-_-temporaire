#pragma once
#include <DirectXMath.h>
using namespace DirectX;

struct Vertex2D {
    XMFLOAT3 position; // x, y, z
    XMFLOAT2 texcoord; // u, v
};

class UIManager
{
public:
    // Constructeur qui reçoit la taille de la fenêtre
    UIManager(/*UINT screenWidth, UINT screenHeight*/);
    ~UIManager();

    // Initialisation des ressources UI (vertex buffer, shaders, etc.)
    void Initialize(ID3D12Device* device);

    // Mettre à jour la taille de l'écran (si nécessaire)
    void UpdateScreenSize(/*UINT screenWidth, UINT screenHeight*/);

    // Rendu de l'UI
    void Render(ID3D12GraphicsCommandList* commandList);

private:
    // Matrice orthographique pour le rendu 2D
    XMMATRIX m_orthoMatrix;

    // Ici, tu peux stocker d'autres ressources, par exemple :
    // - Vertex buffer pour le quad de l'UI
    // - Pipeline state object (PSO) spécifique à l'UI
    // - Root signature pour l'UI
    // - Textures ou autres ressources
};
