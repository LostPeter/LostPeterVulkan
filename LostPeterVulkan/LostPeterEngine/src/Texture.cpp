/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-04
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/Texture.h"

namespace LostPeter
{
    Texture::Texture(uint32 _group,
                     const String& _nameTexture,
                     const StringVector& _aPathTexture,
                     FTextureType _typeTexture,
                     FTexturePixelFormatType _typePixelFormat,
                     FTextureFilterType _typeFilter,
                     FTextureAddressingType _typeAddressing,
                     FTextureBorderColorType _typeBorderColor,
                     bool _isRenderTarget,
                     bool _isGraphicsComputeShared)
        : Base(_group, _nameTexture)
        , aPathTexture(_aPathTexture)
        , typeTexture(_typeTexture)
        , typePixelFormat(_typePixelFormat)
        , typeFilter(_typeFilter)
        , typeAddressing(_typeAddressing)
        , typeBorderColor(_typeBorderColor)
        , isRenderTarget(_isRenderTarget)
        , isGraphicsComputeShared(_isGraphicsComputeShared)
        
        , width(0)
        , height(0)
        , depth(0)

        , poMipMapCount(1)
        , poTextureImage(VK_NULL_HANDLE)
        , poTextureImageMemory(VK_NULL_HANDLE)
        , poTextureImageView(VK_NULL_HANDLE)
        , poTextureSampler(VK_NULL_HANDLE)

        , stagingBuffer(VK_NULL_HANDLE)
        , stagingBufferMemory(VK_NULL_HANDLE)

        //Texture 3D
        , pDataRGBA(nullptr)

        //Texture Animation
        , texChunkMaxX(0)
        , texChunkMaxY(0)
        , texChunkIndex(0)
        , frameCurrent(0)

        //Texture RenderTarget
        , rtColorDefault(0, 0, 0, 1)
        , rtIsSetColor(false)
        , rtImageUsage(VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_STORAGE_BIT)
    {
        this->typeFormat = Util_Transform2VkFormat(this->typePixelFormat);
        if (this->isRenderTarget)
            this->poTextureImageLayout = VK_IMAGE_LAYOUT_GENERAL;
        else
            this->poTextureImageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    }
    Texture::~Texture()
    {
        Destroy();
    }
    void Texture::Destroy()
    {
       
    }
    int Texture::RandomTextureIndex()
    {
        if (this->typeTexture == F_Texture_2DArray)
        {
            int count = (int)this->aPathTexture.size();
            return FMath::Rand(0, count - 1);
        }
        return 0;
    }
    void Texture::LoadTexture(int width,
                              int height,
                              int depth)
    {
        
    }   
    void Texture::UpdateTexture()
    {
        if (this->typeTexture == F_Texture_3D)
        {
            updateNoiseTexture();
        }
    }
    void Texture::updateNoiseTextureData()
    {
        // Perlin noise
        noise::module::Perlin modulePerlin;
        for (int z = 0; z < this->depth; z++)
        {
            for (int y = 0; y < this->height; y++)
            {
                for (int x = 0; x < this->width; x++)
                {
                    float nx = (float)x / (float)this->width;
                    float ny = (float)y / (float)this->height;
                    float nz = (float)z / (float)this->depth;

                    float n = 20.0f * modulePerlin.GetValue(nx, ny, nz);
                    n = n - floor(n);
                    this->pDataRGBA[x + y * this->width + z * this->width * this->height] = static_cast<uint8>(floor(n * 255));
                }
            }
        }
    }
    void Texture::updateNoiseTexture()
    {
        
    }

}; //LostPeter