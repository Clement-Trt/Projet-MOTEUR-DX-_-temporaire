#include "pch.h"

#include "InitDirect3DApp.h"

#include "MeshFactory.h"
#include "InputManager.h"
#include "EntityManager.h"
#include "HealthSystem.h"
#include "AttackSystem.h"
#include "LightSystem.h"

#include "Scene.h"
#include "SceneTest.h"
#include "GameScene.h"

#include "TextureManager.h"
#include "CameraSystem.h"
#include "ColliderManager.h"
#include "ParticleManager.h"
#include "EnnemyManager.h"
#include "MovementManager.h"
#include "LifeTimeManager.h"

InitDirect3DApp::InitDirect3DApp(HINSTANCE hInstance) : WindowDX(hInstance)
{
	m_lastTime = timeGetTime();
}

InitDirect3DApp::~InitDirect3DApp()
{
	delete mp_healthSystem;
	delete mp_attackSystem;
	delete mp_meshFactory;
	delete mp_entityManager;
	delete mp_textureManager;
	delete mp_colliderManager;
	delete mp_particleManager;
	delete mp_ennemyManager;
	delete mp_movementManager;
	delete mp_lifeTimeManager;
	delete mp_cameraManager;
	delete mp_scene;
}

bool InitDirect3DApp::Initialize()
{
	if (!WindowDX::Initialize())
		return false;

	FlushCommandQueue();

	m_currentFence = 0;

	HRESULT hr = m_d3DDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence));
	if (FAILED(hr))
	{
		MessageBox(0, L"echec de la creation du Fence.", L"Erreur", MB_OK);
		return false;
	}

	// Init les frame Ressources
	for (int i = 0; i < gNum_frameResources; i++)
	{
		auto frameResource = std::make_unique<FrameResource>();
		m_d3DDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&frameResource->m_cmdListAlloc));
		frameResource->m_fence = 0;
		m_frameResources.push_back(std::move(frameResource));
	}

	m_currFrameResource = m_frameResources[0].get();


	m_depthStencilDesc = {};
	m_depthStencilDesc.DepthEnable = TRUE;
	m_depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	m_depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	m_depthStencilDesc.StencilEnable = FALSE;

	// Cree le pipeline(root signature & PSO)
	CreatePipelineState();

	// Reinitialiser le command allocator et la command list
	m_currFrameResource->m_cmdListAlloc->Reset();
	m_commandList->Reset(m_currFrameResource->m_cmdListAlloc.Get(), nullptr);

	if (!InitTexture())
	{
		MessageBox(0, L"echec du chargement de la texture !", L"Erreur", MB_OK);
		return false;
	}


	m_commandList->Close();
	ID3D12CommandList* cmdLists[] = { m_commandList.Get() };
	m_commandQueue->ExecuteCommandLists(_countof(cmdLists), cmdLists);
	FlushCommandQueue();

	// Initialisation Game Manager et Scene (ECS)
	mp_entityManager = new EntityManager();

	// MeshFactory
	mp_meshFactory = new MeshFactory;
	mp_meshFactory->Initialize(m_d3DDevice.Get(), mp_entityManager, this);

	// Particles
	mp_particleManager = new ParticleManager;
	mp_particleManager->Initialize(this);

	// Collider
	mp_colliderManager = new ColliderManager;
	mp_colliderManager->Initialize(mp_entityManager, mp_particleManager);

	// Ennemy
	mp_ennemyManager = new EnnemyManager;
	mp_ennemyManager->Initialize(this);

	// AttackSystem
	mp_attackSystem = new AttackSystem;
	mp_attackSystem->Initialize(this);

	// LightSystem
	mp_lightSystem = new LightSystem;
	mp_lightSystem->Initialize(this);

	// HealthManager
	mp_healthSystem = new HealthSystem;
	mp_healthSystem->Initialize(this);

	// MouvementManager
	mp_movementManager = new MovementManager;
	mp_movementManager->Initialize(this);

	// CameraManager
	mp_cameraManager = new CameraSystem;
	mp_cameraManager->Initialize(this);

	// LifeTimeManager
	mp_lifeTimeManager = new LifeTimeManager;
	mp_lifeTimeManager->Initialize(this);

	// Scene
	GameScene* scene = new GameScene;
	SetScene(scene);
	mp_scene->Initialize(this);
	mp_scene->OnInitialize();

	// Pour la lumiere :
	// Calculez la taille necessaire pour le constant buffer
	UINT passCBSize = (sizeof(PassConstants) + 255) & ~255;

	// Creez un resource upload buffer pour le constant buffer
	CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
	CD3DX12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(passCBSize);

	hr = m_d3DDevice->CreateCommittedResource(
		&heapProps,
		D3D12_HEAP_FLAG_NONE,
		&bufferDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_passConstantBuffer)
	);
	if (FAILED(hr))
	{
		// Gestion d'erreur
	}

	// Mappez le constant buffer et obtenez un pointeur qui y est associe
	hr = m_passConstantBuffer->Map(0, nullptr, reinterpret_cast<void**>(&mp_mappedPassCB));
	if (FAILED(hr))
	{
		// Gestion d'erreur
	}

	// Optionnel : initialiser le buffer a zero
	memset(mp_mappedPassCB, 0, passCBSize);

	m_gameIsPaused = true;
	return true;
}

