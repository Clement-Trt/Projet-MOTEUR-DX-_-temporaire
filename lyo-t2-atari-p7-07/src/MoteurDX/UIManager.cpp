#include "pch.h"
#include "UIManager.h"
#include "WindowDX.h"
#include <d3d12.h>

UIManager::UIManager()
{
}

UIManager::~UIManager()
{
	// Libère ici les ressources allouées (buffers, PSO, etc.)
}

void UIManager::Initialize(ID3D12Device* device, ID3D12RootSignature* rootSignature)
{
	m_device = device;
	//m_rootSignature = rootSignature;

	// Création de la matrice orthographique en mode Left-Handed
	m_orthoMatrix = XMMatrixOrthographicLH(static_cast<float>(WindowDX::GetWindowWidth()), static_cast<float>(WindowDX::GetWindowHeight()), 0.0f, 100.0f);

	// 4. Initialiser le pipeline graphique spécifique à l'UI (PSO, root signature, etc.)
	InitializeUIPipeline(m_device.Get());

}

void UIManager::AddUIElement(float posX, float posY, float width, float height)
{
	UIElement* newUI = new UIElement;
	newUI->Initialize(m_device.Get(), posX, posY, width, height);

	m_elements.push_back(newUI);
}

void UIManager::UpdateScreenSize(/*UINT screenWidth, UINT screenHeight*/)
{
	// Mise à jour de la matrice en cas de changement de résolution
	m_orthoMatrix = XMMatrixOrthographicLH(static_cast<float>(WindowDX::GetWindowWidth()), static_cast<float>(WindowDX::GetWindowHeight()), 0.0f, 100.0f);
}


ID3D12Resource* UIElement::CreateVertexBuffer(ID3D12Device* device, void* vertexData, UINT bufferSize)
{
	ID3D12Resource* vertexBuffer = nullptr;

	// 1. Définition des propriétés du heap (mémoire GPU)
	D3D12_HEAP_PROPERTIES heapProps = {};
	heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;  // On utilise un buffer en mode "UPLOAD" (CPU ? GPU)
	heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

	// 2. Description de la ressource (taille, type)
	D3D12_RESOURCE_DESC bufferDesc = {};
	bufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	bufferDesc.Width = bufferSize;  // Taille du buffer en octets
	bufferDesc.Height = 1;
	bufferDesc.DepthOrArraySize = 1;
	bufferDesc.MipLevels = 1;
	bufferDesc.Format = DXGI_FORMAT_UNKNOWN;
	bufferDesc.SampleDesc.Count = 1;
	bufferDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	bufferDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	// 3. Création de la ressource
	HRESULT hr = device->CreateCommittedResource(
		&heapProps,
		D3D12_HEAP_FLAG_NONE,
		&bufferDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,  // On prépare la ressource pour lecture GPU
		nullptr,
		IID_PPV_ARGS(&vertexBuffer)
	);

	if (FAILED(hr))
	{
		return nullptr;  // Gérer l'erreur si nécessaire
	}

	// 4. Copier les données CPU vers le buffer GPU
	void* mappedMemory = nullptr;
	D3D12_RANGE readRange = { 0, 0 };  // On ne lit pas depuis le GPU
	vertexBuffer->Map(0, &readRange, &mappedMemory);
	memcpy(mappedMemory, vertexData, bufferSize);
	vertexBuffer->Unmap(0, nullptr);

	return vertexBuffer;
}

