#include "pch.h"
#include "main.h"

#include <iostream>
#include <DirectXColors.h>
#include "WindowDX.h"
#include "TriangleRenderer.h"
#include "Camera.h"

class InitDirect3DApp : public WindowDX
{
public:
    InitDirect3DApp(HINSTANCE hInstance);
    bool Initialize();
    void Update() override;
    void Draw() override;

private:
    //std::unique_ptr<TriangleRenderer> m_TriangleRenderer;
    TriangleRenderer* m_TriangleRenderer;
    ComPtr<ID3D12PipelineState> mPSO;
    Camera m_Camera;

};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
    InitDirect3DApp theApp(hInstance);
    if (!theApp.Initialize())
        return 0;
    return theApp.Run();
}

InitDirect3DApp::InitDirect3DApp(HINSTANCE hInstance) : WindowDX(hInstance)
{
}

bool InitDirect3DApp::Initialize()
{
    if (!WindowDX::Initialize())
        return false;
    //TEST COMMIT 
 /*   m_TriangleRenderer = std::make_unique<TriangleRenderer>(mD3DDevice.Get(), mCommandQueue.Get(),mCommandList.Get(), mSwapChain.Get(), mRtvHeap.Get(),mRtvDescriptorSize);
    if (!m_TriangleRenderer->Initialize())
        return false;*/

        // Positionner la cam�ra � une position initiale
    m_Camera.SetPosition(0.0f, 0.0f, -5.0f); // Place la cam�ra en arri�re pour voir la sc�ne

    D3D12_DEPTH_STENCIL_DESC depthStencilDesc = {};
    depthStencilDesc.DepthEnable = TRUE;
    depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
    depthStencilDesc.StencilEnable = FALSE;

    float squareSize = 1.0f;
    DirectX::XMFLOAT4 squareColor = { 1.0f, 0.0f, 0.0f, 0.0f };

    m_TriangleRenderer = new TriangleRenderer(mD3DDevice.Get(), mCommandQueue.Get(), mCommandList.Get(), mSwapChain.Get(), mRtvHeap.Get(), mDsvHeap.Get(), mRtvDescriptorSize, squareSize, squareColor, depthStencilDesc, &m_Camera);
    if (!m_TriangleRenderer->Initialize())
    {
        delete m_TriangleRenderer;  // Liberation si l'initialisation echoue
        m_TriangleRenderer = nullptr;
        return false;
    }
    mPSO = m_TriangleRenderer->GetPipelineState();


    return true;
}
void InitDirect3DApp::Update()
{
    if (GetAsyncKeyState(VK_LEFT)) m_Camera.Move(-0.1f, 0, 0);
    if (GetAsyncKeyState(VK_RIGHT)) m_Camera.Move(0.1f, 0, 0);
    if (GetAsyncKeyState(VK_UP)) m_Camera.Move(0, 0.1f, 0);
    if (GetAsyncKeyState(VK_DOWN)) m_Camera.Move(0, -0.1f, 0);

    /*if (GetAsyncKeyState(VK_LEFT)) m_Camera.Rotate(0, -0.1f);
    if (GetAsyncKeyState(VK_RIGHT)) m_Camera.Rotate(0, 0.1f);
    if (GetAsyncKeyState(VK_UP)) m_Camera.Rotate(-0.1f, 0);
    if (GetAsyncKeyState(VK_DOWN)) m_Camera.Rotate(0.1f, 0);*/



    // Update logic for the triangle
    m_TriangleRenderer->Update();
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

    // D�finir le viewport et le rectangle de d�coupe (scissor) pour le rendu.
    mCommandList->RSSetViewports(1, &mScreenViewport);
    mCommandList->RSSetScissorRects(1, &mScissorRect);

    // R�cup�rer le handle du back buffer pour le rendu.
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(mRtvHeap->GetCPUDescriptorHandleForHeapStart(), mCurrBackBuffer, mRtvDescriptorSize);

    // R�cup�rer le handle du Depth Stencil View.
    CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(mDsvHeap->GetCPUDescriptorHandleForHeapStart());

    // Transitionner le back buffer de l'�tat PRESENT � RENDER_TARGET.
    CD3DX12_RESOURCE_BARRIER barrierStart = CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
    mCommandList->ResourceBarrier(1, &barrierStart);

    // Effacer le Render Target avec une couleur de fond
    FLOAT clearColor[] = { 0.0f, 0.0f, 1.0f, 1.0f };
    mCommandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

    // Effacer le Depth Buffer pour r�initialiser les valeurs de profondeur (1.0 = loin).
    mCommandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

    // Attacher le Render Target et le Depth Buffer � l'Output Merger.
    mCommandList->OMSetRenderTargets(1, &rtvHandle, true, &dsvHandle);

    // Appeler le renderer de l'objet
    m_TriangleRenderer->Render(); // Rendu du triangle ici

    // Transitionner le back buffer de RENDER_TARGET � PRESENT pour la pr�sentation.
    CD3DX12_RESOURCE_BARRIER barrierStop = CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
    mCommandList->ResourceBarrier(1, &barrierStop);

    // Fermer la command list et l'ex�cuter sur la GPU.
    mCommandList->Close();
    ID3D12CommandList* ppCommandLists[] = { mCommandList.Get() };
    mCommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

    // Attendre la fin de l'ex�cution des commandes et pr�senter le swap chain.
    FlushCommandQueue();
    mSwapChain->Present(0, 0);
    mCurrBackBuffer = (mCurrBackBuffer + 1) % SwapChainBufferCount;
}