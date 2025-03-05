#include "pch.h"

#include "MeshFactory.h"
// #include "TextureLoader.h"

MeshFactory::MeshFactory()
{
}

void MeshFactory::Initialize(ID3D12Device* device, EntityManager* entityManager, WindowDX* windowDx)
{
	m_Device = device;
	m_entityManager = entityManager;
}

Mesh* MeshFactory::CreateCube()
{
	Mesh* newMesh = new Mesh;

	// Nombre d'indice de la forme (36 points pour un cube)
	newMesh->m_geometryMesh.m_meshIndex = 36;

	// Creer la geometrie du cube
	CreateCubeModel(newMesh);

	// Creer le constant buffer pour ce cube
	CreateCubeConstantBuffer(newMesh);

	// InitMap
	CD3DX12_RANGE readRange(0, 0);
	newMesh->m_constantBuffer->Map(0, &readRange, &newMesh->m_mappedData);

	return newMesh;
}
Mesh* MeshFactory::CreateSkyBoxCube()
{
	Mesh* newMesh = new Mesh;

	newMesh->m_geometryMesh.m_meshIndex = 36;

	CreateCubeModelForSkyBox(newMesh);

	// Créer le constant buffer comme d'habitude
	CreateCubeConstantBuffer(newMesh);

	// Mappage du buffer constant
	CD3DX12_RANGE readRange(0, 0);
	newMesh->m_constantBuffer->Map(0, &readRange, &newMesh->m_mappedData);

	return newMesh;
}

