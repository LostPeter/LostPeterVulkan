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

#ifndef _VK_TEXTURE_H_
#define _VK_TEXTURE_H_

#include "Base.h"

namespace LostPeterVulkan
{
    class vulkanExport VKTexture : public Base
    {
    public:
        VKTexture(uint32 _group,
                  const String& _nameTexture,
                  const StringVector& _aPathTexture,
                  FTextureType _typeTexture,
                  FTexturePixelFormatType _typePixelFormat,
                  FTextureFilterType _typeFilter,
                  FTextureAddressingType _typeAddressing,
                  FTextureBorderColorType _typeBorderColor,
                  bool _isRenderTarget,
                  bool _isGraphicsComputeShared);
        virtual ~VKTexture();

    public:
        StringVector aPathTexture;
        FTextureType typeTexture;
        FTexturePixelFormatType typePixelFormat;
        FTextureFilterType typeFilter;
        FTextureAddressingType typeAddressing;
        FTextureBorderColorType typeBorderColor;
        bool isRenderTarget;
        bool isGraphicsComputeShared;

        int width;
        int height;
        int depth;

		VkImageUsageFlags rtImageUsage;
		VkImageAspectFlags aspectFlags;
		VkImageLayout poTextureImageLayout;

		uint32_t poMipMapCount;
		bool isAutoMipmap;

        VkFormat typeFormat; 
        VkImage poTextureImage;
        VkDeviceMemory poTextureImageMemory;
        VkImageView poTextureImageView;
        VkSampler poTextureSampler;
        VkDescriptorImageInfo poTextureImageInfo;
		VkDescriptorImageInfo poTextureImageInfo_NoSampler;

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;

        //Texture 3D
        uint8* pDataRGBA;
		bool isDeleteRGBA;

        //Texture Animation
        int texChunkMaxX;
        int texChunkMaxY;
        int texChunkIndex;
        int frameCurrent;

        //Texture RenderTarget
        FVector4 rtColorDefault;
        bool rtIsSetColor; 

    public:
        F_FORCEINLINE const VkImage& GetVkImage() const { return this->poTextureImage; }
		F_FORCEINLINE const VkDeviceMemory& GetVkBufferMemory() const { return this->poTextureImageMemory; }
		F_FORCEINLINE const VkImageView& GetVkImageView() const { return this->poTextureImageView; }
		F_FORCEINLINE const VkSampler& GetVkSampler() const { return this->poTextureSampler; }
		F_FORCEINLINE const VkDescriptorImageInfo& GetVkDescriptorImageInfo() const { return this->poTextureImageInfo; }
		F_FORCEINLINE const VkDescriptorImageInfo& GetVkDescriptorImageInfo_NoSampler() const { return this->poTextureImageInfo_NoSampler; }

    public:
        void Destroy();
        virtual int RandomTextureIndex();
        virtual void LoadTexture(int width,
                                 int height,
                                 int depth,
								 int channel = 4,
                                 uint8* pData = nullptr);
        virtual void UpdateTexture();

    protected:
        virtual void updateNoiseTextureData();
        virtual void updateNoiseTexture();
    };

}; //LostPeterVulkan

#endif