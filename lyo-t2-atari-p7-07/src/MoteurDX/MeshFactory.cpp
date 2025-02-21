#include "pch.h"

#include "MeshFactory.h"

MeshFactory::MeshFactory()
{
}

void MeshFactory::InitMeshFactory(ID3D12Device* device, EntityManager* entityManager)
{
	m_Device = device;
	m_entityManager = entityManager;

	CreateSharedCubeGeometry();

	if (m_indexBufferView.SizeInBytes < 72)
	{
		MessageBox(0, L"ERREUR: Le buffer d'indices est trop petit!", L"Erreur", MB_OK);
	}
}

CubeMesh* MeshFactory::CreateCube(float sizeX, float sizeY, float sizeZ, float posX, float posY, float posZ)
{
	CubeMesh* newMesh = new CubeMesh;

	// Cree la geometrie du cube
	//CreateVertexBuffer(newMesh, sizeX);

	newMesh->m_meshIndex = 36;

	// Creer le constant buffer pour ce cube
	CreateCubeConstantBuffer(newMesh);
	newMesh->InitConstantBuffer();

	return newMesh;
}

void MeshFactory::CreateSharedCubeGeometry()
{
	// On cree ici la geometrie d'un cube unitaire (de taille 1) ; pour obtenir des cubes de tailles differentes,
	// on utilisera la transformation (scaling) dans le constant buffer.
	float halfSize = 0.5f;
	VertexMesh vertices[] =
	{
		// Face avant
		{ DirectX::XMFLOAT3(-halfSize, -halfSize, halfSize),  { 1,1,1,1 }, { 0.0f, 1.0f } },
		{ DirectX::XMFLOAT3(halfSize, -halfSize, halfSize),  { 1,1,1,1 }, { 1.0f, 1.0f } },
		{ DirectX::XMFLOAT3(halfSize,  halfSize, halfSize),  { 1,1,1,1 }, { 1.0f, 0.0f } },
		{ DirectX::XMFLOAT3(-halfSize,  halfSize, halfSize),  { 1,1,1,1 }, { 0.0f, 0.0f } },

		// Face arriere
		{ DirectX::XMFLOAT3(-halfSize, -halfSize, -halfSize), { 1,1,1,1 }, { 1.0f, 1.0f } },
		{ DirectX::XMFLOAT3(halfSize, -halfSize, -halfSize), { 1,1,1,1 }, { 0.0f, 1.0f } },
		{ DirectX::XMFLOAT3(halfSize,  halfSize, -halfSize), { 1,1,1,1 }, { 0.0f, 0.0f } },
		{ DirectX::XMFLOAT3(-halfSize,  halfSize, -halfSize), { 1,1,1,1 }, { 1.0f, 0.0f } }
	};

	uint16_t indices[] =
	{
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

	const UINT vSize = sizeof(vertices);
	const UINT iSize = sizeof(indices);

	// On utilise un heap UPLOAD pour permettre au CPU d'ecrire dans la memoire
	CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
	CD3DX12_RESOURCE_DESC vbDesc = CD3DX12_RESOURCE_DESC::Buffer(vSize);
	CD3DX12_RESOURCE_DESC ibDesc = CD3DX12_RESOURCE_DESC::Buffer(iSize);

	// Creer le vertex buffer
	HRESULT hr = m_Device->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE,
		&vbDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_vertexBuffer));
	if (FAILED(hr)) { /* Gerer l'erreur */ }

	// Creer l'index buffer
	hr = m_Device->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE,
		&ibDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_indexBuffer));
	if (FAILED(hr)) { /* Gerer l'erreur */ }

	// Copier les donnees dans le vertex buffer
	void* pVertexData = nullptr;
	CD3DX12_RANGE readRange(0, 0);
	hr = m_vertexBuffer->Map(0, &readRange, &pVertexData);
	memcpy(pVertexData, vertices, vSize);
	m_vertexBuffer->Unmap(0, nullptr);

	// Copier les donnees dans l'index buffer
	void* pIndexData = nullptr;
	hr = m_indexBuffer->Map(0, &readRange, &pIndexData);
	memcpy(pIndexData, indices, iSize);
	m_indexBuffer->Unmap(0, nullptr);

	// Creer les vues
	m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
	m_vertexBufferView.StrideInBytes = sizeof(VertexMesh);
	m_vertexBufferView.SizeInBytes = vSize;

	m_indexBufferView.BufferLocation = m_indexBuffer->GetGPUVirtualAddress();
	m_indexBufferView.Format = DXGI_FORMAT_R16_UINT;
	m_indexBufferView.SizeInBytes = iSize;
}

