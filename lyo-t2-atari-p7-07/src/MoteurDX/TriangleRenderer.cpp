﻿#include "pch.h"

#include "TriangleRenderer.h"
#include "InputManager.h"

TriangleRenderer::TriangleRenderer()
{

}

bool TriangleRenderer::Initialize(ID3D12Device* device, ID3D12CommandQueue* commandQueue, ID3D12GraphicsCommandList* commandList, IDXGISwapChain3* swapChain, ID3D12DescriptorHeap* rtvHeap, ID3D12DescriptorHeap* dsvHeap, UINT rtvDescriptorSize, float size, DirectX::XMFLOAT4 color, D3D12_DEPTH_STENCIL_DESC depthStencilDesc, Camera* camera)
{
    m_Device = device;
    m_CommandQueue = commandQueue;
    m_CommandList = commandList;
    m_SwapChain = swapChain;
    m_RtvHeap = rtvHeap;
    m_DsvHeap = dsvHeap;
    m_RtvDescriptorSize = rtvDescriptorSize;
    m_Size = size;
    m_Color = color;
    m_DepthStencilDesc = depthStencilDesc;
    m_Camera = camera;
    
    CreatePipelineState();
    CreateVertexBuffer(m_Size, m_Color);
    CreateConstantBuffer();
    return true;
}

void TriangleRenderer::Update()
{
    // Update logic (if needed)
    
    UpdateTransform();

    //test input
    if (InputManager::GetKeyDown('A'))
    {
        //MessageBox(0, L"Touche A pressed !", L"Debug Input", MB_OK);
        m_Transform.Move(0.1f,0.0f,0.0f); // Avance
    }
    if (InputManager::GetKeyDown('Z'))
    {
        m_Transform.Rotation(0.0f, DirectX::XMConvertToRadians(5.0f), 0.0f); // Rotation
    }
    if (InputManager::GetKeyDown('Q'))
    {
        m_Transform.Rotation(0.05f, DirectX::XMConvertToRadians(0.0f), 0.0f);
    }
    if (InputManager::GetKeyDown('S'))
    {
        m_Transform.Rotation(0.0f, DirectX::XMConvertToRadians(-5.0f), 0.0f);
    }
    if (InputManager::GetKeyDown('D'))
    {
        m_Transform.Rotation(-0.05f, DirectX::XMConvertToRadians(0.0f), 0.0f);
    }

}

void TriangleRenderer::UpdateTransform()
{
    m_Transform.UpdateMatrix();

    DirectX::XMMATRIX view = m_Camera->GetViewMatrix();

    DirectX::XMMATRIX proj = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, 1.0f, 1.0f, 1000.0f);

    DirectX::XMMATRIX worldViewProj = XMLoadFloat4x4(&m_Transform.GetMatrix()) * view * proj;
    XMStoreFloat4x4(&m_ObjectConstants.WorldViewProj, DirectX::XMMatrixTranspose(worldViewProj));

    void* pData;
    CD3DX12_RANGE readRange(0, 0);
    m_ConstantBuffer->Map(0, &readRange, &pData);
    memcpy(pData, &m_ObjectConstants, sizeof(m_ObjectConstants));
    m_ConstantBuffer->Unmap(0, nullptr);
}

void TriangleRenderer::Render()
{
    // Configurer le root signature pour le pipeline.
    m_CommandList->SetGraphicsRootSignature(m_RootSignature.Get());

    // Definir l'objet de l'etat du pipeline (PSO) contenant la configuration graphique.
    m_CommandList->SetPipelineState(m_PipelineState.Get());

    // Attacher le constant buffer (contenant la matrice World-View-Projection) au pipeline.
    m_CommandList->SetGraphicsRootConstantBufferView(0, m_ConstantBuffer->GetGPUVirtualAddress());

    // Set primitive topology
    m_CommandList->IASetVertexBuffers(0, 1, &m_VertexBufferView);
    m_CommandList->IASetIndexBuffer(&m_IndexBufferView);
    m_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Draw the triangle
    // emettre l'appel de dessin : 36 indices pour dessiner le cube.
    // m_CommandList->DrawInstanced(36, 1, 0, 0);
    // Nb de vertices | InstanceCount | StartIndexLocation | BaseVertexLocation | StartInstanceLocation
    m_CommandList->DrawIndexedInstanced(36, 1, 0, 0, 0);
}

