#pragma once
#include <DirectXMath.h>
using namespace DirectX;

struct Vertex2D {
    XMFLOAT3 position; // x, y, z
    XMFLOAT2 texcoord; // u, v
};

class UIElement {
public:
    // Constructeur avec la taille de l'élément (largeur et hauteur)
    UIElement();
    ~UIElement();

    // Initialise l'élément UI (création du vertex buffer, etc.)
    HRESULT Initialize(ID3D12Device* device, float posX, float posY, float width, float height);

    // Définit la position (translation) de l'élément sur l'écran
    void SetPosition(float x, float y);

    // Retourne la matrice de transformation (ici, simplement une translation)
    DirectX::XMMATRIX GetTransformMatrix() const;

    // Rendu de l'élément UI (le pipeline doit déjà être configuré dans UIManager)
    void Render(ID3D12GraphicsCommandList* commandList);

private:
    // Vertex buffer et vue associée
    ID3D12Resource* m_vertexBuffer;
    D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;


    // Taille et position de l'élément UI
    float m_width;
    float m_height;
    float m_x; // Position X sur l'écran
    float m_y; // Position Y sur l'écran

    // Matrice de transformation (ici pour la translation)
    DirectX::XMMATRIX m_transform;
};

class UIManager
{
public:
    // Constructeur qui reçoit la taille de la fenêtre
    UIManager();
    ~UIManager();

    // Initialisation des ressources UI (vertex buffer, shaders, etc.)
    void Initialize(ID3D12Device* device, ID3D12RootSignature* rootSignature);

    void AddUIElement(float posX, float posY, float width, float height);

    // Mettre à jour la taille de l'écran (si nécessaire)
    void UpdateScreenSize(/*UINT screenWidth, UINT screenHeight*/);

    ID3D12Resource* CreateVertexBuffer(ID3D12Device* device, void* vertexData, UINT bufferSize);

    // Rendu de l'UI
    void Render(ID3D12GraphicsCommandList* commandList, ID3D12RootSignature* rootSignature);


private:

    void InitializeUIPipeline(ID3D12Device* device);

    std::vector<UIElement*> m_elements;
    // Matrice orthographique pour le rendu 2D
    XMMATRIX m_orthoMatrix;

    ComPtr<ID3D12PipelineState> m_pipelineState;

    // App
    ComPtr<ID3D12Device> m_device;
    ComPtr<ID3D12RootSignature> m_rootSignature;

    // Ici, tu peux stocker d'autres ressources, par exemple :
    // - Vertex buffer pour le quad de l'UI
    // - Pipeline state object (PSO) spécifique à l'UI
    // - Root signature pour l'UI
    // - Textures ou autres ressources
};

