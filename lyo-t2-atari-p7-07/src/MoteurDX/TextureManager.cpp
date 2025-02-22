#include "pch.h"
#include "TextureManager.h"
#include "TextureLoaderDuLivre.h" 

bool TextureManager::LoadTexture(const std::wstring& id, const std::wstring& filename)
{
    ComPtr<ID3D12Resource> texture;
    ComPtr<ID3D12Resource> textureUploadHeap;
    HRESULT hr = DirectX::CreateDDSTextureFromFile12(mDevice, mCmdList,
        filename.c_str(),
        texture,
        textureUploadHeap);
    if (FAILED(hr))
        return false;

    // Stocker la texture pour éviter qu'elle ne soit détruite
    m_textures[id] = texture;
    
    // Conserver l'upload heap dans le vector pour qu'il reste valide
    m_uploadHeaps.push_back(textureUploadHeap);

    // Creer le SRV pour la texture
    // Le Shader Resource View (SRV) est un descripteur qui définit comment une ressource (ici une texture) est vue par les shaders. Il précise notamment 
    // le format de la texture, le nombre de mipmaps et la dimension de la ressource. Grâce au SRV, le shader (souvent le pixel shader) peut échantillonner 
    // la texture pour en extraire les données de couleur lors du rendu.
    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.Format = texture->GetDesc().Format;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.MipLevels = texture->GetDesc().MipLevels;

    // Calculer l'offset à utiliser : on prend le prochain slot libre dans le heap
    UINT offset = static_cast<UINT>(mTextureOffsets.size());
    CD3DX12_CPU_DESCRIPTOR_HANDLE handle(m_srvHeap->GetCPUDescriptorHandleForHeapStart(), offset, mDescriptorSize);
    mDevice->CreateShaderResourceView(texture.Get(), &srvDesc, handle);

    mTextureOffsets[id] = offset;
    return true;
}

D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetTextureHandle(const std::wstring& id) const
{
    auto it = mTextureOffsets.find(id);
    if (it == mTextureOffsets.end())
    {
        // Retour d'un handle invalide si l'id n'existe pas
        return {};
    }
    UINT offset = it->second;
    CD3DX12_GPU_DESCRIPTOR_HANDLE handle(m_srvHeap->GetGPUDescriptorHandleForHeapStart(), offset, mDescriptorSize);
    return handle;
}

void TextureManager::CreateDescriptorHeap(UINT numDescriptors)
{
    // Creer un descriptor heap pour le SRV 
    // Le descriptor heap est une zone mémoire réservée sur la carte graphique qui stocke plusieurs descripteurs. Dans le cas du SRV, il contient les 
    // descriptions de ressources (comme les textures) afin que le GPU puisse y accéder directement lors du rendu. Une fois créé et rempli, le heap permet 
    // de regrouper et de gérer ces descripteurs de façon efficace et de les rendre visibles aux shaders.
    D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
    heapDesc.NumDescriptors = numDescriptors;
    heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    // On doit le rendre visible aux shaders
    heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    HRESULT hr = mDevice->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&m_srvHeap));
    if (FAILED(hr))
    {
        // Gérer l'erreur
    }
    mDescriptorSize = mDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}
