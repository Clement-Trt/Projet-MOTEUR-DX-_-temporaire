#include "pch.h"

#include "MeshFactory.h"

MeshFactory::MeshFactory()
{
}

void MeshFactory::InitMeshFactory(ID3D12Device* device, EntityManager* entityManager)
{
	m_Device = device;
	m_entityManager = entityManager;

	// Cree la global geometrie des cube
	CreateSharedCubeGeometry();

	if (m_indexBufferView.SizeInBytes < 72)
	{
		MessageBox(0, L"ERREUR: Le buffer d'indices est trop petit!", L"Erreur", MB_OK);
	}
}

CubeMesh* MeshFactory::CreateCube()
{
	CubeMesh* newMesh = new CubeMesh;

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
		{ DirectX::XMFLOAT3(-halfSize, -halfSize, halfSize),  { 1.0f, 0.0f, 0.0f, 1.0f } },
		{ DirectX::XMFLOAT3(halfSize, -halfSize, halfSize),  { 1.0f, 0.0f, 0.0f, 1.0f } },
		{ DirectX::XMFLOAT3(halfSize,  halfSize, halfSize),  { 0.0f, 1.0f, 0.0f, 1.0f } },
		{ DirectX::XMFLOAT3(-halfSize,  halfSize, halfSize),  { 1.0f, 0.0f, 0.0f, 1.0f } },

		// Face arriere
		{ DirectX::XMFLOAT3(-halfSize, -halfSize, -halfSize), { 0.0f, 0.0f, 1.0f, 1.0f } },
		{ DirectX::XMFLOAT3(halfSize, -halfSize, -halfSize), { 0.0f, 0.0f, 1.0f, 1.0f } },
		{ DirectX::XMFLOAT3(halfSize,  halfSize, -halfSize), { 0.0f, 0.0f, 1.0f, 1.0f } },
		{ DirectX::XMFLOAT3(-halfSize,  halfSize, -halfSize), { 0.0f, 1.0f, 0.0f, 1.0f } }
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
