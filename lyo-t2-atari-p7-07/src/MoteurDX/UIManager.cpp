#include "pch.h"
#include "UIManager.h"
#include "WindowDX.h"
#include <d3d12.h>

UIManager::UIManager(/*UINT screenWidth, UINT screenHeight*/)
{
    // Cr�ation de la matrice orthographique en mode Left-Handed
    m_orthoMatrix = XMMatrixOrthographicLH(static_cast<float>(WindowDX::GetWindowWidth()), static_cast<float>(WindowDX::GetWindowHeight()), 0.1f, 100.0f);

    // 2. D�finir les vertices du quad UI
    float width = 200.0f;  // Exemple de largeur
    float height = 100.0f; // Exemple de hauteur
    Vertex2D vertices[] = {
        // Triangle 1
        { { 0.0f,    0.0f, 0.0f }, { 0.0f, 0.0f } },
        { { width,   0.0f, 0.0f }, { 1.0f, 0.0f } },
        { { width, height, 0.0f }, { 1.0f, 1.0f } },
        // Triangle 2
        { { 0.0f,    0.0f, 0.0f }, { 0.0f, 0.0f } },
        { { width, height, 0.0f }, { 1.0f, 1.0f } },
        { { 0.0f,  height, 0.0f }, { 0.0f, 1.0f } }
    };

    // 3. Cr�er le vertex buffer (en utilisant ton m�canisme habituel)
    // Par exemple, allouer un ID3D12Resource pour ce buffer et copier 'vertices' dedans.
    CreateVertexBuffer(device, vertices, sizeof(vertices));

    // 4. Initialiser le pipeline graphique sp�cifique � l'UI (PSO, root signature, etc.)
    InitializeUIPipeline(device);
}

UIManager::~UIManager()
{
    // Lib�re ici les ressources allou�es (buffers, PSO, etc.)
}

void UIManager::UpdateScreenSize(/*UINT screenWidth, UINT screenHeight*/)
{
    // Mise � jour de la matrice en cas de changement de r�solution
    m_orthoMatrix = XMMatrixOrthographicLH(static_cast<float>(WindowDX::GetWindowWidth()), static_cast<float>(WindowDX::GetWindowHeight()), 0.1f, 100.0f);
}
