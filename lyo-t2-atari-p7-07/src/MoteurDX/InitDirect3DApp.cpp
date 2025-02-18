#include "pch.h"
#include "InitDirect3DApp.h"
#include "GameManager.h"
#include "MeshFactory.h"
#include "InputManager.h"
#include "SceneTest.h"



InitDirect3DApp::InitDirect3DApp(HINSTANCE hInstance) : WindowDX(hInstance)
{
}

bool InitDirect3DApp::Initialize()
{
    if (!WindowDX::Initialize())
        return false;

    // Initialisation Game Manager et Scene (ECS)
    // ---
    m_GM = new GameManager;

    //Scene* scene = new Scene;
    SceneTest* scene = new SceneTest;

    m_GM->Initialize(scene);
    // ---


    // Positionner la camera a une position initiale
    m_Camera.SetPosition(0.0f, 0.0f, -5.0f); // Place la camera en arriere pour voir la scene

    D3D12_DEPTH_STENCIL_DESC depthStencilDesc = {};
    depthStencilDesc.DepthEnable = TRUE;
    depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
    depthStencilDesc.StencilEnable = FALSE;

    float squareSize = 1.0f;
    DirectX::XMFLOAT4 squareColor = { 1.0f, 0.0f, 0.0f, 0.0f };

    // MeshFactory
    m_meshFactory = new MeshFactory;
    //m_Camera = new Camera; 
    m_meshFactory->InitMeshFactory(mD3DDevice.Get(), mCommandQueue.Get(), mCommandList.Get(), mSwapChain.Get(), mRtvHeap.Get(), mDsvHeap.Get(), mRtvDescriptorSize, depthStencilDesc, &m_Camera);
    MessageBox(0, L"InitReussiMeshFacto", 0, 0);

    m_meshFactory->CreateCube(1.0f, 1.0f, 1.0f, 0.0f, 2.0f, 0.0f);
    m_meshFactory->CreateCube(3.0f, 3.0f, 3.0f, 0.0f, 0.0f, 0.0f);
    m_meshFactory->CreateCube(2.0f, 2.0f, 2.0f, 3.0f, 0.0f, 0.0f);
    m_meshFactory->CreateCube(3.0f, 3.0f, 3.0f, 0.0f, 0.0f, 0.0f);
    m_meshFactory->CreateCube(3.0f, 1.0f, 3.0f, 0.0f, 0.0f, 0.0f);
    //MessageBox(0, L"CreationDuCube", 0, 0);

    mPSO = m_meshFactory->GetPipelineState();


    return true;
}
void InitDirect3DApp::Update()
{
    //SetDeltaTime(mDeltaTime); // CHANGER PAR SYSTEME DE TIMER

    //HandleInput(); // AJOUTER SYSTEME DE GESTION D'INPUT

    m_GM->Update();


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

    // Update MeshFactory
    m_meshFactory->Update();
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
    m_meshFactory->Render();

    // Change le titre de la fentre (peux servir pour le debug)
    wchar_t title[256];
    swprintf_s(title, 256, L"NBcube: %d", (int)m_meshFactory->GetCubeList()->size());
    SetWindowText(GetActiveWindow(), title);

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