bool InitDirect3DApp::InitTexture()
{
	// Creation du TextureManager
	mp_textureManager = new TextureManager(m_d3DDevice.Get(), m_commandList.Get());
	// On cree un heap pour le nombre total de textures
	mp_textureManager->CreateDescriptorHeap(11);

	// Chargement des textures en appelant LoadTexture pour chaque ressource
	if (!mp_textureManager->LoadTexture(L"PlayerTexture", L"../../../src/MoteurDX/tile.dds"))
	{
		MessageBox(0, L"echec du chargement de la texture Player.", L"Erreur", MB_OK);
		return false;
	}
	if (!mp_textureManager->LoadTexture(L"WallTexture", L"../../../src/MoteurDX/stone.dds"))
	{
		MessageBox(0, L"echec du chargement de la texture Wall.", L"Erreur", MB_OK);
		return false;
	}
	if (!mp_textureManager->LoadTexture(L"BoxTexture", L"../../../src/MoteurDX/WoodCrate.dds"))
	{
		MessageBox(0, L"echec du chargement de la texture Box.", L"Erreur", MB_OK);
		return false;
	}
	if (!mp_textureManager->LoadTexture(L"IceTexture", L"../../../src/MoteurDX/ice.dds"))
	{
		MessageBox(0, L"echec du chargement de la texture Ice.", L"Erreur", MB_OK);
		return false;
	}
	if (!mp_textureManager->LoadTexture(L"FireTexture", L"../../../src/MoteurDX/fire.dds"))
	{
		MessageBox(0, L"echec du chargement de la texture Fire.", L"Erreur", MB_OK);
		return false;
	}
	if (!mp_textureManager->LoadTexture(L"DroneTexture", L"../../../src/MoteurDX/Drone.dds"))
	{
		MessageBox(0, L"echec du chargement de la texture Ice.", L"Erreur", MB_OK);
		return false;
	}
	if (!mp_textureManager->LoadTexture(L"SkyBox", L"../../../src/MoteurDX/SkyBoxTexture.dds"))
	{
		MessageBox(0, L"echec du chargement de la texture Ice.", L"Erreur", MB_OK);
		return false;
	}
	if (!mp_textureManager->LoadTexture(L"SkyBox2", L"../../../src/MoteurDX/SkyBox2.dds"))
	{
		MessageBox(0, L"echec du chargement de la texture SkyBox2.", L"Erreur", MB_OK);
		return false;
	}
	if (!mp_textureManager->LoadTexture(L"DefaultTexture", L"../../../src/MoteurDX/defaultTexture.dds"))
	{
		MessageBox(0, L"echec du chargement de la texture Ice.", L"Erreur", MB_OK);
		return false;
	}
	if (!mp_textureManager->LoadTexture(L"BlueBeamTexture", L"../../../src/MoteurDX/blueBeam.dds"))
	{
		MessageBox(0, L"echec du chargement de la texture Ice.", L"Erreur", MB_OK);
		return false;
	}
	if (!mp_textureManager->LoadTexture(L"RedBeamTexture", L"../../../src/MoteurDX/redBeam.dds"))
	{
		MessageBox(0, L"echec du chargement de la texture Ice.", L"Erreur", MB_OK);
		return false;
	}

	return true;
}

