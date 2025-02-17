#include "pch.h"
#include "MeshFactory.h"

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

	// Configurer le root signature et le pipeline une seule fois
	m_CommandList->SetGraphicsRootSignature(m_RootSignature.Get());
	m_CommandList->SetPipelineState(m_PipelineState.Get());

	// Set primitive topology (invariant pour tous les cubes)
	m_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void MeshFactory::CreateCube(float sizeX, float sizeY, float sizeZ, float posX, float posY, float posZ)
{
	CubeMesh newCube;
	newCube.m_sizeX = sizeX;
	newCube.m_sizeY = sizeY;
	newCube.m_sizeZ = sizeZ;

	newCube.m_posX = posX;
	newCube.m_posY = posY;
	newCube.m_posZ = posZ;

	cubeList.push_back(&newCube);
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
	for (auto* cube : cubeList)
	{
		// Configurer le root signature pour le pipeline.
		//m_CommandList->SetGraphicsRootSignature(m_RootSignature.Get());

		//// Definir l'objet de l'etat du pipeline (PSO) contenant la configuration graphique.
		//m_CommandList->SetPipelineState(m_PipelineState.Get());

		//// Attacher le constant buffer (contenant la matrice World-View-Projection) au pipeline.
		//m_CommandList->SetGraphicsRootConstantBufferView(0, m_ConstantBuffer->GetGPUVirtualAddress());

		//// Set primitive topology
		//m_CommandList->IASetVertexBuffers(0, 1, &m_VertexBufferView);
		//m_CommandList->IASetIndexBuffer(&m_IndexBufferView);
		//m_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//// Draw the triangle
		//// emettre l'appel de dessin : 36 indices pour dessiner le cube.
		//// m_CommandList->DrawInstanced(36, 1, 0, 0);
		//// Nb de vertices | InstanceCount | StartIndexLocation | BaseVertexLocation | StartInstanceLocation
		//m_CommandList->DrawIndexedInstanced(36, 1, 0, 0, 0);
	}
}

void MeshFactory::CreateVertexBuffer(float size)
{
	// size = taille du cube
	float halfSize = size / 2.0f;

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

	// Create vertex & index buffer
	CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);

	CD3DX12_RESOURCE_DESC vbDesc = CD3DX12_RESOURCE_DESC::Buffer(vSize);
	CD3DX12_RESOURCE_DESC ibDesc = CD3DX12_RESOURCE_DESC::Buffer(iSize);
	
	// Vertex Buffer
	m_Device->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &vbDesc,D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,IID_PPV_ARGS(&m_VertexBuffer));
	// Index Buffer
	m_Device->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &ibDesc,D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,IID_PPV_ARGS(&m_IndexBuffer));
}
