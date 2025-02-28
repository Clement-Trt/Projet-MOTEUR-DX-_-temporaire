#pragma once
#include <unordered_map>
#include <string>
#include <wrl.h>
#include <d3d12.h>
#include "d3dx12.h"

using Microsoft::WRL::ComPtr;

class TextureManager {
public:
    TextureManager(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList)
        : mDevice(device), mCmdList(cmdList) {
    }

    // Charge une texture à partir d'un fichier et la stocke avec un identifiant
    bool LoadTexture(const std::wstring& id, const std::wstring& filename);

    // Retourne le GPU descriptor handle pour la texture associée à l'id
    D3D12_GPU_DESCRIPTOR_HANDLE GetTextureHandle(const std::wstring& id) const;

    // Pour simplifier, on suppose un unique descriptor heap partagé pour toutes les textures.
    void CreateDescriptorHeap(UINT numDescriptors);

    ID3D12DescriptorHeap* GetSrvHeap() const { return m_srvHeap.Get(); }

private:
    ID3D12Device* mDevice = nullptr;
    ID3D12GraphicsCommandList* mCmdList = nullptr;
    ComPtr<ID3D12DescriptorHeap> m_srvHeap;
    UINT mDescriptorSize = 0;

    // On stocke les offset (dans le heap) associés à chaque texture
    std::unordered_map<std::wstring, UINT> mTextureOffsets;

    std::unordered_map<std::wstring, Microsoft::WRL::ComPtr<ID3D12Resource>> m_textures;

    std::vector<ComPtr<ID3D12Resource>> m_uploadHeaps;


};
