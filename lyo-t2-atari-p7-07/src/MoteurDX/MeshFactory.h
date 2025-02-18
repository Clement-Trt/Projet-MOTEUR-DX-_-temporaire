#pragma once

#include <d3dcompiler.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <wrl.h>
#include "d3dx12.h"

#include <vector>

#include "Transform.h"
#include "Camera.h"

using namespace Microsoft::WRL;

struct CubeMesh 
{
	float m_posX, m_posY, m_posZ;
	float m_sizeX, m_sizeY, m_sizeZ;
	ComPtr<ID3D12Resource> m_ConstantBuffer;
	Transform m_Transform;
};

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

	// Init meshFactory qui recup les commandlist etc...
	void InitMeshFactory(ID3D12Device* device, ID3D12CommandQueue* commandQueue, ID3D12GraphicsCommandList* commandList, IDXGISwapChain3* swapChain, ID3D12DescriptorHeap* rtvHeap, ID3D12DescriptorHeap* dsvHeap, UINT rtvDescriptorSize, D3D12_DEPTH_STENCIL_DESC depthStencilDesc, Camera* camera);

	// Create a mesh inside
	void CreateCube(float sizeX, float sizeY, float sizeZ, float posX, float posY, float posZ);

	// Update all mesh
	void Update();

	// Render all mesh
	void Render();

	// Cree un vertex et l'index buffer commun
	void CreateVertexBuffer(float size);

	std::vector<CubeMesh*>* GetCubeList() { return &cubeList; };
	ID3D12PipelineState* GetPipelineState() { return m_PipelineState.Get(); };
private:
	// Methode pour creer le pipeline (root signature & PSO)
	void CreatePipelineState();

	// Alloue et configure le constant buffer pour un cube
	void CreateCubeConstantBuffer(CubeMesh* cube);

	std::vector<CubeMesh*> cubeList;

	// Camera
	Camera* m_Camera;

	ComPtr<ID3D12Device> m_Device;
	ComPtr<ID3D12CommandQueue> m_CommandQueue;
	ComPtr<ID3D12GraphicsCommandList> m_CommandList;
	ComPtr<IDXGISwapChain3> m_SwapChain;
	ComPtr<ID3D12DescriptorHeap> m_RtvHeap;
	ComPtr<ID3D12CommandAllocator> m_CommandAllocator;
	ComPtr<ID3D12DescriptorHeap> m_DsvHeap;
	
	UINT m_RtvDescriptorSize;
	D3D12_DEPTH_STENCIL_DESC m_DepthStencilDesc;

	ComPtr<ID3D12PipelineState> m_PipelineState;
	ComPtr<ID3D12RootSignature> m_RootSignature;

	//Vertex & Index Buffer
	ComPtr<ID3D12Resource> m_VertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW m_VertexBufferView;
	ComPtr<ID3D12Resource> m_IndexBuffer;
	D3D12_INDEX_BUFFER_VIEW m_IndexBufferView;
};