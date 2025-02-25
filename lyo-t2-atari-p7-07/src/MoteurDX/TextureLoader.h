#pragma once

#include <d3d12.h>
#include <wrl.h>
#include <cstdint>

#include "d3dx12.h"

namespace TextureLoader
{
    // Structure decrivant le format de pixel dans le header DDS.
    struct DDS_PIXELFORMAT
    {
        uint32_t size;
        uint32_t flags;
        uint32_t fourCC;
        uint32_t RGBBitCount;
        uint32_t RBitMask;
        uint32_t GBitMask;
        uint32_t BBitMask;
        uint32_t ABitMask;
    };

    // Structure du header DDS.
    struct DDS_HEADER
    {
        uint32_t size;
        uint32_t flags;
        uint32_t height;
        uint32_t width;
        uint32_t pitchOrLinearSize;
        uint32_t depth;
        uint32_t mipMapCount;
        uint32_t reserved1[11];
        DDS_PIXELFORMAT ddspf;
        uint32_t caps;
        uint32_t caps2;
        uint32_t caps3;
        uint32_t caps4;
        uint32_t reserved2;
    };

    // Charge une texture DDS a partir d'un fichier, et cree :
    // - 'texture' : la ressource texture dans le default heap,
    // - 'textureUploadHeap' : le tas d'upload associe pour copier les donnees.
    void LoadDDSTexture(
        ID3D12Device* device,
        ID3D12GraphicsCommandList* commandList,
        const wchar_t* filename,
        Microsoft::WRL::ComPtr<ID3D12Resource>& texture,
        Microsoft::WRL::ComPtr<ID3D12Resource>& textureUploadHeap);
}
