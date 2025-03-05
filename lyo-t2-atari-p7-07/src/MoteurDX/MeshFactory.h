#pragma once

#include "Transform.h"
#include "EntityManager.h"
#include "Components.h"
#include "WindowDX.h"

using namespace Microsoft::WRL;

struct VertexMesh
{
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT3 Normal;
	DirectX::XMFLOAT4 Color;
	DirectX::XMFLOAT2 TexCoord;
};

struct TransformConstants
{
	DirectX::XMFLOAT4X4 World;
	DirectX::XMFLOAT4X4 WorldViewProj;
};

class MeshFactory
{
public:
	MeshFactory();

	void Initialize(ID3D12Device* device, EntityManager* entityManager, WindowDX* windowDx);

	Mesh* CreateCube();
	Mesh* CreateSkyBoxCube();

private:
	// Type of mesh geometry
	void CreateCubeModel(Mesh* cube);
	void CreateCubeModelForSkyBox(Mesh* cube);

	// Alloue et configure le constant buffer pour un cube
	void CreateCubeConstantBuffer(Mesh* cube);

	// App
	ComPtr<ID3D12Device> m_Device;

	EntityManager* mp_entityManager;
};