void UIManager::InitializeUIPipeline(ID3D12Device* device)
{
	{
		//// 1. Création de la Root Signature (décrit comment les shaders accèdent aux ressources)
		//D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
		//rootSignatureDesc.NumParameters = 0;  // Pas de paramètres pour l'instant
		//rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

		// Déclaration d'un tableau pour les paramètres de la root signature
		//D3D12_ROOT_PARAMETER rootParameters[1] = {};

		//// Configure le premier paramètre pour un CBV à register b0
		//rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		//rootParameters[0].Descriptor.ShaderRegister = 0;
		//rootParameters[0].Descriptor.RegisterSpace = 0;
		//rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;

		//// Mise à jour de la description de la root signature
		//D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
		//rootSignatureDesc.NumParameters = _countof(rootParameters);
		//rootSignatureDesc.pParameters = rootParameters;
		//rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

		//

		//// Definition d'un parametre root pour un Constant Buffer (CBV)
		//CD3DX12_ROOT_PARAMETER slotRootParameter[1]; // 2 parametres au lieu de 1 pour la texture
		//// Parametre 0 : constant buffer (transformation)
		//slotRootParameter[0].InitAsConstantBufferView(0);
		////// Parametre 1 : table de descripteurs pour la texture (register t0)
		////CD3DX12_DESCRIPTOR_RANGE texTable;
		////texTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
		////slotRootParameter[1].InitAsDescriptorTable(1, &texTable, D3D12_SHADER_VISIBILITY_PIXEL);

		//// Declarez le static sampler pour register(s0) 
		//CD3DX12_STATIC_SAMPLER_DESC samplerDesc(0); // register s0
		//samplerDesc.Filter = D3D12_FILTER_ANISOTROPIC;
		//samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		//samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		//samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;

		//// Creation de la Root Signature
		//CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(2, slotRootParameter, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

		////


		//ID3DBlob* signatureBlob = nullptr;
		//ID3DBlob* errorBlob = nullptr;

		//HRESULT hr = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
		////HRESULT hr = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
		//if (FAILED(hr)) return;

		//m_rootSignature = nullptr;
		//hr = device->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature));
		//if (FAILED(hr)) return;

		//D3D12_ROOT_PARAMETER rootParameters[1];

		//// Définir un Constant Buffer View (CBV) sur le slot b0
		//rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		//rootParameters[0].Descriptor.ShaderRegister = 0;  // Correspond à b0
		//rootParameters[0].Descriptor.RegisterSpace = 0;
		//rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	}

	D3D12_DESCRIPTOR_RANGE descriptorRange = {};
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRV pour la texture
	descriptorRange.NumDescriptors = 1;
	descriptorRange.BaseShaderRegister = 0;  // Correspond à t0
	descriptorRange.RegisterSpace = 0;
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_ROOT_PARAMETER rootParameters[2]; // On passe à 2 paramètres

	// CBV 
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[0].Descriptor.ShaderRegister = 0;  // Correspond à b0
	rootParameters[0].Descriptor.RegisterSpace = 0;
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;

	//SRV pour la texture (binding sur t0)
	//rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV;
	//rootParameters[1].Descriptor.ShaderRegister = 0;  // Correspond à t0
	//rootParameters[1].Descriptor.RegisterSpace = 0;
	//rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[1].DescriptorTable.NumDescriptorRanges = 1;
	rootParameters[1].DescriptorTable.pDescriptorRanges = &descriptorRange;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	D3D12_STATIC_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.MipLODBias = 0;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK;
	samplerDesc.MinLOD = 0.0f;
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
	samplerDesc.ShaderRegister = 0;  // Correspond à s0 dans ton shader
	samplerDesc.RegisterSpace = 0;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // Visible par le Pixel Shader


	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
	rootSignatureDesc.NumParameters = _countof(rootParameters);
	rootSignatureDesc.pParameters = rootParameters;
	rootSignatureDesc.NumStaticSamplers = 1;
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	ID3DBlob* signatureBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	HRESULT hr = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);

	if (FAILED(hr))
	{
		if (errorBlob) {
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
		}
		return;
	}

	hr = device->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature));

	if (FAILED(hr))
	{
		return;
	}


	// Load and compile shaders
	ComPtr<ID3DBlob> vertexShader;
	ComPtr<ID3DBlob> pixelShader;

	UINT compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	hr = D3DCompileFromFile(L"../../../src/MoteurDX/UIVertexShader.hlsl", nullptr, nullptr, "main", "vs_5_0", compileFlags, 0, &vertexShader, &errorBlob);
	if (FAILED(hr))
	{
		wchar_t errorMsg[256];
		swprintf_s(errorMsg, L"D3DCompileFromFileVertex failed with HRESULT 0x%08X.", hr);
		MessageBox(0, errorMsg, L"D3DCompileFromFile Error", MB_OK);
	}

	hr = D3DCompileFromFile(L"../../../src/MoteurDX/UIPixelShader.hlsl", nullptr, nullptr, "main", "ps_5_0", compileFlags, 0, &pixelShader, &errorBlob);
	if (FAILED(hr))
	{
		wchar_t errorMsg[256];
		swprintf_s(errorMsg, L"D3DCompileFromFilePixel failed with HRESULT 0x%08X.", hr);
		MessageBox(0, errorMsg, L"D3DCompileFromFile Error", MB_OK);
	}

	// 2. Définition de l'Input Layout (format des sommets)
	D3D12_INPUT_ELEMENT_DESC inputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(float) * 3, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};


	// 3. Configuration du Blend State (activer la transparence)
	D3D12_BLEND_DESC blendDesc = {};
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	// 4. Définition du Rasterizer State (pas de culling pour l'UI)
	D3D12_RASTERIZER_DESC rasterizerDesc = {};
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
	rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;
	rasterizerDesc.DepthClipEnable = TRUE;

	// 5. Création du PSO
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.pRootSignature = m_rootSignature.Get();
	psoDesc.InputLayout = { inputLayout, _countof(inputLayout) };
	psoDesc.VS = { vertexShader->GetBufferPointer(), vertexShader->GetBufferSize() };
	psoDesc.PS = { pixelShader->GetBufferPointer(), pixelShader->GetBufferSize() };
	psoDesc.RasterizerState = rasterizerDesc;
	psoDesc.BlendState = blendDesc;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	psoDesc.SampleDesc.Count = 1;
	psoDesc.DSVFormat = DXGI_FORMAT_UNKNOWN;

	D3D12_DEPTH_STENCIL_DESC depthStencilDesc = {};
	depthStencilDesc.DepthEnable = FALSE;
	depthStencilDesc.StencilEnable = FALSE;
	psoDesc.DepthStencilState = depthStencilDesc;
	psoDesc.SampleMask = UINT_MAX; // Autorise tous les samples


	hr = device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineState));
	if (FAILED(hr)) return;
}



