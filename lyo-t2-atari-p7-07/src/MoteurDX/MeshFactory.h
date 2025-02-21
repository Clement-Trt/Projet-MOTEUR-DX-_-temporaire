#pragma once

#include "d3dx12.h"

#include "Transform.h"

#include "EntityManager.h"
#include "Components.h"

using namespace Microsoft::WRL;

struct VertexMesh
{
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT4 Color;
};

// Structure pour les constantes (matrice WorldViewProj)
struct TransformConstants
{
	DirectX::XMFLOAT4X4 WorldViewProj;
};

class MeshFactory
{
public:
	MeshFactory();

	// Init meshFactory
	void InitMeshFactory(ID3D12Device* device, EntityManager* entityManager);

	// Create a mesh inside
	CubeMesh* CreateCube();

	void CreateSharedCubeGeometry();

	D3D12_VERTEX_BUFFER_VIEW* GetVertexBufferView() { return &m_vertexBufferView; };
	D3D12_INDEX_BUFFER_VIEW* GetIndexBufferView() { return &m_indexBufferView; };
private:
	// Cree un vertex et l'index buffer commun
	void CreateVertexBuffer(CubeMesh* cubeMesh, float cubeSizeX);

	// Alloue et configure le constant buffer pour un cube
	void CreateCubeConstantBuffer(CubeMesh* cube);

	// App
	ComPtr<ID3D12Device> m_Device;

	// Entity manager
	EntityManager* m_entityManager;

	// Vertex Buffer
	ComPtr<ID3D12Resource> m_vertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;

	// Index Buffer
	ComPtr<ID3D12Resource> m_indexBuffer;
	D3D12_INDEX_BUFFER_VIEW m_indexBufferView;
};