#include "pch.h"

#include "InitDirect3DApp.h"

#include "MeshFactory.h"
#include "InputManager.h"
#include "EntityManager.h"

#include "Scene.h"
//#include "SceneTest.h"
#include "GameScene.h"

#include "TextureLoaderDuLivre.h"
#include "TextureManager.h"
#include "CameraSystem.h"
#include "ColliderManager.h"

InitDirect3DApp::InitDirect3DApp(HINSTANCE hInstance) : WindowDX(hInstance)
{
	m_lastTime = timeGetTime();
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
	m_meshFactory->InitMeshFactory(mD3DDevice.Get(), GetEntityManager(), this);
	MessageBox(0, L"InitReussiMeshFacto", 0, 0);

	// Collider
	m_colliderManager = new ColliderManager;
	m_colliderManager->InitCollider(GetEntityManager());

	m_depthStencilDesc = {};
	m_depthStencilDesc.DepthEnable = TRUE;
	m_depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	m_depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	m_depthStencilDesc.StencilEnable = FALSE;

	// Cree le pipeline(root signature & PSO)
	CreatePipelineState();

	// Reinitialiser le command allocator et la command list
	mCurrFrameResource->CmdListAlloc->Reset();
	mCommandList->Reset(mCurrFrameResource->CmdListAlloc.Get(), nullptr);

	if (!InitTexture())
	{
		MessageBox(0, L"echec du chargement de la texture !", L"Erreur", MB_OK);
		return false;
	}

	// Positionner la camera a une position initiale
	m_mainView = new CameraComponent;
	m_mainView->m_cameraView = CameraSystem::DefaultView();

	mCommandList->Close();
	ID3D12CommandList* cmdLists[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(cmdLists), cmdLists);
	FlushCommandQueue();

	// Scene
	//SceneTest* scene = new SceneTest;
	GameScene* scene = new GameScene;
	SetScene(scene);
	m_scene->Initialize(this);
	m_scene->OnInitialize();

	m_attackSystem = new AttackSystem;
	m_attackSystem->Initialize(this);

	return true;
}

bool InitDirect3DApp::InitTexture()
{
	// Creation du TextureManager
	m_textureManager = new TextureManager(mD3DDevice.Get(), mCommandList.Get());
	// On cree un heap pour le nombre total de textures
	m_textureManager->CreateDescriptorHeap(5);

	// Chargement des textures en appelant LoadTexture pour chaque ressource
	if (!m_textureManager->LoadTexture(L"PlayerTexture", L"../../../src/MoteurDX/tile.dds"))
	{
		MessageBox(0, L"echec du chargement de la texture Player.", L"Erreur", MB_OK);
		return false;
	}
	if (!m_textureManager->LoadTexture(L"WallTexture", L"../../../src/MoteurDX/stone.dds"))
	{
		MessageBox(0, L"echec du chargement de la texture Wall.", L"Erreur", MB_OK);
		return false;
	}
	if (!m_textureManager->LoadTexture(L"BoxTexture", L"../../../src/MoteurDX/WoodCrate.dds"))
	{
		MessageBox(0, L"echec du chargement de la texture Box.", L"Erreur", MB_OK);
		return false;
	}
	if (!m_textureManager->LoadTexture(L"IceTexture", L"../../../src/MoteurDX/ice.dds"))
	{
		MessageBox(0, L"echec du chargement de la texture Ice.", L"Erreur", MB_OK);
		return false;
	}
	if (!m_textureManager->LoadTexture(L"SkyBox", L"../../../src/MoteurDX/SkyBoxTexture.dds"))
	{
		MessageBox(0, L"echec du chargement de la texture Ice.", L"Erreur", MB_OK);
		return false;
	}

	return true;
}

