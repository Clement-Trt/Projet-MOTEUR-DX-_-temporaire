#pragma once
#include <unordered_map>
#include <string>
#include <wrl.h>
#include <d3d12.h>


using Microsoft::WRL::ComPtr;

class TextureManager {
public:
    TextureManager(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList)
        : mp_device(device), mp_cmdList(cmdList) {
    }

    // Charge une texture �Epartir d'un fichier et la stocke avec un identifiant
    bool LoadTexture(const std::wstring& id, const std::wstring& filename);

    // Retourne le GPU descriptor handle pour la texture associee �El'id
    D3D12_GPU_DESCRIPTOR_HANDLE GetTextureHandle(const std::wstring& id) const;

    // Pour simplifier, on suppose un unique descriptor heap partag�Epour toutes les textures.
    void CreateDescriptorHeap(UINT numDescriptors);

    ID3D12DescriptorHeap* GetSrvHeap() const { return m_srvHeap.Get(); }

private:
    ID3D12Device* mp_device = nullptr;
    ID3D12GraphicsCommandList* mp_cmdList = nullptr;
    ComPtr<ID3D12DescriptorHeap> m_srvHeap;
    UINT m_descriptorSize = 0;

    // On stocke les offset (dans le heap) associes �Echaque texture
    std::unordered_map<std::wstring, UINT> m_textureOffsets;

    std::unordered_map<std::wstring, Microsoft::WRL::ComPtr<ID3D12Resource>> m_textures;

    std::vector<ComPtr<ID3D12Resource>> m_uploadHeaps;


};