void UIManager::Render(ID3D12GraphicsCommandList* commandList, ID3D12RootSignature* rootSignature)
{
	// Sélectionner le pipeline et le root signature pour l'UI
	commandList->SetPipelineState(m_pipelineState.Get());

	commandList->SetGraphicsRootSignature(m_rootSignature.Get());

	// Transmettre la matrice orthographique au shader (via un buffer constant ou root constants)
	commandList->SetGraphicsRoot32BitConstants(0, 16, &m_orthoMatrix, 0);

	for (auto& UI : m_elements)
	{
		UI->Render(commandList);
	}

	//// Sélectionner le pipeline et le root signature pour l'UI
	//commandList->SetPipelineState(m_uiPSO);
	//commandList->SetGraphicsRootSignature(m_uiRootSignature);

	//// Transmettre la matrice orthographique au shader (via un buffer constant ou root constants)
	//commandList->SetGraphicsRoot32BitConstants(0, 16, &m_orthoMatrix, 0);

	//// Lier le vertex buffer
	//commandList->IASetVertexBuffers(0, 1, &m_uiVertexBufferView);
	//commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//// Dessiner le quad (6 sommets pour 2 triangles)
	//commandList->DrawInstanced(6, 1, 0, 0);
}


// UP = UI MANAGER
// ---------------------------------------------------------------------------------------------------------------------------------------------------------------
// DOWN = UI ELEMENT


