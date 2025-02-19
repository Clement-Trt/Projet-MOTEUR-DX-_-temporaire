#include "pch.h"

#include "InitDirect3DApp.h"
#include <d3dcompiler.h>

#include "MeshFactory.h"
#include "InputManager.h"
#include "Scene.h"
#include "EntityManager.h"
#include "SceneTest.h"


InitDirect3DApp::InitDirect3DApp(HINSTANCE hInstance) : WindowDX(hInstance)
{
}

bool InitDirect3DApp::Initialize()
{
	if (!WindowDX::Initialize())
		return false;

	// Initialisation Game Manager et Scene (ECS)
	m_entityManager = new EntityManager();

	// MeshFactory
	m_meshFactory = new MeshFactory;
	m_meshFactory->InitMeshFactory(mD3DDevice.Get(), GetEntityManager());
	MessageBox(0, L"InitReussiMeshFacto", 0, 0);

	// Scene
	SceneTest* scene = new SceneTest;
	SetScene(scene);
	mScene->Initialize(this);
	mScene->OnInitialize();

	// Cree le pipeline(root signature & PSO)
	CreatePipelineState();

	// Set primitive topology (invariant pour tous les cubes)
	mCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Positionner la camera a une position initiale
	m_Camera.SetPosition(0.0f, 0.0f, -5.0f); // Place la camera en arriere pour voir la scene

	m_depthStencilDesc = {};
	m_depthStencilDesc.DepthEnable = TRUE;
	m_depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	m_depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	m_depthStencilDesc.StencilEnable = FALSE;

	float squareSize = 1.0f;
	DirectX::XMFLOAT4 squareColor = { 1.0f, 0.0f, 0.0f, 0.0f };

	/*m_meshFactory->CreateCube(1.0f, 1.0f, 1.0f, 0.0f, 2.0f, 0.0f);
	m_meshFactory->CreateCube(3.0f, 3.0f, 3.0f, 0.0f, 0.0f, 0.0f);
	m_meshFactory->CreateCube(2.0f, 2.0f, 2.0f, 3.0f, 0.0f, 0.0f);
	m_meshFactory->CreateCube(3.0f, 3.0f, 3.0f, 0.0f, 0.0f, 0.0f);
	m_meshFactory->CreateCube(3.0f, 1.0f, 3.0f, 0.0f, 0.0f, 0.0f);*/
	//MessageBox(0, L"CreationDuCube", 0, 0);

	mPSO = mPipelineState;


	return true;
}
void InitDirect3DApp::Update()
{
	//SetDeltaTime(mDeltaTime); // AJOUTER SYSTEME DE TIMER

	//HandleInput(); // AJOUTER SYSTEME DE GESTION D'INPUT

	// GESTION DES INPUTS
	{
		// Mettez a jour la souris en passant le handle de la fenetre
		InputManager::UpdateMouse(GetActiveWindow());

		// Recuperer le deplacement de la souris
		int deltaX = InputManager::GetMouseDeltaX();
		int deltaY = InputManager::GetMouseDeltaY();

		// Sensibilite de la souris
		const float sensitivity = 0.005f;
		if (InputManager::GetKeyIsPressed(MK_LBUTTON))
		{
			// Mettre a jour la rotation de la camera en fonction du delta
			m_Camera.Rotate(-deltaY * sensitivity, deltaX * sensitivity);
		}

		/*if (InputManager::GetKeyDown(VK_LEFT)) m_Camera.Move(0, -0.1f, 0);
		if (InputManager::GetKeyDown(VK_RIGHT)) m_Camera.Move(0, 0.1f, 0);
		if (InputManager::GetKeyDown(VK_UP)) m_Camera.Move(0.1f, 0, 0);
		if (InputManager::GetKeyDown(VK_DOWN)) m_Camera.Move(-0.1, 0, 0); */

		if (InputManager::GetKeyIsPressed(VK_LEFT)) m_Camera.MoveRelative(0.0f, -0.1f, 0.0f);
		if (InputManager::GetKeyIsPressed(VK_RIGHT)) m_Camera.MoveRelative(0.0f, 0.1f, 0.0f);
		if (InputManager::GetKeyIsPressed(VK_UP)) m_Camera.MoveRelative(0.1f, 0.0f, 0.0f);
		if (InputManager::GetKeyIsPressed(VK_DOWN)) m_Camera.MoveRelative(-0.1f, 0.0f, 0.0f);

		/*if (InputManager::GetKeyDown('K')) m_Camera.Rotate(0, -0.01f);
		if (InputManager::GetKeyDown('M')) m_Camera.Rotate(0, 0.01f);
		if (InputManager::GetKeyDown('L')) m_Camera.Rotate(-0.01f, 0);
		if (InputManager::GetKeyDown('O')) m_Camera.Rotate(0.01f, 0); */
	}

	// UPDATE DU JEU
	UpdatePhysics();
}