void InitDirect3DApp::Update()
{
	if (m_gameIsPaused == true)
	{
		if (InputManager::GetKeyDown(VK_ESCAPE))
		{
			m_gameIsPaused = false;
			//ShowCursor(FALSE);
			InputManager::SetCursorLockedAndInvisible(true);
			InputManager::SetCursorOnTheCenterOfTheScreen(GetActiveWindow());
		}
	}
	else
	{
		SetCursor(LoadCursor(NULL, IDC_CROSS));
		if (InputManager::GetKeyDown(VK_ESCAPE))
		{
			m_gameIsPaused = true;
			// ShowCursor(TRUE);
			InputManager::SetCursorLockedAndInvisible(false);
		}

		// UPDATE DU JEU
		UpdateTimer();
		UpdatePhysics();
	}
}

void InitDirect3DApp::UpdateTimer()
{
	DWORD currentTime = timeGetTime();
	m_deltaTime = (currentTime - m_lastTime) / 1000.0f; // conversion en secondes
	m_lastTime = currentTime;
}

void InitDirect3DApp::UpdatePhysics()
{
	// UPDATE SCENE
	if (mp_entityManager->GetNbEntity() > 0 && mp_entityManager->GetEntityTab()[0] != nullptr)
	{
		// Update (En gros gestion des input)
		mp_scene->OnUpdate();

		// Ennemies
		mp_ennemyManager->Update();

		// MovementSystem
		mp_movementManager->Update(); 

		// CollisionsSystem
		mp_colliderManager->Update();

		// cameraSystem
		mp_cameraManager->Update(); 

		// attacksystem
		mp_attackSystem->Update(m_deltaTime);

		// HealthSystem
		mp_healthSystem->Update(m_deltaTime); 

		// LightSystem
		mp_lightSystem->Update(m_deltaTime);
		
		// LifeTimeSystem
		mp_lifeTimeManager->Update(m_deltaTime);
	}

	// DESTROY ENTITIES
	for (auto& entityToDestroy : mp_entityManager->GetToDestroyTab())
	{
		mp_entityManager->DestroyEntity(entityToDestroy);
	}
	mp_entityManager->GetToDestroyTab().clear();

	// ADD ENTITIES
	for (auto& entityToAdd : mp_entityManager->GetEntityToAddTab())
	{
		mp_entityManager->AddEntityToTab(entityToAdd, mp_entityManager->GetComponentToAddTab()[entityToAdd->tab_index]);
	}
	mp_entityManager->GetEntityToAddTab().clear();
	mp_entityManager->GetComponentToAddTab().clear();
	mp_entityManager->ResetEntitiesToAdd();
}