void TriangleRenderer::CreateVertexBuffer(float size, DirectX::XMFLOAT4 color)
{
    float halfSize = size / 2.0f;

    Vertex m_triangleVertices[] =
    {
        // X Y Z | R G B A
        { DirectX::XMFLOAT3(0.0f, 0.5f, 0.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) }, // Haut (Rouge)
        { DirectX::XMFLOAT3(0.5f, -0.5f, 0.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) }, // Bas droit (Vert)
        { DirectX::XMFLOAT3(-0.5f, -0.5f, 0.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) },  // Bas gauche (Violet)
    };

    Vertex vertices[] =
    {
        
        // Face avant
        { DirectX::XMFLOAT3(-halfSize, -halfSize, halfSize), { 1.0f, 0.0f, 0.0f, 1.0f } }, // Bas gauche  (0)
        { DirectX::XMFLOAT3(halfSize,  -halfSize, halfSize), { 1.0f, 0.0f, 0.0f, 1.0f } }, // Bas droit   (1)
        { DirectX::XMFLOAT3(halfSize,  halfSize, halfSize), { 0.0f, 1.0f, 0.0f, 1.0f } },  // Haut droit  (2)
        { DirectX::XMFLOAT3(-halfSize, halfSize, halfSize), { 1.0f, 0.0f, 0.0f, 1.0f } },  // Haut gauche (3)

        // Face arriere
        { DirectX::XMFLOAT3(-halfSize, -halfSize, -halfSize), { 0.0f, 0.0f, 1.0f, 1.0f } },  // Bas gauche  (4)
        { DirectX::XMFLOAT3(halfSize,  -halfSize, -halfSize), { 0.0f, 0.0f, 1.0f, 1.0f } },  // Bas droit   (5)
        { DirectX::XMFLOAT3(halfSize,  halfSize, -halfSize), { 0.0f, 0.0f, 1.0f, 1.0f } },   // Haut droit  (6)
        { DirectX::XMFLOAT3(-halfSize, halfSize, -halfSize), { 0.0f, 1.0f, 0.0f, 1.0f } }    // Haut gauche (7)
    };

    uint16_t indices[] =
    {
        // Triangle1 | triangle2 (pour chaque carre du cube)
        // Face avant
        0, 1, 2,  0, 2, 3,
        // Face arriere
        4, 6, 5,  4, 7, 6,
        // Face gauche
        4, 5, 1,  4, 1, 0,
        // Face droite
        3, 2, 6,  3, 6, 7,
        // Face haut
        1, 5, 6,  1, 6, 2,
        // Face bas
        4, 0, 3,  4, 3, 7
    };

    // Creation des buffers pour stocker les sommets et indices
    const UINT vSize = sizeof(vertices);
    const UINT iSize = sizeof(indices);

    // Create vertex buffer
    CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
    
    CD3DX12_RESOURCE_DESC vbDesc = CD3DX12_RESOURCE_DESC::Buffer(vSize);
    CD3DX12_RESOURCE_DESC ibDesc = CD3DX12_RESOURCE_DESC::Buffer(iSize);
    m_Device->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &vbDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
        IID_PPV_ARGS(&m_VertexBuffer));

    m_Device->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &ibDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
        IID_PPV_ARGS(&m_IndexBuffer));

    // Copy data to vertex buffer
    void* pVertexData;
    CD3DX12_RANGE readRange(0, 0);
    m_VertexBuffer->Map(0, &readRange, &pVertexData);
    memcpy(pVertexData, vertices, vSize);
    m_VertexBuffer->Unmap(0, nullptr);

    void* pIndexData;
    m_IndexBuffer->Map(0, &readRange, &pIndexData);
    memcpy(pIndexData, indices, iSize);
    m_IndexBuffer->Unmap(0, nullptr);

    // Initialize vertex buffer view
    m_VertexBufferView.BufferLocation = m_VertexBuffer->GetGPUVirtualAddress();
    m_VertexBufferView.StrideInBytes = sizeof(Vertex);
    m_VertexBufferView.SizeInBytes = vSize;

    m_IndexBufferView.BufferLocation = m_IndexBuffer->GetGPUVirtualAddress();
    m_IndexBufferView.Format = DXGI_FORMAT_R16_UINT;
    m_IndexBufferView.SizeInBytes = iSize;
}

