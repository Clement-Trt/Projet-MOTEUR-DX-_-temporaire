#include "pch.h"
#include "TextureLoader.h"

#include <fstream>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <cstdint>
#include <cstring>

namespace TextureLoader
{
    void LoadDDSTexture(
        ID3D12Device* device,
        ID3D12GraphicsCommandList* commandList,
        const wchar_t* filename,
        Microsoft::WRL::ComPtr<ID3D12Resource>& texture,
        Microsoft::WRL::ComPtr<ID3D12Resource>& textureUploadHeap)
    {
        // Ouvrir le fichier DDS en mode binaire.
        std::ifstream file(filename, std::ios::binary | std::ios::ate);
        if (!file)
            throw std::runtime_error("echec de l'ouverture du fichier DDS.");

        std::streamsize fileSize = file.tellg();
        file.seekg(0, std::ios::beg);
        std::vector<char> fileData(static_cast<size_t>(fileSize));
        if (!file.read(fileData.data(), fileSize))
            throw std::runtime_error("echec de la lecture du fichier DDS.");
        file.close();

        // Verifier la signature "DDS " dans les 4 premiers octets.
        if (fileData.size() < 4 || memcmp(fileData.data(), "DDS ", 4) != 0)
            throw std::runtime_error("Fichier DDS invalide.");

        if (fileData.size() < 4 + sizeof(DDS_HEADER))
            throw std::runtime_error("Header DDS incomplet.");

        DDS_HEADER* ddsHeader = reinterpret_cast<DDS_HEADER*>(fileData.data() + 4);

        // Verifier si la texture est compressee.
       /* bool isCompressed = (ddsHeader->ddspf.flags & DDS_FOURCC) != 0;
        if (isCompressed)
            throw std::runtime_error("La texture est compressee. Cette version du loader attend une texture non compressee.");*/

        uint32_t width = ddsHeader->width;
        uint32_t height = ddsHeader->height;
        uint32_t mipMapCount = ddsHeader->mipMapCount;
        if (mipMapCount == 0)
            mipMapCount = 1;

        // Pour cet exemple minimal, nous supposons que le format est DXGI_FORMAT_R8G8B8A8_UNORM.
        DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;

        // Definir la description de la texture.
        D3D12_RESOURCE_DESC textureDesc = {};
        textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
        textureDesc.Alignment = 0;
        textureDesc.Width = width;
        textureDesc.Height = height;
        textureDesc.DepthOrArraySize = 1;
        textureDesc.MipLevels = mipMapCount;
        textureDesc.Format = format;
        textureDesc.SampleDesc.Count = 1;
        textureDesc.SampleDesc.Quality = 0;
        textureDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
        textureDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

        // Creer la ressource texture dans le default heap en etat COPY_DEST.
        CD3DX12_HEAP_PROPERTIES defaultHeapProps(D3D12_HEAP_TYPE_DEFAULT);
        HRESULT hr = device->CreateCommittedResource(
            &defaultHeapProps,
            D3D12_HEAP_FLAG_NONE,
            &textureDesc,
            D3D12_RESOURCE_STATE_COPY_DEST,
            nullptr,
            IID_PPV_ARGS(&texture));
        if (FAILED(hr))
            throw std::runtime_error("echec de la creation de la ressource texture.");

        // Calculer la taille requise pour le heap d'upload.
        UINT64 uploadBufferSize = 0;
        device->GetCopyableFootprints(&textureDesc, 0, mipMapCount, 0, nullptr, nullptr, nullptr, &uploadBufferSize);

        if (uploadBufferSize == 0)
        {
            throw std::runtime_error("echec du calcul de la taille de l'upload buffer !");
        }
        // Creer le resource upload heap.
        auto uploadBufferDesc = CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize);
        CD3DX12_HEAP_PROPERTIES uploadHeapProps(D3D12_HEAP_TYPE_UPLOAD);
        hr = device->CreateCommittedResource(
            &uploadHeapProps,
            D3D12_HEAP_FLAG_NONE,
            &uploadBufferDesc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&textureUploadHeap));
        if (FAILED(hr))
            throw std::runtime_error("echec de la creation du tas d'upload pour la texture.");

        size_t headerSize = 4 + sizeof(DDS_HEADER);
        const char* textureData = fileData.data() + headerSize;

        // Preparer les sous-ressources pour chaque niveau de mipmap.
        std::vector<D3D12_SUBRESOURCE_DATA> subresources(mipMapCount);
        uint32_t mipWidth = width;
        uint32_t mipHeight = height;
        const char* pData = textureData;

        for (uint32_t mip = 0; mip < mipMapCount; mip++)
        {
            // Verifier si la texture est compressee
            bool isCompressed = (ddsHeader->ddspf.flags & 0x4) != 0;
            char fourCC[5] = { 0 };
            memcpy(fourCC, &ddsHeader->ddspf.fourCC, 4);
            OutputDebugStringA(fourCC); // Affiche par exemple "DXT1" ou "DXT5"

            size_t rowPitch = 0;
            size_t slicePitch = 0;

            if (isCompressed)
            {
                MessageBoxA(0, 0, "Texture isCompressed", MB_OK);
                UINT blockSize = 0;
                if (ddsHeader->ddspf.fourCC == MAKEFOURCC('D', 'X', 'T', '1'))
                    blockSize = 8;
                else if (ddsHeader->ddspf.fourCC == MAKEFOURCC('D', 'X', 'T', '3') ||
                    ddsHeader->ddspf.fourCC == MAKEFOURCC('D', 'X', 'T', '5'))
                    blockSize = 16;
                else
                    throw std::runtime_error("Format compresse non supporte.");

                /*size_t numBlocksWide = (mipWidth + 3) / 4;
                size_t numBlocksHigh = (mipHeight + 3) / 4;*/
                size_t numBlocksWide = std::max<size_t>(1u, (mipWidth + 3) / 4);
                size_t numBlocksHigh = std::max<size_t>(1u, (mipHeight + 3) / 4);
                /*rowPitch = numBlocksWide * blockSize;
                slicePitch = rowPitch * numBlocksHigh;*/
                rowPitch = ((mipWidth + 3) / 4) ;
                slicePitch = rowPitch * ((mipHeight + 3) / 4);
                /*rowPitch = numBlocksWide * blockSize;
                slicePitch = rowPitch * numBlocksHigh;*/
            }
            else
            {
                MessageBoxA(0, 0, "Texture is Not Compressed", MB_OK);
                rowPitch = mipWidth * 4;         // 4 octets par pixel
                slicePitch = rowPitch * mipHeight;
            }


            // Pour une texture compressee, le RowPitch est calcule en nombre de blocs
           /* size_t rowPitch = (((mipWidth + 3) / 4));
            size_t slicePitch = rowPitch * ((mipHeight + 3) / 4);*/

            // Pour une texture non compressee : 4 octets par pixel
            // size_t rowPitch = mipWidth * 4; // 4 octets par pixel pour un format RGBA8
            // size_t slicePitch = rowPitch * mipHeight;

            subresources[mip].pData = pData;
            subresources[mip].RowPitch = rowPitch;
            subresources[mip].SlicePitch = slicePitch;

            pData += slicePitch; // Avance le pointeur pour le prochain mip

            // Mettre a jour mipWidth et mipHeight en divisant par 2 a chaque niveau, minimum 1.
            mipWidth = std::max<uint32_t>(1u, mipWidth / 2);
            mipHeight = std::max<uint32_t>(1u, mipHeight / 2);
        }

        if (mipMapCount == 0 || mipMapCount > 16)
        {
            throw std::runtime_error("Nombre de mipmaps invalide !");
        }

        // Copier les sous-ressources depuis l'upload heap vers la ressource texture.
        UpdateSubresources(commandList, texture.Get(), textureUploadHeap.Get(), 0, 0, mipMapCount, subresources.data());

        // Transitionner la texture vers l'etat PIXEL_SHADER_RESOURCE.
        CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
            texture.Get(),
            D3D12_RESOURCE_STATE_COPY_DEST,
            D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
        commandList->ResourceBarrier(1, &barrier);
    }
}