void InitDirect3DApp::Render()
{
	// Si il ya des entitees
	if (mp_entityManager->GetNbEntity() > 0)
	{
		// Configure le root signature et le pipeline
		m_commandList->SetGraphicsRootSignature(m_rootSignature.Get());
		m_commandList->SetPipelineState(m_pipelineState.Get());

		// On doit lier le heap de textures (celui du TextureManager)
		ID3D12DescriptorHeap* descriptorHeaps[] = { mp_textureManager->GetSrvHeap() };
		m_commandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

		m_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		PassConstants passConstants = mp_lightSystem->GetPassConstants();
		memcpy(mp_mappedPassCB, &passConstants, sizeof(PassConstants));
		m_commandList->SetGraphicsRootConstantBufferView(2, m_passConstantBuffer->GetGPUVirtualAddress());

		DirectX::XMMATRIX view = mp_cameraManager->GetViewMatrix();
		DirectX::XMMATRIX proj = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, 1.0f, 1.0f, 1000.0f);

		// Mes a jour le constant buffer et dessiner chaque cube
		for (auto* entity : mp_entityManager->GetEntityTab())
		{
			// Check si l'entity dans la table est null
			if (entity == nullptr)
			{
				continue;
			}

			MeshComponent* entityMesh = nullptr;
			TransformComponent* entityTransform = nullptr;

			for (auto& component : mp_entityManager->GetComponentsTab()[entity->tab_index]->vec_components)
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
					break; // On arrive la boucle des qu'on a trouve les deux composants
				}
			}
			if (!entityMesh || !entityTransform)
			{
				continue;
			}

			// Calculer la matrice World-View-Projection
			DirectX::XMMATRIX world = XMLoadFloat4x4(&entityTransform->m_transform.GetMatrix());
			DirectX::XMMATRIX wvp = world * view * proj;
			TransformConstants objConstants;
			XMStoreFloat4x4(&objConstants.World, DirectX::XMMatrixTranspose(world));
			XMStoreFloat4x4(&objConstants.WorldViewProj, DirectX::XMMatrixTranspose(wvp));

			// Mise a jour du constant buffer via le mapping persistant (m_mappedData)
			memcpy(entityMesh->m_cubeMesh->m_mappedData, &objConstants, sizeof(objConstants));

			// Attacher le constant buffer a la racine (slot 0)
			m_commandList->SetGraphicsRootConstantBufferView(0, entityMesh->m_cubeMesh->m_constantBuffer->GetGPUVirtualAddress());
			m_commandList->IASetVertexBuffers(0, 1, &entityMesh->m_cubeMesh->m_geometryMesh.m_vertexBufferView);
			m_commandList->IASetIndexBuffer(&entityMesh->m_cubeMesh->m_geometryMesh.m_indexBufferView);

			if (entityMesh->m_textureID.empty())
			{
				// Si aucun ID n'est defini, utilisez par defaut "PlayerTexture"
				entityMesh->m_textureID = L"PlayerTexture";
			}
			// Recuperation du handle de la texture a utiliser selon l'identifiant contenu dans le MeshComponent
			D3D12_GPU_DESCRIPTOR_HANDLE textureHandle = mp_textureManager->GetTextureHandle(entityMesh->m_textureID);
			// Mise a jour du slot 1 de la root signature (table de descripteurs) pour pointer sur la texture
			m_commandList->SetGraphicsRootDescriptorTable(1, textureHandle);

			// Dessiner le cube (36 indices)
			m_commandList->DrawIndexedInstanced(entityMesh->m_cubeMesh->m_geometryMesh.m_meshIndex, 1, 0, 0, 0);
		}
	}
}

void InitDirect3DApp::CreatePipelineState()
{
	// Definition d'un parametre root pour un Constant Buffer (CBV)
	CD3DX12_ROOT_PARAMETER slotRootParameter[3];
	// Parametre 0 : constant buffer (transformation)
	slotRootParameter[0].InitAsConstantBufferView(0);
	// Parametre 1 : table de descripteurs pour la texture (register t0)
	CD3DX12_DESCRIPTOR_RANGE texTable;
	texTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
	slotRootParameter[1].InitAsDescriptorTable(1, &texTable, D3D12_SHADER_VISIBILITY_PIXEL);
	// Parametre 2 : Pour l'eclairage (PassConstants)
	slotRootParameter[2].InitAsConstantBufferView(1);

	// Declarez le static sampler pour register(s0) 
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc(0); // register s0
	samplerDesc.Filter = D3D12_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;

	// Creation de la Root Signature
	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(3, slotRootParameter, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> serializedRootSig = nullptr;
	ComPtr<ID3DBlob> error = nullptr;
	HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &serializedRootSig, &error);
	if (FAILED(hr))
	{
		if (error) MessageBoxA(0, (char*)error->GetBufferPointer(), "RootSignature Error", MB_OK);
		return;
	}

	hr = m_d3DDevice->CreateRootSignature(0, serializedRootSig->GetBufferPointer(), serializedRootSig->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature));
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
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 40, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	// Create the pipeline state object
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.InputLayout = { inputLayout, _countof(inputLayout) };
	psoDesc.pRootSignature = m_rootSignature.Get();
	psoDesc.VS = { vertexShader->GetBufferPointer(), vertexShader->GetBufferSize() };
	psoDesc.PS = { pixelShader->GetBufferPointer(), pixelShader->GetBufferSize() };
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.DepthStencilState = m_depthStencilDesc;
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	psoDesc.SampleDesc.Count = 1;
	psoDesc.SampleDesc.Quality = 0;
	psoDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT; 
	psoDesc.DepthStencilState = m_depthStencilDesc;

	hr = m_d3DDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineState));
	if (FAILED(hr))
	{
		MessageBox(0, L"CreateGraphicsPipelineState failed.", L"Error", MB_OK);
		return;
	}
}