void MeshFactory::CreateCubeModel(Mesh* cube)
{
	// On cree ici la geometrie d'un cube unitaire (de taille 1) ; pour obtenir des cubes de tailles differentes,
	// on utilisera la transformation (scaling) dans le constant buffer.
	float halfSize = 0.5f;
	
	// 6 faces * 4 sommets chacune = 24 sommets
	VertexMesh vertices[24] =
	{
		// Front face (z = +halfSize)
		{ DirectX::XMFLOAT3(-halfSize, -halfSize,  halfSize), {1,1,1,1}, {0.0f, 1.0f} },
		{ DirectX::XMFLOAT3(halfSize, -halfSize,  halfSize), {1,1,1,1}, {1.0f, 1.0f} },
		{ DirectX::XMFLOAT3(halfSize,  halfSize,  halfSize), {1,1,1,1}, {1.0f, 0.0f} },
		{ DirectX::XMFLOAT3(-halfSize,  halfSize,  halfSize), {1,1,1,1}, {0.0f, 0.0f} },

		// Back face (z = -halfSize)
		{ DirectX::XMFLOAT3(halfSize, -halfSize, -halfSize), {1,1,1,1}, {0.0f, 1.0f} },
		{ DirectX::XMFLOAT3(-halfSize, -halfSize, -halfSize), {1,1,1,1}, {1.0f, 1.0f} },
		{ DirectX::XMFLOAT3(-halfSize,  halfSize, -halfSize), {1,1,1,1}, {1.0f, 0.0f} },
		{ DirectX::XMFLOAT3(halfSize,  halfSize, -halfSize), {1,1,1,1}, {0.0f, 0.0f} },

		// Left face (x = -halfSize)
		{ DirectX::XMFLOAT3(-halfSize, -halfSize, -halfSize), {1,1,1,1}, {0.0f, 1.0f} },
		{ DirectX::XMFLOAT3(-halfSize, -halfSize,  halfSize), {1,1,1,1}, {1.0f, 1.0f} },
		{ DirectX::XMFLOAT3(-halfSize,  halfSize,  halfSize), {1,1,1,1}, {1.0f, 0.0f} },
		{ DirectX::XMFLOAT3(-halfSize,  halfSize, -halfSize), {1,1,1,1}, {0.0f, 0.0f} },

		// Right face (x = +halfSize)
		{ DirectX::XMFLOAT3(halfSize, -halfSize,  halfSize), {1,1,1,1}, {0.0f, 1.0f} },
		{ DirectX::XMFLOAT3(halfSize, -halfSize, -halfSize), {1,1,1,1}, {1.0f, 1.0f} },
		{ DirectX::XMFLOAT3(halfSize,  halfSize, -halfSize), {1,1,1,1}, {1.0f, 0.0f} },
		{ DirectX::XMFLOAT3(halfSize,  halfSize,  halfSize), {1,1,1,1}, {0.0f, 0.0f} },

		// Top face (y = +halfSize)
		{ DirectX::XMFLOAT3(-halfSize,  halfSize,  halfSize), {1,1,1,1}, {0.0f, 1.0f} },
		{ DirectX::XMFLOAT3(halfSize,  halfSize,  halfSize), {1,1,1,1}, {1.0f, 1.0f} },
		{ DirectX::XMFLOAT3(halfSize,  halfSize, -halfSize), {1,1,1,1}, {1.0f, 0.0f} },
		{ DirectX::XMFLOAT3(-halfSize,  halfSize, -halfSize), {1,1,1,1}, {0.0f, 0.0f} },

		// Bottom face (y = -halfSize)
		{ DirectX::XMFLOAT3(-halfSize, -halfSize, -halfSize), {1,1,1,1}, {0.0f, 1.0f} },
		{ DirectX::XMFLOAT3(halfSize, -halfSize, -halfSize), {1,1,1,1}, {1.0f, 1.0f} },
		{ DirectX::XMFLOAT3(halfSize, -halfSize,  halfSize), {1,1,1,1}, {1.0f, 0.0f} },
		{ DirectX::XMFLOAT3(-halfSize, -halfSize,  halfSize), {1,1,1,1}, {0.0f, 0.0f} },
	};

	// Indices : 6 faces * 2 triangles par face * 3 indices par triangle = 36 indices
	uint16_t indices[36] =
	{
		// Front face
		0,  1,  2,  0,  2,  3,
		// Back face
		4,  5,  6,  4,  6,  7,
		// Left face
		8,  9, 10,  8, 10, 11,
		// Right face
		12, 13, 14, 12, 14, 15,
		// Top face
		16, 17, 18, 16, 18, 19,
		// Bottom face
		20, 21, 22, 20, 22, 23
	};

	const UINT vSize = sizeof(vertices);
	const UINT iSize = sizeof(indices);

	// On utilise un heap UPLOAD pour permettre au CPU d'ecrire dans la memoire
	CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
	CD3DX12_RESOURCE_DESC vbDesc = CD3DX12_RESOURCE_DESC::Buffer(vSize);
	CD3DX12_RESOURCE_DESC ibDesc = CD3DX12_RESOURCE_DESC::Buffer(iSize);

	// Creer le vertex buffer
	ComPtr<ID3D12Resource>& cubeVertexBuffer = cube->m_geometryMesh.m_vertexBuffer;

	HRESULT hr = m_Device->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE,
		&vbDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&cubeVertexBuffer));
	if (FAILED(hr)) { /* Gerer l'erreur */ }

	// Creer l'index buffer
	ComPtr<ID3D12Resource>& cubeIndexBuffer = cube->m_geometryMesh.m_indexBuffer;

	hr = m_Device->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE,
		&ibDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&cubeIndexBuffer));
	if (FAILED(hr)) { /* Gerer l'erreur */ }

	// Copier les donnees dans le vertex buffer
	void* pVertexData = nullptr;
	CD3DX12_RANGE readRange(0, 0);
	hr = cubeVertexBuffer->Map(0, &readRange, &pVertexData);
	memcpy(pVertexData, vertices, vSize);
	cubeVertexBuffer->Unmap(0, nullptr);

	// Copier les donnees dans l'index buffer
	void* pIndexData = nullptr;
	hr = cubeIndexBuffer->Map(0, &readRange, &pIndexData);
	memcpy(pIndexData, indices, iSize);
	cubeIndexBuffer->Unmap(0, nullptr);

	// Creer les vues
	D3D12_VERTEX_BUFFER_VIEW& cubeBufferView = cube->m_geometryMesh.m_vertexBufferView;

	cubeBufferView.BufferLocation = cubeVertexBuffer->GetGPUVirtualAddress();
	cubeBufferView.StrideInBytes = sizeof(VertexMesh);
	cubeBufferView.SizeInBytes = vSize;

	D3D12_INDEX_BUFFER_VIEW& cubeIndexView = cube->m_geometryMesh.m_indexBufferView;

	cubeIndexView.BufferLocation = cubeIndexBuffer->GetGPUVirtualAddress();
	cubeIndexView.Format = DXGI_FORMAT_R16_UINT;
	cubeIndexView.SizeInBytes = iSize;
}