void InitDirect3DApp::Update()
{
	//SetDeltaTime(m_deltaTime); // AJOUTER SYSTEME DE TIMER

	//HandleInput(); // AJOUTER SYSTEME DE GESTION D'INPUT

	// GESTION DES INPUTS
	//{
		//// Mettez a jour la souris en passant le handle de la fenetre
		//InputManager::UpdateMouse(GetActiveWindow());

		//// Recuperer le deplacement de la souris
		//int deltaX = InputManager::GetMouseDeltaX();
		//int deltaY = InputManager::GetMouseDeltaY();

		//// Sensibilite de la souris
		//const float sensitivity = 0.005f;
		//if (InputManager::GetKeyIsPressed(MK_LBUTTON))
		//{
		//	// Mettre a jour la rotation de la camera en fonction du delta
		//	m_Camera.Rotate(-deltaY * sensitivity, deltaX * sensitivity);
		//}

		///*if (InputManager::GetKeyIsPressed(VK_LEFT)) m_Camera.MoveRelative(0.0f, -0.1f, 0.0f);
		//if (InputManager::GetKeyIsPressed(VK_RIGHT)) m_Camera.MoveRelative(0.0f, 0.1f, 0.0f);
		//if (InputManager::GetKeyIsPressed(VK_UP)) m_Camera.MoveRelative(0.1f, 0.0f, 0.0f);
		//if (InputManager::GetKeyIsPressed(VK_DOWN)) m_Camera.MoveRelative(-0.1f, 0.0f, 0.0f);*/
		//if (InputManager::GetKeyIsPressed('Q')) m_Camera.MoveRelative(0.0f, -0.1f, 0.0f);
		//if (InputManager::GetKeyIsPressed('D')) m_Camera.MoveRelative(0.0f, 0.1f, 0.0f);
		//if (InputManager::GetKeyIsPressed('Z')) m_Camera.MoveRelative(0.1f, 0.0f, 0.0f);
		//if (InputManager::GetKeyIsPressed('S')) m_Camera.MoveRelative(-0.1f, 0.0f, 0.0f);
		//if (InputManager::GetKeyIsPressed('A')) m_Camera.MoveRelative(0.0f, 0.0f, 0.1f);
		//if (InputManager::GetKeyIsPressed('E')) m_Camera.MoveRelative(0.0f, 0.0f, -0.1f);
	//}

	// UPDATE DU JEU
	UpdateTimer();
	UpdatePhysics();

	m_healthSystem.Update(m_entityManager, m_deltaTime);
	m_attackSystem->Update(m_entityManager, m_deltaTime);
}

//AABB InitDirect3DApp::GetAABB(const Transform& transform)
//{
//	AABB box;
//	box.min.x = transform.vPosition.x - transform.vScale.x;
//	box.min.y = transform.vPosition.y - transform.vScale.y;
//	box.min.z = transform.vPosition.z - transform.vScale.z;
//
//	box.max.x = transform.vPosition.x + transform.vScale.x;
//	box.max.y = transform.vPosition.y + transform.vScale.y;
//	box.max.z = transform.vPosition.z + transform.vScale.z;
//
//	return box;
//}
//
//bool InitDirect3DApp::CheckCollision(const AABB& a, const AABB& b)
//{
//	if (a.max.x < b.min.x || a.min.x > b.max.x)
//		return false;
//	if (a.max.y < b.min.y || a.min.y > b.max.y)
//		return false;
//	if (a.max.z < b.min.z || a.min.z > b.max.z)
//		return false;
//	return true;
//
//}
//bool InitDirect3DApp::AABBIntersect(const TransformComponent& a, const TransformComponent& b)
//{
//	// Calcul des bornes pour l'objet A
//	float aMinX = a.m_transform.vPosition.x - a.m_transform.vScale.x;
//	float aMaxX = a.m_transform.vPosition.x + a.m_transform.vScale.x;
//	float aMinY = a.m_transform.vPosition.y - a.m_transform.vScale.y;
//	float aMaxY = a.m_transform.vPosition.y + a.m_transform.vScale.y;
//	float aMinZ = a.m_transform.vPosition.z - a.m_transform.vScale.z;
//	float aMaxZ = a.m_transform.vPosition.z + a.m_transform.vScale.z;
//
//	// Calcul des bornes pour l'objet B
//	float bMinX = b.m_transform.vPosition.x - b.m_transform.vScale.x;
//	float bMaxX = b.m_transform.vPosition.x + b.m_transform.vScale.x;
//	float bMinY = b.m_transform.vPosition.y - b.m_transform.vScale.y;
//	float bMaxY = b.m_transform.vPosition.y + b.m_transform.vScale.y;
//	float bMinZ = b.m_transform.vPosition.z - b.m_transform.vScale.z;
//	float bMaxZ = b.m_transform.vPosition.z + b.m_transform.vScale.z;
//
//	// Verification du chevauchement sur chaque axe
//	if (aMaxX < bMinX || aMinX > bMaxX)
//		return false;
//	if (aMaxY < bMinY || aMinY > bMaxY)
//		return false;
//	if (aMaxZ < bMinZ || aMinZ > bMaxZ)
//		return false;
//
//	return true;
//}