void InitDirect3DApp::Render()
{
	// Configure le root signature et le pipeline
	mCommandList->SetGraphicsRootSignature(mRootSignature.Get());
	mCommandList->SetPipelineState(mPipelineState.Get());

	// Definit les vertex et index buffers communs
	mCommandList->IASetVertexBuffers(0, 1, &mVertexBufferView);
	mCommandList->IASetIndexBuffer(&mIndexBufferView);
	mCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Si il ya des entitees
	if (m_entityManager->GetNbEntity() > 0)
	{
		// Mes a jour le constant buffer et dessiner chaque cube
		for (auto* entity : m_entityManager->GetEntityTab())
		{
			if (entity == nullptr) 
			{
				break;
			}

			MeshComponent* entityMesh = nullptr;
			TransformComponent* entityTransform = nullptr;

			if (entity->id < 0)
			{
				entityMesh = static_cast<MeshComponent*>(m_entityManager->GetComponentToAddTab()[entity->tab_index]->tab_components[Mesh_index]);
				entityTransform = static_cast<TransformComponent*>(m_entityManager->GetComponentToAddTab()[entity->tab_index]->tab_components[Transform_index]);
			}
			if (entity->id > 0)
			{
				entityMesh = static_cast<MeshComponent*>(m_entityManager->GetComponentsTab()[entity->tab_index]->tab_components[Mesh_index]);
				entityTransform = static_cast<TransformComponent*>(m_entityManager->GetComponentsTab()[entity->tab_index]->tab_components[Transform_index]);
			}

			// Calculer la matrice World-View-Projection
			DirectX::XMMATRIX world = XMLoadFloat4x4(&entityTransform->m_transform.GetMatrix());
			DirectX::XMMATRIX view = m_Camera.GetViewMatrix();
			DirectX::XMMATRIX proj = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, 1.0f, 1.0f, 1000.0f);
			DirectX::XMMATRIX wvp = world * view * proj;

			TransformConstants objConstants;
			XMStoreFloat4x4(&objConstants.WorldViewProj, DirectX::XMMatrixTranspose(wvp));

			// Mettre a jour le constant buffer du cube
			void* pData;
			CD3DX12_RANGE readRange(0, 0);
			entityMesh->m_cubeMesh->m_constantBuffer.Get()->Map(0, &readRange, &pData);
			memcpy(pData, &objConstants, sizeof(objConstants));
			entityMesh->m_cubeMesh->m_constantBuffer.Get()->Unmap(0, nullptr);

			// Attacher le constant buffer à la racine (slot 0)
			mCommandList->SetGraphicsRootConstantBufferView(0, entityMesh->m_cubeMesh->m_constantBuffer.Get()->GetGPUVirtualAddress());

			// Dessiner le cube (36 indices)
			mCommandList->DrawIndexedInstanced(entityMesh->m_cubeMesh->m_meshIndex, 1, 0, 0, 0);
		}
	}

}

void InitDirect3DApp::CreatePipelineState()
{
	// Definition d'un parametre root pour un Constant Buffer (CBV)
	CD3DX12_ROOT_PARAMETER slotRootParameter[1];
	slotRootParameter[0].InitAsConstantBufferView(0);

	// Creation de la Root Signature
	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(1, slotRootParameter, 0, nullptr,D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> serializedRootSig = nullptr;
	ComPtr<ID3DBlob> error = nullptr;
	HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1,&serializedRootSig, &error);
	if (FAILED(hr))
	{
		if (error) MessageBoxA(0, (char*)error->GetBufferPointer(), "RootSignature Error", MB_OK);
		return;
	}

	hr = mD3DDevice->CreateRootSignature(0, serializedRootSig->GetBufferPointer(),
		serializedRootSig->GetBufferSize(), IID_PPV_ARGS(&mRootSignature));
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
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	// Create the pipeline state object
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.InputLayout = { inputLayout, _countof(inputLayout) };
	psoDesc.pRootSignature = mRootSignature.Get();
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
	psoDesc.DepthStencilState = m_depthStencilDesc; // Utilisation de la valeur stockee

	hr = mD3DDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&mPipelineState));
	if (FAILED(hr))
	{
		MessageBox(0, L"CreateGraphicsPipelineState failed.", L"Error", MB_OK);
		return;
	}
}