void MeshFactory::CreateCubeModelForSkyBox(Mesh* cube)
{
	// On cree ici la geometrie d'un cube unitaire (de taille 1) ; pour obtenir des cubes de tailles differentes,
	// on utilisera la transformation (scaling) dans le constant buffer.
	float halfSize = 0.5f;
	
	// 6 faces * 4 sommets chacune = 24 sommets
	VertexMesh vertices[24] =
	{
		// Front face (z = +halfSize)
		{ DirectX::XMFLOAT3(-halfSize, -halfSize,  halfSize), {1,1,1,1}, {0.0f, 1.0f} },
		{ DirectX::XMFLOAT3(halfSize, -halfSize,  halfSize), {1,1,1,1}, {1.0f, 1.0f} },
		{ DirectX::XMFLOAT3(halfSize,  halfSize,  halfSize), {1,1,1,1}, {1.0f, 0.0f} },
		{ DirectX::XMFLOAT3(-halfSize,  halfSize,  halfSize), {1,1,1,1}, {0.0f, 0.0f} },

		// Back face (z = -halfSize)
		{ DirectX::XMFLOAT3(halfSize, -halfSize, -halfSize), {1,1,1,1}, {0.0f, 1.0f} },
		{ DirectX::XMFLOAT3(-halfSize, -halfSize, -halfSize), {1,1,1,1}, {1.0f, 1.0f} },
		{ DirectX::XMFLOAT3(-halfSize,  halfSize, -halfSize), {1,1,1,1}, {1.0f, 0.0f} },
		{ DirectX::XMFLOAT3(halfSize,  halfSize, -halfSize), {1,1,1,1}, {0.0f, 0.0f} },

		// Left face (x = -halfSize)
		{ DirectX::XMFLOAT3(-halfSize, -halfSize, -halfSize), {1,1,1,1}, {0.0f, 1.0f} },
		{ DirectX::XMFLOAT3(-halfSize, -halfSize,  halfSize), {1,1,1,1}, {1.0f, 1.0f} },
		{ DirectX::XMFLOAT3(-halfSize,  halfSize,  halfSize), {1,1,1,1}, {1.0f, 0.0f} },
		{ DirectX::XMFLOAT3(-halfSize,  halfSize, -halfSize), {1,1,1,1}, {0.0f, 0.0f} },

		// Right face (x = +halfSize)
		{ DirectX::XMFLOAT3(halfSize, -halfSize,  halfSize), {1,1,1,1}, {0.0f, 1.0f} },
		{ DirectX::XMFLOAT3(halfSize, -halfSize, -halfSize), {1,1,1,1}, {1.0f, 1.0f} },
		{ DirectX::XMFLOAT3(halfSize,  halfSize, -halfSize), {1,1,1,1}, {1.0f, 0.0f} },
		{ DirectX::XMFLOAT3(halfSize,  halfSize,  halfSize), {1,1,1,1}, {0.0f, 0.0f} },

		// Top face (y = +halfSize)
		{ DirectX::XMFLOAT3(-halfSize,  halfSize,  halfSize), {1,1,1,1}, {0.0f, 1.0f} },
		{ DirectX::XMFLOAT3(halfSize,  halfSize,  halfSize), {1,1,1,1}, {1.0f, 1.0f} },
		{ DirectX::XMFLOAT3(halfSize,  halfSize, -halfSize), {1,1,1,1}, {1.0f, 0.0f} },
		{ DirectX::XMFLOAT3(-halfSize,  halfSize, -halfSize), {1,1,1,1}, {0.0f, 0.0f} },

		// Bottom face (y = -halfSize)
		{ DirectX::XMFLOAT3(-halfSize, -halfSize, -halfSize), {1,1,1,1}, {0.0f, 1.0f} },
		{ DirectX::XMFLOAT3(halfSize, -halfSize, -halfSize), {1,1,1,1}, {1.0f, 1.0f} },
		{ DirectX::XMFLOAT3(halfSize, -halfSize,  halfSize), {1,1,1,1}, {1.0f, 0.0f} },
		{ DirectX::XMFLOAT3(-halfSize, -halfSize,  halfSize), {1,1,1,1}, {0.0f, 0.0f} },
	};

	// Indices : 6 faces * 2 triangles par face * 3 indices par triangle = 36 indices
	// Inverses pour voir depuis l'interieur
	uint16_t indices[36] =
	{
		// Front face
		0,  2,  1,  0,  3,  2,
		// Back face
		4,  6,  5,  4,  7,  6,
		// Left face
		8,  10, 9,  8,  11, 10,
		// Right face
		12, 14, 13, 12, 15, 14,
		// Top face
		16, 18, 17, 16, 19, 18,
		// Bottom face
		20, 22, 21, 20, 23, 22
	};

	const UINT vSize = sizeof(vertices);
	const UINT iSize = sizeof(indices);

	// On utilise un heap UPLOAD pour permettre au CPU d'ecrire dans la memoire
	CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
	CD3DX12_RESOURCE_DESC vbDesc = CD3DX12_RESOURCE_DESC::Buffer(vSize);
	CD3DX12_RESOURCE_DESC ibDesc = CD3DX12_RESOURCE_DESC::Buffer(iSize);

	// Creer le vertex buffer
	ComPtr<ID3D12Resource>& cubeVertexBuffer = cube->m_geometryMesh.m_vertexBuffer;

	HRESULT hr = m_Device->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE,
		&vbDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&cubeVertexBuffer));
	if (FAILED(hr)) { /* Gerer l'erreur */ }

	// Creer l'index buffer
	ComPtr<ID3D12Resource>& cubeIndexBuffer = cube->m_geometryMesh.m_indexBuffer;

	hr = m_Device->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE,
		&ibDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&cubeIndexBuffer));
	if (FAILED(hr)) { /* Gerer l'erreur */ }

	// Copier les donnees dans le vertex buffer
	void* pVertexData = nullptr;
	CD3DX12_RANGE readRange(0, 0);
	hr = cubeVertexBuffer->Map(0, &readRange, &pVertexData);
	memcpy(pVertexData, vertices, vSize);
	cubeVertexBuffer->Unmap(0, nullptr);

	// Copier les donnees dans l'index buffer
	void* pIndexData = nullptr;
	hr = cubeIndexBuffer->Map(0, &readRange, &pIndexData);
	memcpy(pIndexData, indices, iSize);
	cubeIndexBuffer->Unmap(0, nullptr);

	// Creer les vues
	D3D12_VERTEX_BUFFER_VIEW& cubeBufferView = cube->m_geometryMesh.m_vertexBufferView;

	cubeBufferView.BufferLocation = cubeVertexBuffer->GetGPUVirtualAddress();
	cubeBufferView.StrideInBytes = sizeof(VertexMesh);
	cubeBufferView.SizeInBytes = vSize;

	D3D12_INDEX_BUFFER_VIEW& cubeIndexView = cube->m_geometryMesh.m_indexBufferView;

	cubeIndexView.BufferLocation = cubeIndexBuffer->GetGPUVirtualAddress();
	cubeIndexView.Format = DXGI_FORMAT_R16_UINT;
	cubeIndexView.SizeInBytes = iSize;
}