void InitDirect3DApp::Draw()
{

	// Start des frames
	m_currFrameIndex = (m_currFrameIndex + 1) % gNum_frameResources;
	m_currFrameResource = m_frameResources[m_currFrameIndex].get();

	// Start du lag meter
	DWORD t = timeGetTime();

	// Reinitialise le command allocator et la command list
	m_currFrameResource->m_cmdListAlloc->Reset();
	m_commandList->Reset(m_currFrameResource->m_cmdListAlloc.Get(), nullptr);

	// Definir le viewport et le rectangle de decoupe (scissor) pour le rendu.
	m_commandList->RSSetViewports(1, &m_screenViewport);
	m_commandList->RSSetScissorRects(1, &m_scissorRect);

	// Recuperer le handle du back buffer pour le rendu.
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart(), m_currBackBuffer, m_rtvDescriptorSize);

	// Recuperer le handle du Depth Stencil View.
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(m_dsvHeap->GetCPUDescriptorHandleForHeapStart());

	// Transitionner le back buffer de l'etat PRESENT a RENDER_TARGET.
	CD3DX12_RESOURCE_BARRIER barrierStart = CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	m_commandList->ResourceBarrier(1, &barrierStart);

	// Effacer le Render Target avec une couleur de fond
	FLOAT clearColor[] = { 1.0f, 0.0f, 1.0f, 1.0f };
	m_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

	// Effacer le Depth Buffer pour reinitialiser les valeurs de profondeur (1.0 = loin).
	m_commandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	// Attacher le Render Target et le Depth Buffer a l'Output Merger.
	m_commandList->OMSetRenderTargets(1, &rtvHandle, true, &dsvHandle);

	// Appeler le renderer des objets
	Render();

	// Transitionner le back buffer de RENDER_TARGET a PRESENT pour la presentation.
	CD3DX12_RESOURCE_BARRIER barrierStop = CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	m_commandList->ResourceBarrier(1, &barrierStop);

	// Fermer la command list et l'executer sur la GPU.
	m_commandList->Close();
	ID3D12CommandList* ppCommandLists[] = { m_commandList.Get() };
	m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	// Soumettre les commandes au GPU
	m_swapChain->Present(0, 0);
	m_currBackBuffer = (m_currBackBuffer + 1) % SwapChainBufferCount;

	// Advance the fence value to mark commands up to this fence point.
	m_currFrameResource->m_fence = ++m_currentFence;

	// Add an instruction to the command queue to set a new fence point. 
	// Because we are on the GPU timeline, the new fence point won't be 
	// set until the GPU finishes processing all the commands prior to this Signal().
	m_commandQueue->Signal(m_fence.Get(), m_currentFence);

	// Attendre que la frame precedente soit terminee
	if (m_fence->GetCompletedValue() < m_currFrameResource->m_fence)
	{
		HANDLE eventHandle = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		m_fence->SetEventOnCompletion(m_currFrameResource->m_fence, eventHandle);
		WaitForSingleObject(eventHandle, INFINITE);
		CloseHandle(eventHandle);
	}

	DWORD dt = timeGetTime() - t;
}