void InitDirect3DApp::UpdatePhysics()
{

	if (m_entityManager->GetEntityTab()[0] != nullptr)
	{
		// Update
		mScene->OnUpdate();

		// update transform of entites


		// COLLISIONS a ajouter

	}


	// DESTROY ENTITIES
	for (auto& entityToDestroy : m_entityManager->GetToDestroyTab())
	{
		m_entityManager->DestroyEntity(entityToDestroy);
	}
	m_entityManager->GetToDestroyTab().clear();

	// ADD ENTITIES
	for (auto& entityToAdd : m_entityManager->GetEntityToAddTab())
	{
		m_entityManager->AddEntityToTab(entityToAdd, m_entityManager->GetComponentToAddTab()[entityToAdd->tab_index]);
	}
	m_entityManager->GetEntityToAddTab().clear();
	m_entityManager->GetComponentToAddTab().clear();
	m_entityManager->ResetEntitiesToAdd();
}

void InitDirect3DApp::Draw()
{
	// Reinitialise le command allocator et la command list
	HRESULT hr = mCommandAllocator->Reset();
	if (FAILED(hr))
	{
		MessageBox(0, L"Erreur lors du Reset du Command Allocator.", 0, 0);
		return;
	}
	hr = mCommandList->Reset(mCommandAllocator.Get(), nullptr); // rajouter le pipeline state
	if (FAILED(hr))
	{
		MessageBox(0, L"Erreur lors du Reset de la Command List.", 0, 0);
		return;
	}

	// Definir le viewport et le rectangle de decoupe (scissor) pour le rendu.
	mCommandList->RSSetViewports(1, &mScreenViewport);
	mCommandList->RSSetScissorRects(1, &mScissorRect);

	// Recuperer le handle du back buffer pour le rendu.
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(mRtvHeap->GetCPUDescriptorHandleForHeapStart(), mCurrBackBuffer, mRtvDescriptorSize);

	// Recuperer le handle du Depth Stencil View.
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(mDsvHeap->GetCPUDescriptorHandleForHeapStart());

	// Transitionner le back buffer de l'etat PRESENT a RENDER_TARGET.
	CD3DX12_RESOURCE_BARRIER barrierStart = CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	mCommandList->ResourceBarrier(1, &barrierStart);

	// Effacer le Render Target avec une couleur de fond
	FLOAT clearColor[] = { 1.0f, 0.0f, 1.0f, 1.0f };
	mCommandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

	// Effacer le Depth Buffer pour reinitialiser les valeurs de profondeur (1.0 = loin).
	mCommandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	// Attacher le Render Target et le Depth Buffer a l'Output Merger.
	mCommandList->OMSetRenderTargets(1, &rtvHandle, true, &dsvHandle);

	// Appeler le renderer des objets
	Render();

	// Change le titre de la fentre (peux servir pour le debug)
	wchar_t title[256];
	//swprintf_s(title, 256, L"NBcube: %d", (int)m_meshFactory->GetCubeList()->size());
	//SetWindowText(GetActiveWindow(), title);

	// Transitionner le back buffer de RENDER_TARGET a PRESENT pour la presentation.
	CD3DX12_RESOURCE_BARRIER barrierStop = CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	mCommandList->ResourceBarrier(1, &barrierStop);

	// Fermer la command list et l'executer sur la GPU.
	mCommandList->Close();
	ID3D12CommandList* ppCommandLists[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	// Attendre la fin de l'execution des commandes et presenter le swap chain.
	FlushCommandQueue();
	mSwapChain->Present(0, 0);
	mCurrBackBuffer = (mCurrBackBuffer + 1) % SwapChainBufferCount;
}