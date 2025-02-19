#include "pch.h"

#include "MeshFactory.h"
#include "GameManager.h"
#include "EntityManager.h"
#include "Components.h"

MeshFactory::MeshFactory()
{
}

void MeshFactory::InitMeshFactory(ID3D12Device* device, EntityManager* entityManager)
{
	m_Device = device;
	m_entityManager = entityManager;
}

//CubeMesh* MeshFactory::CreateCube(Entity* entity, float sizeX, float sizeY, float sizeZ, float posX, float posY, float posZ)
//{
//	
//	//MeshComponent* newCube = static_cast<MeshComponent*>(m_entityManager->GetComponentsTab()[entity->tab_index]->tab_components[Mesh_index]);
//	//TransformComponent* transform = static_cast<TransformComponent*>(m_entityManager->GetComponentsTab()[entity->tab_index]->tab_components[Transform_index]);
//
//	//transform->m_transform.Move(posX, posY, posZ);
//	//transform->m_transform.Scale(sizeX, sizeY, sizeZ);
//
//	//// Cree la geometrie du cube
//	//CreateVertexBuffer(&newCube->m_cubeMesh, sizeX);
//
//	//// Creer le constant buffer pour ce cube
//	//CreateCubeConstantBuffer(&newCube->m_cubeMesh);
//
//	//return &newCube->m_cubeMesh;
//}

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

	// Creation des buffers pour stocker les sommets et indices
	const UINT vSize = sizeof(vertices);
	const UINT iSize = sizeof(indices);

	// Create vertex & index buffer
	CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);

	CD3DX12_RESOURCE_DESC vbDesc = CD3DX12_RESOURCE_DESC::Buffer(vSize);
	CD3DX12_RESOURCE_DESC ibDesc = CD3DX12_RESOURCE_DESC::Buffer(iSize);
	
	// Vertex Buffer
	m_Device->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &vbDesc,D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,IID_PPV_ARGS(&cubeMesh->m_vertexBuffer));
	// Index Buffer
	m_Device->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &ibDesc,D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,IID_PPV_ARGS(&cubeMesh->m_indexBuffer));

	// Copier les données dans le vertex buffer
	void* pVertexData;
	CD3DX12_RANGE readRange(0, 0);
	HRESULT hr = cubeMesh->m_vertexBuffer.Get()->Map(0, &readRange, &pVertexData);
	memcpy(pVertexData, vertices, vSize);
	cubeMesh->m_vertexBuffer.Get()->Unmap(0, nullptr);

	// Copier les données dans l'index buffer
	void* pIndexData;
	hr = cubeMesh->m_indexBuffer.Get()->Map(0, &readRange, &pIndexData);
	memcpy(pIndexData, indices, iSize);
	cubeMesh->m_indexBuffer.Get()->Unmap(0, nullptr);

	// Creer les vues
	cubeMesh->m_vertexBufferView.BufferLocation = cubeMesh->m_vertexBuffer.Get()->GetGPUVirtualAddress();
	cubeMesh->m_vertexBufferView.StrideInBytes = sizeof(VertexMesh);
	cubeMesh->m_vertexBufferView.SizeInBytes = vSize;

	cubeMesh->m_indexBufferView.BufferLocation = cubeMesh->m_indexBuffer.Get()->GetGPUVirtualAddress();
	cubeMesh->m_indexBufferView.Format = DXGI_FORMAT_R16_UINT;
	cubeMesh->m_indexBufferView.SizeInBytes = iSize;
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
