#include "pch.h"

#include "InitDirect3DApp.h"
#include <d3dcompiler.h>

#include "MeshFactory.h"
#include "InputManager.h"
#include "Scene.h"
#include "EntityManager.h"
#include "SceneTest.h"
#include "TextureLoaderDuLivre.h"

InitDirect3DApp::InitDirect3DApp(HINSTANCE hInstance) : WindowDX(hInstance)
{
}

bool InitDirect3DApp::Initialize()
{
	if (!WindowDX::Initialize())
		return false;

	FlushCommandQueue();

	mCurrentFence = 0; // Initialisation de la valeur du fence

	HRESULT hr = mD3DDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mFence));
	if (FAILED(hr))
	{
		MessageBox(0, L"echec de la creation du Fence.", L"Erreur", MB_OK);
		return false;
	}

	// Init les frame Ressources
	for (int i = 0; i < gNumFrameResources; i++)
	{
		auto frameResource = std::make_unique<FrameResource>();
		mD3DDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&frameResource->CmdListAlloc));
		frameResource->Fence = 0;
		mFrameResources.push_back(std::move(frameResource));
	}

	mCurrFrameResource = mFrameResources[0].get();

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

	// RESET A ajouter
	mCurrFrameResource->CmdListAlloc->Reset();
	mCommandList->Reset(mCurrFrameResource->CmdListAlloc.Get(), nullptr);

	// CHARGEMENT TEXTURE :
	// Charger la texture depuis un fichier DDS
	hr = DirectX::CreateDDSTextureFromFile12(
		mD3DDevice.Get(),
		mCommandList.Get(),
		L"../../../src/MoteurDX/tile.dds",
		m_Texture,
		m_TextureUploadHeap);
	if (FAILED(hr))
	{
		MessageBox(0, L"echec du chargement de la texture !", L"Error", MB_OK);
		//return;
	}

	/*CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_Texture.Get(),
		D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	mCommandList->ResourceBarrier(1, &barrier);*/

	// Creer un descriptor heap pour le SRV (1 descripteur suffira) 
	// Le descriptor heap est une zone mémoire réservée sur la carte graphique qui stocke plusieurs descripteurs. Dans le cas du SRV, il contient les 
	// descriptions de ressources (comme les textures) afin que le GPU puisse y accéder directement lors du rendu. Une fois créé et rempli, le heap permet 
	// de regrouper et de gérer ces descripteurs de façon efficace et de les rendre visibles aux shaders.
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.NumDescriptors = 1;
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	hr = mD3DDevice->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&m_SrvHeap));
	if (FAILED(hr))
	{
		MessageBox(0, L"Failed to create SRV Heap!", L"Error", MB_OK);
		//return;
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
	mD3DDevice->CreateShaderResourceView(m_Texture.Get(), &srvDesc, m_SrvHeap->GetCPUDescriptorHandleForHeapStart());


	// Set primitive topology (invariant pour tous les cubes)
	// mCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Positionner la camera a une position initiale
	m_Camera.SetPosition(0.0f, 0.0f, -5.0f); // Place la camera en arriere pour voir la scene

	m_depthStencilDesc = {};
	m_depthStencilDesc.DepthEnable = TRUE;
	m_depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	m_depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	m_depthStencilDesc.StencilEnable = FALSE;

	//MessageBox(0, L"CreationDuCube", 0, 0);

	mCommandList->Close();
	ID3D12CommandList* cmdLists[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(cmdLists), cmdLists);
	FlushCommandQueue();

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

		/*if (InputManager::GetKeyIsPressed(VK_LEFT)) m_Camera.MoveRelative(0.0f, -0.1f, 0.0f);
		if (InputManager::GetKeyIsPressed(VK_RIGHT)) m_Camera.MoveRelative(0.0f, 0.1f, 0.0f);
		if (InputManager::GetKeyIsPressed(VK_UP)) m_Camera.MoveRelative(0.1f, 0.0f, 0.0f);
		if (InputManager::GetKeyIsPressed(VK_DOWN)) m_Camera.MoveRelative(-0.1f, 0.0f, 0.0f);*/
		if (InputManager::GetKeyIsPressed('Q')) m_Camera.MoveRelative(0.0f, -0.1f, 0.0f);
		if (InputManager::GetKeyIsPressed('D')) m_Camera.MoveRelative(0.0f, 0.1f, 0.0f);
		if (InputManager::GetKeyIsPressed('Z')) m_Camera.MoveRelative(0.1f, 0.0f, 0.0f);
		if (InputManager::GetKeyIsPressed('S')) m_Camera.MoveRelative(-0.1f, 0.0f, 0.0f);
		if (InputManager::GetKeyIsPressed('A')) m_Camera.MoveRelative(0.0f, 0.0f, 0.1f);
		if (InputManager::GetKeyIsPressed('E')) m_Camera.MoveRelative(0.0f, 0.0f, -0.1f);
	}

	// UPDATE DU JEU
	UpdatePhysics();
}