//HRESULT UIElement::Initialize(ID3D12Device* device, float width, float height)
//{
//
//    // 2. Définir les vertices du quad UI
//    float width = 200.0f;  // Exemple de largeur
//    float height = 100.0f; // Exemple de hauteur
//    Vertex2D vertices[] = {
//        // Triangle 1
//        { { 0.0f,    0.0f, 0.0f }, { 0.0f, 0.0f } },
//        { { width,   0.0f, 0.0f }, { 1.0f, 0.0f } },
//        { { width, height, 0.0f }, { 1.0f, 1.0f } },
//        // Triangle 2
//        { { 0.0f,    0.0f, 0.0f }, { 0.0f, 0.0f } },
//        { { width, height, 0.0f }, { 1.0f, 1.0f } },
//        { { 0.0f,  height, 0.0f }, { 0.0f, 1.0f } }
//    };
//
//    // 3. Créer le vertex buffer (en utilisant ton mécanisme habituel)
//    // Par exemple, allouer un ID3D12Resource pour ce buffer et copier 'vertices' dedans.
//    CreateVertexBuffer(device, vertices, sizeof(vertices));
//}


UIElement::UIElement()
	: m_vertexBuffer(nullptr),
	m_width(0.0f),
	m_height(0.0f),
	m_x(0.0f),
	m_y(0.0f),
	m_transform(XMMatrixIdentity())
{
}

UIElement::~UIElement()
{
	if (m_vertexBuffer) {
		m_vertexBuffer->Release();
	}
}

//Mesh* UIElement::CreateUIRectangle()
//{
//	Mesh* newMesh = new Mesh;
//
//	// Nombre d'indice de la forme (36 points pour un cube)
//	newMesh->m_geometryMesh.m_meshIndex = 6;
//
//	// Creer la geometrie du cube
//	CreateRectangleUIModel(newMesh);
//
//	// Creer le constant buffer pour ce cube
//	CreateConstantBuffer(newMesh);
//
//	// InitMap
//	CD3DX12_RANGE readRange(0, 0);
//	newMesh->m_constantBuffer->Map(0, &readRange, &newMesh->m_mappedData);
//
//	return newMesh;
//}

//void UIElement::CreateRectangleUIModel(Mesh* cube, float posX, float posY, float width, float height))
//{
//	m_width = width;
//	m_height = height;
//	// On définit un quad en espace local allant de (0,0) à (width, height)
//	Vertex2D vertices[] = {
//		// Triangle 1
//		{ { 0.0f,     0.0f, 0.0f }, { 0.0f, 0.0f } },
//		{ { m_width,  0.0f, 0.0f }, { 1.0f, 0.0f } },
//		{ { m_width,  m_height, 0.0f }, { 1.0f, 1.0f } },
//		// Triangle 2
//		{ { 0.0f,     0.0f, 0.0f }, { 0.0f, 0.0f } },
//		{ { m_width,  m_height, 0.0f }, { 1.0f, 1.0f } },
//		{ { 0.0f,     m_height, 0.0f }, { 0.0f, 1.0f } }
//	};
//}

void UIElement::CreateConstantBuffer(Mesh* cube)
{
}

