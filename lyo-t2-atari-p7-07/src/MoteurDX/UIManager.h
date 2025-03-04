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
    // Constructeur qui re�oit la taille de la fen�tre
    UIManager();
    ~UIManager();

    // Initialisation des ressources UI (vertex buffer, shaders, etc.)
    void Initialize(ID3D12Device* device);

    void AddUIElement(UIElement* element);

    // Mettre � jour la taille de l'�cran (si n�cessaire)
    void UpdateScreenSize(/*UINT screenWidth, UINT screenHeight*/);

    ID3D12Resource* CreateVertexBuffer(ID3D12Device* device, void* vertexData, UINT bufferSize);



private:

    // Rendu de l'UI
    void Render(ID3D12GraphicsCommandList* commandList);

    void InitializeUIPipeline(ID3D12Device* device);

    std::vector<UIElement*> m_elements;
    // Matrice orthographique pour le rendu 2D
    XMMATRIX m_orthoMatrix;

    ComPtr<ID3D12PipelineState> m_pipelineState;
    // Ici, tu peux stocker d'autres ressources, par exemple :
    // - Vertex buffer pour le quad de l'UI
    // - Pipeline state object (PSO) sp�cifique � l'UI
    // - Root signature pour l'UI
    // - Textures ou autres ressources
};


class UIElement {
public:
    // Constructeur avec la taille de l'�l�ment (largeur et hauteur)
    UIElement(float width, float height);
    ~UIElement();

    // Initialise l'�l�ment UI (cr�ation du vertex buffer, etc.)
    HRESULT Initialize(ID3D12Device* device);

    // D�finit la position (translation) de l'�l�ment sur l'�cran
    void SetPosition(float x, float y);

    // Retourne la matrice de transformation (ici, simplement une translation)
    DirectX::XMMATRIX GetTransformMatrix() const;

    // Rendu de l'�l�ment UI (le pipeline doit d�j� �tre configur� dans UIManager)
    void Render(ID3D12GraphicsCommandList* commandList);

private:
    // Vertex buffer et vue associ�e
    ID3D12Resource* m_vertexBuffer;
    D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;


    // Taille et position de l'�l�ment UI
    float m_width;
    float m_height;
    float m_x; // Position X sur l'�cran
    float m_y; // Position Y sur l'�cran

    // Matrice de transformation (ici pour la translation)
    DirectX::XMMATRIX m_transform;
};