#include "pch.h"
#include "MeshFactory.h"
// #include "TextureLoader.h"
#include "TextureLoaderDuLivre.h"

MeshFactory::MeshFactory()
{
}

void MeshFactory::InitMeshFactory(ID3D12Device* device, ID3D12CommandQueue* commandQueue, ID3D12GraphicsCommandList* commandList, IDXGISwapChain3* swapChain, ID3D12DescriptorHeap* rtvHeap, ID3D12DescriptorHeap* dsvHeap, UINT rtvDescriptorSize, D3D12_DEPTH_STENCIL_DESC depthStencilDesc, Camera* camera)
{
	m_Device = device;
	m_CommandQueue = commandQueue;
	m_CommandList = commandList;
	m_SwapChain = swapChain;
	m_RtvHeap = rtvHeap;
	m_DsvHeap = dsvHeap;
	m_RtvDescriptorSize = rtvDescriptorSize;
	m_DepthStencilDesc = depthStencilDesc;
	m_Camera = camera;

	// Cree le pipeline(root signature & PSO)
	CreatePipelineState();

	// Charger la texture DDS
	//HRESULT hr = S_OK;
	//try
	//{
	//	TextureLoader::LoadDDSTexture(
	//		m_Device.Get(),
	//		m_CommandList.Get(),
	//		L"../../../src/MoteurDX/stone.dds", // chemin de votre texture
	//		m_Texture,
	//		m_TextureUploadHeap);
	//}
	//catch (const std::exception& e)
	//{
	//	MessageBoxA(0, e.what(), "Texture Load Error", MB_OK);
	//	return;
	//}

	// Charger la texture depuis un fichier DDS
	HRESULT hr = DirectX::CreateDDSTextureFromFile12(
		m_Device.Get(),
		m_CommandList.Get(),
		L"../../../src/MoteurDX/bricks.dds",
		m_Texture,
		m_TextureUploadHeap);
	if (FAILED(hr))
	{
		MessageBox(0, L"echec du chargement de la texture !", L"Error", MB_OK);
		return;
	}

	// Creer un descriptor heap pour le SRV (1 descripteur suffira) 
	// Le descriptor heap est une zone mémoire réservée sur la carte graphique qui stocke plusieurs descripteurs. Dans le cas du SRV, il contient les 
	// descriptions de ressources (comme les textures) afin que le GPU puisse y accéder directement lors du rendu. Une fois créé et rempli, le heap permet 
	// de regrouper et de gérer ces descripteurs de façon efficace et de les rendre visibles aux shaders.
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.NumDescriptors = 1;
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	hr = m_Device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&m_SrvHeap));
	if (FAILED(hr))
	{
		MessageBox(0, L"Failed to create SRV Heap!", L"Error", MB_OK);
		return;
	}

	// Creer le SRV pour la texture
	// Le Shader Resource View (SRV) est un descripteur qui définit comment une ressource (ici une texture) est vue par les shaders. Il précise notamment 
	// le format de la texture, le nombre de mipmaps et la dimension de la ressource. Grâce au SRV, le shader (souvent le pixel shader) peut échantillonner 
	// la texture pour en extraire les données de couleur lors du rendu.
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = m_Texture->GetDesc().Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = m_Texture->GetDesc().MipLevels;
	m_Device->CreateShaderResourceView(m_Texture.Get(), &srvDesc, m_SrvHeap->GetCPUDescriptorHandleForHeapStart());


	// Cree la geometrie du cube (ici un cube unitaire)
	CreateVertexBuffer(1.0f);

	// Set primitive topology (invariant pour tous les cubes)
	m_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void MeshFactory::CreateCube(float sizeX, float sizeY, float sizeZ, float posX, float posY, float posZ)
{
	CubeMesh* newCube = new CubeMesh;
	newCube->m_sizeX = sizeX;
	newCube->m_sizeY = sizeY;
	newCube->m_sizeZ = sizeZ;

	newCube->m_posX = posX;
	newCube->m_posY = posY;
	newCube->m_posZ = posZ;

	// Set la position
	newCube->m_Transform.Move(posX, posY, posZ);
	newCube->m_Transform.Scale(sizeX, sizeY, sizeZ);

	// Creer le constant buffer pour ce cube
	CreateCubeConstantBuffer(newCube);

	cubeList.push_back(newCube);
}

void MeshFactory::Update() 
{
	if (!cubeList.empty()) 
	{
		for (auto* cube : cubeList)
		{
			cube->m_Transform.UpdateMatrix();
		}
	}
}

void MeshFactory::Render()
{
	// Configure le root signature et le pipeline
	m_CommandList->SetGraphicsRootSignature(m_RootSignature.Get());
	m_CommandList->SetPipelineState(m_PipelineState.Get());

	// Lier le descriptor heap contenant le SRV de la texture
	ID3D12DescriptorHeap* descriptorHeaps[] = { m_SrvHeap.Get() };
	m_CommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);
	// Le slot 1 de la root signature (table de descripteurs) pointe sur notre texture
	m_CommandList->SetGraphicsRootDescriptorTable(3, m_SrvHeap->GetGPUDescriptorHandleForHeapStart());

	// Definit les vertex et index buffers communs
	m_CommandList->IASetVertexBuffers(0, 1, &m_VertexBufferView);
	m_CommandList->IASetIndexBuffer(&m_IndexBufferView);
	m_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Mes a jour le constant buffer et dessiner chaque cube
	for (auto* cube : cubeList)
	{
		// Calculer la matrice World-View-Projection
		DirectX::XMMATRIX world = XMLoadFloat4x4(&cube->m_Transform.GetMatrix());
		DirectX::XMMATRIX view = m_Camera->GetViewMatrix();
		DirectX::XMMATRIX proj = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, 1.0f, 1.0f, 1000.0f);
		DirectX::XMMATRIX wvp = world * view * proj;

		TransformConstants objConstants;
		XMStoreFloat4x4(&objConstants.WorldViewProj, DirectX::XMMatrixTranspose(wvp));

		// Mettre a jour le constant buffer du cube
		void* pData;
		CD3DX12_RANGE readRange(0, 0);
		cube->m_ConstantBuffer->Map(0, &readRange, &pData);
		memcpy(pData, &objConstants, sizeof(objConstants));
		cube->m_ConstantBuffer->Unmap(0, nullptr);

		// Attacher le constant buffer a la racine (slot 0)
		m_CommandList->SetGraphicsRootConstantBufferView(0, cube->m_ConstantBuffer->GetGPUVirtualAddress());

		// Dessiner le cube (36 indices)
		m_CommandList->DrawIndexedInstanced(36, 1, 0, 0, 0);
	}
}