void InitDirect3DApp::Render()
{
	// Si il ya des entitees
	if (m_entityManager->GetNbEntity() > 0)
	{
		// Configure le root signature et le pipeline
		mCommandList->SetGraphicsRootSignature(mRootSignature.Get());
		mCommandList->SetPipelineState(mPipelineState.Get());

		// Lier le descriptor heap contenant le SRV de la texture
		ID3D12DescriptorHeap* descriptorHeaps[] = { m_SrvHeap.Get() };
		mCommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);
		// Le slot 1 de la root signature (table de descripteurs) pointe sur notre texture
		mCommandList->SetGraphicsRootDescriptorTable(1, m_SrvHeap->GetGPUDescriptorHandleForHeapStart());

		// Definit les vertex et index buffers communs
		mCommandList->IASetVertexBuffers(0, 1,m_meshFactory->GetVertexBufferView());
		mCommandList->IASetIndexBuffer(m_meshFactory->GetIndexBufferView());
		mCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		DirectX::XMMATRIX view = m_Camera.GetViewMatrix();
		DirectX::XMMATRIX proj = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, 1.0f, 1.0f, 1000.0f);

		// Mes a jour le constant buffer et dessiner chaque cube
		for (auto* entity : m_entityManager->GetEntityTab())
		{
			if (entity == nullptr) 
			{
				continue;
			}

			MeshComponent* entityMesh = nullptr;
			TransformComponent* entityTransform = nullptr;

			for (auto& component : m_entityManager->GetComponentsTab()[entity->tab_index]->vec_components)
			{
				if (!entityMesh && component->ID == Mesh_ID)
				{
					entityMesh = static_cast<MeshComponent*>(component);
				}
				if (!entityTransform && component->ID == Transform_ID)
				{
					entityTransform = static_cast<TransformComponent*>(component);
				}
				if (entityMesh && entityTransform)
				{
					break; // On arrive la boucle d qu'on a trouve les deux composants
				}
			}
			if (!entityMesh || !entityTransform)
			{
				// Gérer l'erreur (afficher un message, ignorer le rendu, etc.)
				return;
			}
			/*entityMesh = static_cast<MeshComponent*>(m_entityManager->GetComponentsTab()[entity->tab_index]->tab_components[Mesh_index]);
			entityTransform = static_cast<TransformComponent*>(m_entityManager->GetComponentsTab()[entity->tab_index]->tab_components[Transform_index]);*/

			// Calculer la matrice World-View-Projection
			DirectX::XMMATRIX world = XMLoadFloat4x4(&entityTransform->m_transform.GetMatrix());
			DirectX::XMMATRIX wvp = world * view * proj;
			TransformConstants objConstants;
			XMStoreFloat4x4(&objConstants.WorldViewProj, DirectX::XMMatrixTranspose(wvp));

			// Mise a jour du constant buffer via le mapping persistant (m_mappedData)
			memcpy(entityMesh->m_cubeMesh->m_mappedData, &objConstants, sizeof(objConstants));

			// Attacher le constant buffer a la racine (slot 0)
			mCommandList->SetGraphicsRootConstantBufferView(0, entityMesh->m_cubeMesh->m_constantBuffer->GetGPUVirtualAddress());

			// Dessiner le cube (36 indices)
			mCommandList->DrawIndexedInstanced(entityMesh->m_cubeMesh->m_meshIndex, 1, 0, 0, 0);

		}
	}
}

void InitDirect3DApp::CreatePipelineState()
{
	// Definition d'un parametre root pour un Constant Buffer (CBV)
	/*CD3DX12_ROOT_PARAMETER slotRootParameter[1];
	slotRootParameter[0].InitAsConstantBufferView(0);*/

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
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;

	// Creation de la Root Signature
	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(2, slotRootParameter, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	// Creation de la Root Signature
	// CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(1, slotRootParameter, 0, nullptr,D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> serializedRootSig = nullptr;
	ComPtr<ID3DBlob> error = nullptr;
	HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1,&serializedRootSig, &error);
	if (FAILED(hr))
	{
		if (error) MessageBoxA(0, (char*)error->GetBufferPointer(), "RootSignature Error", MB_OK);
		return;
	}

	hr = mD3DDevice->CreateRootSignature(0, serializedRootSig->GetBufferPointer(),serializedRootSig->GetBufferSize(), IID_PPV_ARGS(&mRootSignature));
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
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
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
	psoDesc.DepthStencilState = m_depthStencilDesc;
	//psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;

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

	if (m_entityManager->GetNbEntity() > 0 && m_entityManager->GetEntityTab()[0] != nullptr)
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

	mCurrFrameIndex = (mCurrFrameIndex + 1) % gNumFrameResources;
	mCurrFrameResource = mFrameResources[mCurrFrameIndex].get();

	DWORD t = timeGetTime();

	mCurrFrameResource->CmdListAlloc->Reset();
	mCommandList->Reset(mCurrFrameResource->CmdListAlloc.Get(), nullptr);

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

	// Transitionner le back buffer de RENDER_TARGET a PRESENT pour la presentation.
	CD3DX12_RESOURCE_BARRIER barrierStop = CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	mCommandList->ResourceBarrier(1, &barrierStop);

	// Fermer la command list et l'executer sur la GPU.
	mCommandList->Close();
	ID3D12CommandList* ppCommandLists[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	// Soumettre les commandes au GPU
	//mCommandList->Close();
	//ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
	//mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	// FlushCommandQueue();

	mSwapChain->Present(0, 0);
	mCurrBackBuffer = (mCurrBackBuffer + 1) % SwapChainBufferCount;

	// Advance the fence value to mark commands up to this fence point.
	mCurrFrameResource->Fence = ++mCurrentFence;

	// Add an instruction to the command queue to set a new fence point. 
	// Because we are on the GPU timeline, the new fence point won't be 
	// set until the GPU finishes processing all the commands prior to this Signal().
	mCommandQueue->Signal(mFence.Get(), mCurrentFence);

	// Attendre que la frame precedente soit terminee
	if (mFence->GetCompletedValue() < mCurrFrameResource->Fence)
	{
		HANDLE eventHandle = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		mFence->SetEventOnCompletion(mCurrFrameResource->Fence, eventHandle);
		WaitForSingleObject(eventHandle, INFINITE);
		CloseHandle(eventHandle);
	}

	DWORD dt = timeGetTime() - t;
	wchar_t title[256];
	swprintf_s(title, 256, L"lag meters: %d", (int)dt);
	SetWindowText(GetActiveWindow(), title);
}