void TriangleRenderer::CreateConstantBuffer()
{
    // Taille alignee du Constant Buffer
    const UINT alignedSize = (sizeof(ObjectConstants) + 255) & ~255;

    // Creation du buffer
    CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
    CD3DX12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(alignedSize);

    HRESULT hr = m_Device->CreateCommittedResource(
        &heapProps,
        D3D12_HEAP_FLAG_NONE,
        &bufferDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&m_ConstantBuffer)
    );

    if (FAILED(hr))
    {
        MessageBox(0, L"CreateConstantBuffer failed!", L"Buffer Error", MB_OK);
        return;
    }

    // Initialisation du buffer avec des donnees
    void* mappedData;
    CD3DX12_RANGE readRange(0, 0);
    m_ConstantBuffer->Map(0, &readRange, &mappedData);
    memcpy(mappedData, &m_ObjectConstants, sizeof(m_ObjectConstants));
    m_ConstantBuffer->Unmap(0, nullptr);

    // Creation du CBV
    m_CbvDesc.BufferLocation = m_ConstantBuffer->GetGPUVirtualAddress();
    m_CbvDesc.SizeInBytes = alignedSize;
}

void TriangleRenderer::CreatePipelineState()
{
    //// Create root signature
    //CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
    //rootSignatureDesc.Init(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    //ComPtr<ID3DBlob> signature = nullptr;
    //ComPtr<ID3DBlob> error = nullptr;
    //D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
    //HRESULT h = m_Device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_RootSignature));

// Definition d'un parametre root pour un Constant Buffer (CBV)
    CD3DX12_ROOT_PARAMETER slotRootParameter[1];
    slotRootParameter[0].InitAsConstantBufferView(0);

    // Creation de la Root Signature
    CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(1, slotRootParameter, 0, nullptr,
        D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    ComPtr<ID3DBlob> serializedRootSig = nullptr;
    ComPtr<ID3DBlob> error = nullptr;
    HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1,
        &serializedRootSig, &error);
    if (FAILED(hr))
    {
        if (error) MessageBoxA(0, (char*)error->GetBufferPointer(), "RootSignature Error", MB_OK);
        return;
    }

    hr = m_Device->CreateRootSignature(0, serializedRootSig->GetBufferPointer(),
        serializedRootSig->GetBufferSize(), IID_PPV_ARGS(&m_RootSignature));
    if (FAILED(hr))
    {
        MessageBox(0, L"CreateRootSignature failed!", L"Error", MB_OK);
        return;
    }

    // Load and compile shaders
    ComPtr<ID3DBlob> vertexShader;
    ComPtr<ID3DBlob> pixelShader;

    UINT compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
    compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    hr = D3DCompileFromFile(L"../../../src/MoteurDX/VertexShader.hlsl", nullptr, nullptr, "VSMain", "vs_5_0", compileFlags, 0, &vertexShader, &error);
    if (FAILED(hr))
    {
        wchar_t errorMsg[256];
        swprintf_s(errorMsg, L"D3DCompileFromFileVertex failed with HRESULT 0x%08X.", hr);
        MessageBox(0, errorMsg, L"D3DCompileFromFile Error", MB_OK);
    }

    hr = D3DCompileFromFile(L"../../../src/MoteurDX/PixelShader.hlsl", nullptr, nullptr, "PSMain", "ps_5_0", compileFlags, 0, &pixelShader, &error);
    if (FAILED(hr))
    {
        wchar_t errorMsg[256];
        swprintf_s(errorMsg, L"D3DCompileFromFilePixel failed with HRESULT 0x%08X.", hr);
        MessageBox(0, errorMsg, L"D3DCompileFromFile Error", MB_OK);
    }

    // Define the input layout
    D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
    };

    // Create the pipeline state object
    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
    psoDesc.InputLayout = { inputLayout, _countof(inputLayout) };
    psoDesc.pRootSignature = m_RootSignature.Get();
    psoDesc.VS = { vertexShader->GetBufferPointer(), vertexShader->GetBufferSize() };
    psoDesc.PS = { pixelShader->GetBufferPointer(), pixelShader->GetBufferSize() };
    psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    // psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
    psoDesc.DepthStencilState.DepthEnable = TRUE;
    psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
    psoDesc.SampleMask = UINT_MAX;
    psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    psoDesc.NumRenderTargets = 1;
    psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    psoDesc.SampleDesc.Count = 1;
    psoDesc.SampleDesc.Quality = 0;
    psoDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;  // Ajout du format Depth/Stencil
    psoDesc.DepthStencilState = m_DepthStencilDesc; // Utilisation de la valeur stockee

    hr = m_Device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_PipelineState));
    if (FAILED(hr))
    {
        MessageBox(0, L"CreateGraphicsPipelineState failed.", L"Error", MB_OK);
        return;
    }
}

ID3D12PipelineState* TriangleRenderer::GetPipelineState() const
{
    return m_PipelineState.Get();
}

ID3D12RootSignature* TriangleRenderer::GetRootSignature() const
{
    return m_RootSignature.Get();
}