HRESULT UIElement::Initialize(ID3D12Device* device, float posX, float posY, float width, float height)
{
	m_width = width;
	m_height = height;
	// On définit un quad en espace local allant de (0,0) à (width, height)
	Vertex2D vertices[] = {
		// Triangle 1
		{ { 0.0f,     0.0f, 0.0f }, { 0.0f, 0.0f } },
		{ { m_width,  0.0f, 0.0f }, { 1.0f, 0.0f } },
		{ { m_width,  m_height, 0.0f }, { 1.0f, 1.0f } },
		// Triangle 2
		{ { 0.0f,     0.0f, 0.0f }, { 0.0f, 0.0f } },
		{ { m_width,  m_height, 0.0f }, { 1.0f, 1.0f } },
		{ { 0.0f,     m_height, 0.0f }, { 0.0f, 1.0f } }

//		// Front face (z = +halfSize)
//{ DirectX::XMFLOAT3(-0.0f,  0.0f,  5.0f),  {0.0f, 0.0f} },
//{ DirectX::XMFLOAT3(width,  0.0f,  5.0f),  {1.0f, 0.0f} },
//{ DirectX::XMFLOAT3(width, width,  5.0f),  {1.0f, 1.0f} },
////{ DirectX::XMFLOAT3(-width, -width,  width),  {0.0f, 1.0f} },
//
//// Back face (z = -width)
//{ DirectX::XMFLOAT3(0.0f,  0.0f, -0.0f), {0.0f, 0.0f} },
//{ DirectX::XMFLOAT3(width, width, -0.0f), {0.0f, 1.0f} },
//{ DirectX::XMFLOAT3(-0.0f, width, -0.0f), {1.0f, 1.0f} },
////{ DirectX::XMFLOAT3(-width,  width, -width), {1.0f, 0.0f} },
	};

	SetPosition(posX, posY);

	// 3. Créer le vertex buffer (en utilisant ton mécanisme habituel)
    // Par exemple, allouer un ID3D12Resource pour ce buffer et copier 'vertices' dedans.
    
	//m_vertexBuffer = CreateVertexBuffer(device, vertices, sizeof(vertices));

	UINT bufferSize = sizeof(vertices);

	// Préparation des propriétés du heap pour un buffer en mode UPLOAD
	D3D12_HEAP_PROPERTIES heapProps = {};
	heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
	heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

	// Description de la ressource (vertex buffer)
	D3D12_RESOURCE_DESC resourceDesc = {};
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Width = bufferSize;
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// Creer le vertex buffer
	//ComPtr<ID3D12Resource>& cubeVertexBuffer = cube->m_geometryMesh.m_vertexBuffer;

	HRESULT hr = device->CreateCommittedResource(
		&heapProps,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_vertexBuffer)
	);

	if (FAILED(hr)) {
		return hr;
	}

	// Copier les données des sommets dans le buffer GPU
	void* mappedData = nullptr;
	D3D12_RANGE readRange = { 0, 0 }; // On ne lit pas depuis le GPU
	hr = m_vertexBuffer->Map(0, &readRange, &mappedData);
	if (FAILED(hr)) {
		return hr;
	}
	memcpy(mappedData, vertices, bufferSize);
	m_vertexBuffer->Unmap(0, nullptr);

	// Créer la vue du vertex buffer
	m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
	m_vertexBufferView.StrideInBytes = sizeof(Vertex2D);
	m_vertexBufferView.SizeInBytes = bufferSize;

	return S_OK;
}

void UIElement::SetPosition(float x, float y)
{
	m_x = x;
	m_y = y;
	// Mise à jour de la matrice de transformation (ici, uniquement une translation)
	m_transform = XMMatrixTranslation(m_x, m_y, 0.0f);
}

DirectX::XMMATRIX UIElement::GetTransformMatrix() const
{
	return m_transform;
}

void UIElement::Render(ID3D12GraphicsCommandList* commandList)
{
	// On suppose que le pipeline UI (PSO, root signature, etc.) est déjà configuré par UIManager.
	// Ici, on se contente de lier le vertex buffer et d'effectuer le dessin.
	//commandList->IASetVertexBuffers(0, 1, &m_vertexBufferView);
	if (m_vertexBuffer)
	{
		commandList->IASetVertexBuffers(0, 1, &m_vertexBufferView);
		//commandList->SetGraphicsRootConstantBufferView(1, m_vertexBuffer->GetGPUVirtualAddress());
		commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		commandList->DrawInstanced(6, 1, 0, 0); // 6 vertices pour render un rectangle 
	}
}