void MeshFactory::CreateVertexBuffer(float size)
{
	// size = taille du cube
	float halfSize = size / 2.0f;

	VertexMesh vertices[] =
	{
		// Face avant
		{ DirectX::XMFLOAT3(-halfSize, -halfSize, halfSize), { 1, 0, 0, 1 }, { 0.0f, 1.0f } },
		{ DirectX::XMFLOAT3(halfSize, -halfSize, halfSize), { 1, 0, 0, 1 }, { 1.0f, 1.0f } },
		{ DirectX::XMFLOAT3(halfSize,  halfSize, halfSize), { 0, 1, 0, 1 }, { 1.0f, 0.0f } },
		{ DirectX::XMFLOAT3(-halfSize,  halfSize, halfSize), { 1, 0, 0, 1 }, { 0.0f, 0.0f } },	

		// Face arriere
		{ DirectX::XMFLOAT3(-halfSize, -halfSize, -halfSize), { 0, 0, 1, 1 }, { 1.0f, 1.0f } },
		{ DirectX::XMFLOAT3(halfSize, -halfSize, -halfSize), { 0, 0, 1, 1 }, { 0.0f, 1.0f } },
		{ DirectX::XMFLOAT3(halfSize,  halfSize, -halfSize), { 0, 0, 1, 1 }, { 0.0f, 0.0f } },
		{ DirectX::XMFLOAT3(-halfSize,  halfSize, -halfSize), { 0, 1, 0, 1 }, { 1.0f, 0.0f } }
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

	// Create vertex & index buffer
	CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);

	CD3DX12_RESOURCE_DESC vbDesc = CD3DX12_RESOURCE_DESC::Buffer(vSize);
	CD3DX12_RESOURCE_DESC ibDesc = CD3DX12_RESOURCE_DESC::Buffer(iSize);
	
	// Vertex Buffer
	m_Device->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &vbDesc,D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,IID_PPV_ARGS(&m_VertexBuffer));
	// Index Buffer
	m_Device->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &ibDesc,D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,IID_PPV_ARGS(&m_IndexBuffer));

	// Copier les donnees dans le vertex buffer
	void* pVertexData;
	CD3DX12_RANGE readRange(0, 0);
	HRESULT hr = m_VertexBuffer->Map(0, &readRange, &pVertexData);
	memcpy(pVertexData, vertices, vSize);
	m_VertexBuffer->Unmap(0, nullptr);

	// Copier les donnees dans l'index buffer
	void* pIndexData;
	hr = m_IndexBuffer->Map(0, &readRange, &pIndexData);
	memcpy(pIndexData, indices, iSize);
	m_IndexBuffer->Unmap(0, nullptr);

	// Creer les vues
	m_VertexBufferView.BufferLocation = m_VertexBuffer->GetGPUVirtualAddress();
	m_VertexBufferView.StrideInBytes = sizeof(VertexMesh);
	m_VertexBufferView.SizeInBytes = vSize;

	m_IndexBufferView.BufferLocation = m_IndexBuffer->GetGPUVirtualAddress();
	m_IndexBufferView.Format = DXGI_FORMAT_R16_UINT;
	m_IndexBufferView.SizeInBytes = iSize;
}

void MeshFactory::CreatePipelineState()
{
	// Definition d'un parametre root pour un Constant Buffer (CBV)
	CD3DX12_ROOT_PARAMETER slotRootParameter[2]; // 2 parametres au lieu de 1 pour la texture
	// Parametre 0 : constant buffer (transformation)
	slotRootParameter[0].InitAsConstantBufferView(0);
	// Parametre 1 : table de descripteurs pour la texture (register t0)
	CD3DX12_DESCRIPTOR_RANGE texTable;
	texTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
	slotRootParameter[1].InitAsDescriptorTable(1, &texTable, D3D12_SHADER_VISIBILITY_PIXEL);

	// Declarez le static sampler pour register(s0) 
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc(0); // register s0
	samplerDesc.Filter = D3D12_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;	// Creation de la Root Signature

	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(2, slotRootParameter, 1, &samplerDesc,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	/*CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(2, slotRootParameter, 0, nullptr,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);*/

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
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
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


void MeshFactory::CreateCubeConstantBuffer(CubeMesh* cube)
{
	// Taille alignee du constant buffer
	const UINT alignedSize = (sizeof(TransformConstants) + 255) & ~255;
	CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
	CD3DX12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(alignedSize);

	HRESULT hr = m_Device->CreateCommittedResource(&heapProps,D3D12_HEAP_FLAG_NONE,&bufferDesc,D3D12_RESOURCE_STATE_GENERIC_READ,nullptr,IID_PPV_ARGS(&cube->m_ConstantBuffer));

	if (FAILED(hr))
	{
		MessageBox(0, L"Failed to create cube constant buffer!", L"Error", MB_OK);
		return;
	}
}