void InitDirect3DApp::Render()
{
	// Si il ya des entitees
	if (m_entityManager->GetNbEntity() > 0)
	{
		// Configure le root signature et le pipeline
		mCommandList->SetGraphicsRootSignature(mRootSignature.Get());
		mCommandList->SetPipelineState(mPipelineState.Get());

		// On doit lier le heap de textures (celui du TextureManager)
		ID3D12DescriptorHeap* descriptorHeaps[] = { m_textureManager->GetSrvHeap() };
		mCommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

		// Definit les vertex et index buffers communs
		/*mCommandList->IASetVertexBuffers(0, 1,m_meshFactory->GetVertexBufferView());
		mCommandList->IASetIndexBuffer(m_meshFactory->GetIndexBufferView());*/
		mCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		DirectX::XMMATRIX view = m_mainView->m_cameraView;
		DirectX::XMMATRIX proj = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, 1.0f, 1.0f, 1000.0f);

		// Mes a jour le constant buffer et dessiner chaque cube
		for (auto* entity : m_entityManager->GetEntityTab())
		{
			// Check si l'entity dans la table est null
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
				// Gerer l'erreur (afficher un message, ignorer le rendu, etc.)
				continue;
				// return;
			}

			// Calculer la matrice World-View-Projection
			DirectX::XMMATRIX world = XMLoadFloat4x4(&entityTransform->m_transform.GetMatrix());
			DirectX::XMMATRIX wvp = world * view * proj;
			TransformConstants objConstants;
			XMStoreFloat4x4(&objConstants.WorldViewProj, DirectX::XMMatrixTranspose(wvp));

			// Mise a jour du constant buffer via le mapping persistant (m_mappedData)
			memcpy(entityMesh->m_cubeMesh->m_mappedData, &objConstants, sizeof(objConstants));

			// Attacher le constant buffer a la racine (slot 0)
			mCommandList->SetGraphicsRootConstantBufferView(0, entityMesh->m_cubeMesh->m_constantBuffer->GetGPUVirtualAddress());
			mCommandList->IASetVertexBuffers(0, 1, &entityMesh->m_cubeMesh->m_geometryMesh.m_vertexBufferView);
			mCommandList->IASetIndexBuffer(&entityMesh->m_cubeMesh->m_geometryMesh.m_indexBufferView);

			if (entityMesh->textureID.empty())
			{
				// Si aucun ID n'est defini, utilisez par defaut "PlayerTexture"
				entityMesh->textureID = L"PlayerTexture";
			}
			// Recuperation du handle de la texture a utiliser selon l'identifiant contenu dans le MeshComponent
			D3D12_GPU_DESCRIPTOR_HANDLE textureHandle = m_textureManager->GetTextureHandle(entityMesh->textureID);
			// Mise a jour du slot 1 de la root signature (table de descripteurs) pour pointer sur la texture
			mCommandList->SetGraphicsRootDescriptorTable(1, textureHandle);

			// Dessiner le cube (36 indices)
			mCommandList->DrawIndexedInstanced(entityMesh->m_cubeMesh->m_geometryMesh.m_meshIndex, 1, 0, 0, 0);
		}
	}
}

void InitDirect3DApp::CreatePipelineState()
{
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

	ComPtr<ID3DBlob> serializedRootSig = nullptr;
	ComPtr<ID3DBlob> error = nullptr;
	HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &serializedRootSig, &error);
	if (FAILED(hr))
	{
		if (error) MessageBoxA(0, (char*)error->GetBufferPointer(), "RootSignature Error", MB_OK);
		return;
	}

	hr = mD3DDevice->CreateRootSignature(0, serializedRootSig->GetBufferPointer(), serializedRootSig->GetBufferSize(), IID_PPV_ARGS(&mRootSignature));
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
		m_scene->OnUpdate();

		// update transform of entites


		// COLLISIONS a ajouter

	}

	// Collisions
	m_colliderManager->UpdateCollider();

	//for (auto& entity1 : m_entityManager->GetEntityTab())
	//{
	//	if (entity1 == nullptr)
	//		continue;
	//	
	//	TransformComponent* transform1 = nullptr;
	//	for (auto* component : m_entityManager->GetComponentsTab()[entity1->tab_index]->vec_components)
	//	{
	//		if (component->ID == Transform_ID)
	//		{
	//			transform1 = static_cast<TransformComponent*>(component);
	//		}
	//	}
	//	if (!transform1)
	//		continue;
	//	for (uint32_t entity2 = entity1->tab_index + 1; entity2 < 64000; entity2++)
	//	{
	//		if (!m_entityManager->GetEntityTab()[entity2])
	//			continue;

	//		TransformComponent* transform2 = nullptr;
	//		for (auto* component : m_entityManager->GetComponentsTab()[entity2]->vec_components)
	//		{
	//			if (component->ID == Transform_ID)
	//			{
	//				transform2 = static_cast<TransformComponent*>(component);
	//			}
	//		}
	//		if (!transform2)
	//			continue;

	//		// Test de collision
	//		if (AABBIntersect(*transform1, *transform2))
	//		{
	//			// Ici, vous pouvez ajouter le traitement n�cessaire en cas de collision
	//		}
	//	}

	//}


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

void InitDirect3DApp::UpdateTimer()
{
	DWORD currentTime = timeGetTime();
	m_deltaTime = (currentTime - m_lastTime) / 1000.0f; // conversion en secondes
	m_lastTime = currentTime;
}


void InitDirect3DApp::Draw()
{

	// Start des frames
	mCurrFrameIndex = (mCurrFrameIndex + 1) % gNumFrameResources;
	mCurrFrameResource = mFrameResources[mCurrFrameIndex].get();

	// Start du lag meter
	DWORD t = timeGetTime();

	// Reinitialise le command allocator et la command list
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


	// Affichage du lag meter

	/*wchar_t title[256];
	swprintf_s(title, 256, L"lag meters: %d", (int)dt);
	SetWindowText(GetActiveWindow(), title);*/
}