//Mesh* MeshFactory::CreateSphere(int latitudeBands, int longitudeBands)
//{
//	Mesh* newMesh = new Mesh;
//
//	std::vector<VertexMesh> vertices;
//	std::vector<uint16_t> indices;
//
//	float radius = 0.5f;
//	for (int lat = 0; lat <= latitudeBands; lat++) {
//		float theta = lat * DirectX::XM_PI / latitudeBands;
//		float sinTheta = sinf(theta);
//		float cosTheta = cosf(theta);
//
//		for (int lon = 0; lon <= longitudeBands; lon++) {
//			float phi = lon * 2 * DirectX::XM_PI / longitudeBands;
//			float sinPhi = sinf(phi);
//			float cosPhi = cosf(phi);
//
//			DirectX::XMFLOAT3 position(
//				radius * cosPhi * sinTheta,
//				radius * cosTheta,
//				radius * sinPhi * sinTheta
//			);
//			DirectX::XMFLOAT2 texCoord(
//				1.0f - ((float)lon / longitudeBands),
//				1.0f - ((float)lat / latitudeBands)
//			);
//			DirectX::XMFLOAT4 color(1, 1, 1, 1);
//
//			vertices.push_back({ position, color, texCoord });
//		}
//	}
//
//	for (int lat = 0; lat < latitudeBands; lat++) {
//		for (int lon = 0; lon < longitudeBands; lon++) {
//			int first = (lat * (longitudeBands + 1)) + lon;
//			int second = first + longitudeBands + 1;
//
//			indices.push_back(first);
//			indices.push_back(second);
//			indices.push_back(first + 1);
//
//			indices.push_back(second);
//			indices.push_back(second + 1);
//			indices.push_back(first + 1);
//		}
//	}
//
//	newMesh->m_geometryMesh.m_meshIndex = static_cast<UINT>(indices.size());
//
//	CreateBuffer(newMesh, vertices.data(), sizeof(VertexMesh) * vertices.size(), indices.data(), sizeof(uint16_t) * indices.size());
//
//	CD3DX12_RANGE readRange(0, 0);
//	newMesh->m_constantBuffer->Map(0, &readRange, &newMesh->m_mappedData);
//
//	return newMesh;
//}


void MeshFactory::CreateCubeConstantBuffer(Mesh* cube)
{
	// Taille alignee du constant buffer
	const UINT alignedSize = (sizeof(TransformConstants) + 255) & ~255;
	CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
	CD3DX12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(alignedSize);

	HRESULT hr = m_Device->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &bufferDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&cube->m_constantBuffer));

	if (FAILED(hr))
	{
		MessageBox(0, L"Failed to create cube constant buffer!", L"Error", MB_OK);
		return;
	}
}