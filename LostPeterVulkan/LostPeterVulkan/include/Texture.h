/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-04
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "Base.h"

namespace LostPeter
{
    class utilExport Texture : public Base
    {
    public:
        Texture(const String& _nameTexture,
                FTextureType _typeTexture,
                bool _isRenderTarget,
                bool _isGraphicsComputeShared,
                VkFormat _typeFormat,
                VulkanTextureFilterType _typeFilter,
                VulkanTextureAddressingType _typeAddressing,
                VulkanTextureBorderColorType _typeBorderColor,
                const StringVector& _aPathTexture);
        virtual ~Texture();

    public:
        StringVector aPathTexture;
        FTextureType typeTexture;
        bool isRenderTarget;
        bool isGraphicsComputeShared;
        VkFormat typeFormat; 
        VulkanTextureFilterType typeFilter;
        VulkanTextureAddressingType typeAddressing;
        VulkanTextureBorderColorType typeBorderColor;
        int width;
        int height;
        int depth;

        uint32_t poMipMapCount;
        VkImage poTextureImage;
        VkDeviceMemory poTextureImageMemory;
        VkImageView poTextureImageView;
        VkSampler poTextureSampler;
        VkDescriptorImageInfo poTextureImageInfo;
        VkImageLayout poTextureImageLayout;

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;

        //Texture 3D
        uint8* pDataRGBA;

        //Texture Animation
        int texChunkMaxX;
        int texChunkMaxY;
        int texChunkIndex;
        int frameCurrent;

        //Texture RenderTarget
        FVector4 rtColorDefault;
        bool rtIsSetColor; 
        VkImageUsageFlags rtImageUsage;

    public:
        void Destroy();
        virtual int RandomTextureIndex();
        virtual void LoadTexture(int width,
                                 int height,
                                 int depth);
        virtual void UpdateTexture();

    protected:
        virtual void updateNoiseTextureData();
        virtual void updateNoiseTexture();
    };

}; //LostPeter

#endif