void MeshFactory::CreateVertexBuffer(CubeMesh* cubeMesh,float cubeSizeX)
{
	// size = taille du cube
	float halfSizeX = cubeSizeX / 2.0f;

	VertexMesh vertices[] =
	{
		// Face avant
		{ DirectX::XMFLOAT3(-halfSizeX, -halfSizeX, halfSizeX), { 1.0f, 0.0f, 0.0f, 1.0f } }, // Bas gauche  (0)
		{ DirectX::XMFLOAT3(halfSizeX,  -halfSizeX, halfSizeX), { 1.0f, 0.0f, 0.0f, 1.0f } }, // Bas droit   (1)
		{ DirectX::XMFLOAT3(halfSizeX,  halfSizeX, halfSizeX), { 0.0f, 1.0f, 0.0f, 1.0f } },  // Haut droit  (2)
		{ DirectX::XMFLOAT3(-halfSizeX, halfSizeX, halfSizeX), { 1.0f, 0.0f, 0.0f, 1.0f } },  // Haut gauche (3)

		// Face arriere
		{ DirectX::XMFLOAT3(-halfSizeX, -halfSizeX, -halfSizeX), { 0.0f, 0.0f, 1.0f, 1.0f } },  // Bas gauche  (4)
		{ DirectX::XMFLOAT3(halfSizeX,  -halfSizeX, -halfSizeX), { 0.0f, 0.0f, 1.0f, 1.0f } },  // Bas droit   (5)
		{ DirectX::XMFLOAT3(halfSizeX,  halfSizeX, -halfSizeX), { 0.0f, 0.0f, 1.0f, 1.0f } },   // Haut droit  (6)
		{ DirectX::XMFLOAT3(-halfSizeX, halfSizeX, -halfSizeX), { 0.0f, 1.0f, 0.0f, 1.0f } }    // Haut gauche (7)
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

	//// 6 faces * 4 sommets chacune = 24 sommets
	//VertexMesh vertices[24] =
	//{
	//	// Front face (z = +halfSize)
	//	{ DirectX::XMFLOAT3(-halfSize, -halfSize,  halfSize), {1,1,1,1}, {0.0f, 1.0f} },
	//	{ DirectX::XMFLOAT3(halfSize, -halfSize,  halfSize), {1,1,1,1}, {1.0f, 1.0f} },
	//	{ DirectX::XMFLOAT3(halfSize,  halfSize,  halfSize), {1,1,1,1}, {1.0f, 0.0f} },
	//	{ DirectX::XMFLOAT3(-halfSize,  halfSize,  halfSize), {1,1,1,1}, {0.0f, 0.0f} },

	//	// Back face (z = -halfSize)
	//	{ DirectX::XMFLOAT3(halfSize, -halfSize, -halfSize), {1,1,1,1}, {0.0f, 1.0f} },
	//	{ DirectX::XMFLOAT3(-halfSize, -halfSize, -halfSize), {1,1,1,1}, {1.0f, 1.0f} },
	//	{ DirectX::XMFLOAT3(-halfSize,  halfSize, -halfSize), {1,1,1,1}, {1.0f, 0.0f} },
	//	{ DirectX::XMFLOAT3(halfSize,  halfSize, -halfSize), {1,1,1,1}, {0.0f, 0.0f} },

	//	// Left face (x = -halfSize)
	//	{ DirectX::XMFLOAT3(-halfSize, -halfSize, -halfSize), {1,1,1,1}, {0.0f, 1.0f} },
	//	{ DirectX::XMFLOAT3(-halfSize, -halfSize,  halfSize), {1,1,1,1}, {1.0f, 1.0f} },
	//	{ DirectX::XMFLOAT3(-halfSize,  halfSize,  halfSize), {1,1,1,1}, {1.0f, 0.0f} },
	//	{ DirectX::XMFLOAT3(-halfSize,  halfSize, -halfSize), {1,1,1,1}, {0.0f, 0.0f} },

	//	// Right face (x = +halfSize)
	//	{ DirectX::XMFLOAT3(halfSize, -halfSize,  halfSize), {1,1,1,1}, {0.0f, 1.0f} },
	//	{ DirectX::XMFLOAT3(halfSize, -halfSize, -halfSize), {1,1,1,1}, {1.0f, 1.0f} },
	//	{ DirectX::XMFLOAT3(halfSize,  halfSize, -halfSize), {1,1,1,1}, {1.0f, 0.0f} },
	//	{ DirectX::XMFLOAT3(halfSize,  halfSize,  halfSize), {1,1,1,1}, {0.0f, 0.0f} },

	//	// Top face (y = +halfSize)
	//	{ DirectX::XMFLOAT3(-halfSize,  halfSize,  halfSize), {1,1,1,1}, {0.0f, 1.0f} },
	//	{ DirectX::XMFLOAT3(halfSize,  halfSize,  halfSize), {1,1,1,1}, {1.0f, 1.0f} },
	//	{ DirectX::XMFLOAT3(halfSize,  halfSize, -halfSize), {1,1,1,1}, {1.0f, 0.0f} },
	//	{ DirectX::XMFLOAT3(-halfSize,  halfSize, -halfSize), {1,1,1,1}, {0.0f, 0.0f} },

	//	// Bottom face (y = -halfSize)
	//	{ DirectX::XMFLOAT3(-halfSize, -halfSize, -halfSize), {1,1,1,1}, {0.0f, 1.0f} },
	//	{ DirectX::XMFLOAT3(halfSize, -halfSize, -halfSize), {1,1,1,1}, {1.0f, 1.0f} },
	//	{ DirectX::XMFLOAT3(halfSize, -halfSize,  halfSize), {1,1,1,1}, {1.0f, 0.0f} },
	//	{ DirectX::XMFLOAT3(-halfSize, -halfSize,  halfSize), {1,1,1,1}, {0.0f, 0.0f} },
	//};

	//// Indices : 6 faces * 2 triangles par face * 3 indices par triangle = 36 indices
	//uint16_t indices[36] =
	//{
	//	// Front face
	//	0,  1,  2,  0,  2,  3,
	//	// Back face
	//	4,  5,  6,  4,  6,  7,
	//	// Left face
	//	8,  9, 10,  8, 10, 11,
	//	// Right face
	//	12, 13, 14, 12, 14, 15,
	//	// Top face
	//	16, 17, 18, 16, 18, 19,
	//	// Bottom face
	//	20, 21, 22, 20, 22, 23
	//};

	// Creation des buffers pour stocker les sommets et indices
	const UINT vSize = sizeof(vertices);
	const UINT iSize = sizeof(indices);

	// Create vertex & index buffer
	CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);

	CD3DX12_RESOURCE_DESC vbDesc = CD3DX12_RESOURCE_DESC::Buffer(vSize);
	CD3DX12_RESOURCE_DESC ibDesc = CD3DX12_RESOURCE_DESC::Buffer(iSize);
	
	// Vertex Buffer
	m_Device->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &vbDesc,D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,IID_PPV_ARGS(&m_vertexBuffer));
	// Index Buffer
	m_Device->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &ibDesc,D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,IID_PPV_ARGS(&m_indexBuffer));

	// Copier les donnes dans le vertex buffer
	void* pVertexData;
	CD3DX12_RANGE readRange(0, 0);
	HRESULT hr = m_vertexBuffer.Get()->Map(0, &readRange, &pVertexData);
	memcpy(pVertexData, vertices, vSize);
	m_vertexBuffer.Get()->Unmap(0, nullptr);

	// Copier les donnes dans l'index buffer
	void* pIndexData;
	hr = m_indexBuffer.Get()->Map(0, &readRange, &pIndexData);
	memcpy(pIndexData, indices, iSize);
	m_indexBuffer.Get()->Unmap(0, nullptr);

	// Creer les vues
	m_vertexBufferView.BufferLocation = m_vertexBuffer.Get()->GetGPUVirtualAddress();
	m_vertexBufferView.StrideInBytes = sizeof(VertexMesh);
	m_vertexBufferView.SizeInBytes = vSize;

	m_indexBufferView.BufferLocation = m_indexBuffer.Get()->GetGPUVirtualAddress();
	m_indexBufferView.Format = DXGI_FORMAT_R16_UINT;
	m_indexBufferView.SizeInBytes = iSize;
}

void MeshFactory::CreateCubeConstantBuffer(CubeMesh* cube)
{
	// Taille alignee du constant buffer
	const UINT alignedSize = (sizeof(TransformConstants) + 255) & ~255;
	CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
	CD3DX12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(alignedSize);

	HRESULT hr = m_Device->CreateCommittedResource(&heapProps,D3D12_HEAP_FLAG_NONE,&bufferDesc,D3D12_RESOURCE_STATE_GENERIC_READ,nullptr,IID_PPV_ARGS(&cube->m_constantBuffer));

	if (FAILED(hr))
	{
		MessageBox(0, L"Failed to create cube constant buffer!", L"Error", MB_OK);
		return;
	}
}
