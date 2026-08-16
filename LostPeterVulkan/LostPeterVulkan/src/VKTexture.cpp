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

#include "../include/VKTexture.h"
#include "../include/VulkanWindow.h"

namespace LostPeterVulkan
{
    VKTexture::VKTexture(uint32 _group,
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
		, isDeleteRGBA(true)

        //Texture Animation
        , texChunkMaxX(0)
        , texChunkMaxY(0)
        , texChunkIndex(0)
        , frameCurrent(0)

        //Texture RenderTarget
        , rtColorDefault(0, 0, 0, 1)
        , rtIsSetColor(false)
        , rtImageUsage(VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT)
    {
        this->typeFormat = Util_Transform2VkFormat(this->typePixelFormat);
        if (this->isRenderTarget)
            this->poTextureImageLayout = VK_IMAGE_LAYOUT_GENERAL;
        else
            this->poTextureImageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    }
    VKTexture::~VKTexture()
    {
        Destroy();
    }
    void VKTexture::Destroy()
    {
		VulkanWindow* pWindow = Base::GetWindowPtr();

        if (this->stagingBuffer != VK_NULL_HANDLE)
        {
            pWindow->destroyVkBuffer(this->stagingBuffer, this->stagingBufferMemory);
        }
        pWindow->destroyVkImage(this->poTextureImage, this->poTextureImageMemory, this->poTextureImageView);
        this->poTextureImage = VK_NULL_HANDLE;
        this->poTextureImageMemory = VK_NULL_HANDLE;
        this->poTextureImageView = VK_NULL_HANDLE;
        pWindow->destroyVkImageSampler(this->poTextureSampler);
        this->poTextureSampler = VK_NULL_HANDLE;

		if (this->isDeleteRGBA)
        {
            F_DELETE_T(this->pDataRGBA)
        }
    }
    int VKTexture::RandomTextureIndex()
    {
        if (this->typeTexture == F_Texture_2DArray)
        {
            int count = (int)this->aPathTexture.size();
            return FMath::Rand(0, count - 1);
        }
        return 0;
    }
    void VKTexture::LoadTexture(int width,
                                int height,
                                int depth,
							    int channel /*= 4*/,
                                uint8* pData /*= nullptr*/)
    {
        this->width = width;
        this->height = height;
        this->depth = depth;

		VulkanWindow* pWindow = Base::GetWindowPtr();

        if (!this->isRenderTarget)
        {
            if (this->typeTexture == F_Texture_1D)
            {
                pWindow->createTexture1D(this->name,
										 this->aPathTexture[0], 
										 this->poMipMapCount, 
										 this->poTextureImage, 
										 this->poTextureImageMemory);
                pWindow->createVkImageView(this->name,
										   this->poTextureImage, 
										   VK_IMAGE_VIEW_TYPE_1D, 
										   this->typeFormat, 
										   VK_IMAGE_ASPECT_COLOR_BIT, 
										   this->poMipMapCount, 
										   1, 
										   this->poTextureImageView);
            }
            else if (this->typeTexture == F_Texture_2D)
            {
                pWindow->createTexture2D(this->name,
										 this->aPathTexture[0], 
										 VK_IMAGE_TYPE_2D, 
										 VK_SAMPLE_COUNT_1_BIT, 
										 this->typeFormat, true, 
										 this->poMipMapCount, 
										 this->poTextureImage, 
										 this->poTextureImageMemory);
                pWindow->createVkImageView(this->name,
										   this->poTextureImage, 
										   VK_IMAGE_VIEW_TYPE_2D, 
										   this->typeFormat, 
										   VK_IMAGE_ASPECT_COLOR_BIT, 
										   this->poMipMapCount, 
										   1, 
										   this->poTextureImageView);
            }
            else if (this->typeTexture == F_Texture_2DArray)
            {
                pWindow->createTexture2DArray(this->name,
											  this->aPathTexture, 
											  VK_IMAGE_TYPE_2D,
											  VK_SAMPLE_COUNT_1_BIT, 
											  this->typeFormat, 
											  true, 
											  this->poMipMapCount, 
											  this->poTextureImage, 
											  this->poTextureImageMemory);
                pWindow->createVkImageView(this->name,
										   this->poTextureImage, 
										   VK_IMAGE_VIEW_TYPE_2D_ARRAY, 
										   this->typeFormat, 
										   VK_IMAGE_ASPECT_COLOR_BIT, 
										   this->poMipMapCount, 
										   (int)this->aPathTexture.size(), 
										   this->poTextureImageView);
            }
            else if (this->typeTexture == F_Texture_3D)
            {
                uint32_t size = width * height * depth;
                this->pDataRGBA = new uint8[size];
                memset(this->pDataRGBA, 0, (size_t)size);
                updateNoiseTextureData();
                pWindow->createTexture3D(this->name,
										 this->typeFormat, 
										 this->pDataRGBA, 
										 size, 
										 width, 
										 height, 
										 depth, 
										 this->poTextureImage, 
										 this->poTextureImageMemory, 
										 this->stagingBuffer, 
										 this->stagingBufferMemory);
                pWindow->createVkImageView(this->name,
										   this->poTextureImage, 
										   VK_IMAGE_VIEW_TYPE_3D, 
										   this->typeFormat, 
										   VK_IMAGE_ASPECT_COLOR_BIT, 
										   this->poMipMapCount, 
										   1, 
										   this->poTextureImageView);
            }
            else if (this->typeTexture == F_Texture_CubeMap)
            {
                pWindow->createTextureCubeMap(this->name,
											  this->aPathTexture, 
											  this->poMipMapCount, 
											  this->poTextureImage, 
											  this->poTextureImageMemory);
                pWindow->createVkImageView(this->name,
										   this->poTextureImage, 
										   VK_IMAGE_VIEW_TYPE_CUBE, 
										   this->typeFormat, 
										   VK_IMAGE_ASPECT_COLOR_BIT, 
										   this->poMipMapCount, 
										   (int)this->aPathTexture.size(), 
										   this->poTextureImageView);
            }   
            else
            {
                String msg = "*********************** VKTexture::LoadTexture: Wrong texture type, Create from file, name: [" + this->name + "] !";
                F_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }
        }
        else
        {
            if (this->typeTexture == F_Texture_1D)
            {
                pWindow->createTextureRenderTarget1D(this->name,
													 this->rtColorDefault, 
													 this->rtIsSetColor, 
													 channel,
													 this->width, 
													 false,
													 this->poMipMapCount, 
													 VK_SAMPLE_COUNT_1_BIT, 
													 this->typeFormat, 
													 this->rtImageUsage,
													 VK_IMAGE_LAYOUT_GENERAL,
													 this->isGraphicsComputeShared,
													 this->poTextureImage, 
													 this->poTextureImageMemory);
                pWindow->createVkImageView(this->name,
										   this->poTextureImage, 
										   VK_IMAGE_VIEW_TYPE_1D, 
										   this->typeFormat, 
										   VK_IMAGE_ASPECT_COLOR_BIT, 
										   this->poMipMapCount, 
										   1, 
										   this->poTextureImageView);
            } 
            else if (this->typeTexture == F_Texture_2D)
            {
                pWindow->createTextureRenderTarget2D(this->name,
													 pData,
													 channel,
													 this->width, 
													 this->height,
													 false,
													 this->poMipMapCount, 
													 VK_SAMPLE_COUNT_1_BIT, 
													 this->typeFormat, 
													 this->rtImageUsage,
													 VK_IMAGE_LAYOUT_GENERAL,
													 this->isGraphicsComputeShared,
													 this->poTextureImage, 
													 this->poTextureImageMemory);
                pWindow->createVkImageView(this->name,
										   this->poTextureImage, 
										   VK_IMAGE_VIEW_TYPE_2D, 
										   this->typeFormat, 
										   VK_IMAGE_ASPECT_COLOR_BIT, 
										   this->poMipMapCount, 
										   1, 
										   this->poTextureImageView);
            }
            else if (this->typeTexture == F_Texture_2DArray)
            {
                pWindow->createTextureRenderTarget2DArray(this->name,
														  this->rtColorDefault, 
														  this->rtIsSetColor, 
														  channel,
														  this->width, 
														  this->height,
														  this->depth,
														  false,
														  this->poMipMapCount, 
														  VK_SAMPLE_COUNT_1_BIT, 
														  this->typeFormat, 
														  this->rtImageUsage,
														  VK_IMAGE_LAYOUT_GENERAL,
														  this->isGraphicsComputeShared,
														  this->poTextureImage, 
														  this->poTextureImageMemory);
                pWindow->createVkImageView(this->name,
										   this->poTextureImage, 
										   VK_IMAGE_VIEW_TYPE_2D_ARRAY, 
										   this->typeFormat, 
										   VK_IMAGE_ASPECT_COLOR_BIT, 
										   this->poMipMapCount, 
										   (int)this->aPathTexture.size(), 
										   this->poTextureImageView);
            }
            else if (this->typeTexture == F_Texture_3D)
            {
				if (pData == nullptr)
                {
                    uint32_t size = width * height * depth * channel;
                    this->pDataRGBA = new uint8[size];
                    memset(this->pDataRGBA, 0, (size_t)size);
                    updateNoiseTextureData();
                    this->isDeleteRGBA = true;
                }
                else 
                {
                    this->pDataRGBA = pData;
                    this->isDeleteRGBA = false;
                }
                pWindow->createTextureRenderTarget3D(this->name,
													 this->pDataRGBA, 
													 channel, 
													 this->width, 
													 this->height,
													 this->depth,
													 false,
													 this->poMipMapCount, 
													 VK_SAMPLE_COUNT_1_BIT,
													 this->typeFormat, 
													 this->rtImageUsage,
													 VK_IMAGE_LAYOUT_GENERAL,
													 this->isGraphicsComputeShared,
													 this->poTextureImage, 
													 this->poTextureImageMemory);
                pWindow->createVkImageView(this->name,
										   this->poTextureImage, 
										   VK_IMAGE_VIEW_TYPE_3D, 
										   this->typeFormat, 
										   VK_IMAGE_ASPECT_COLOR_BIT, 
										   this->poMipMapCount, 
										   1, 
										   this->poTextureImageView);
            }
            else if (this->typeTexture == F_Texture_CubeMap)
            {
                pWindow->createTextureRenderTargetCubeMap(this->name,
														  this->width, 
														  this->height,
														  false,
														  this->poMipMapCount, 
														  VK_SAMPLE_COUNT_1_BIT,
														  this->typeFormat, 
														  this->rtImageUsage,
														  VK_IMAGE_LAYOUT_GENERAL,
														  this->isGraphicsComputeShared,
														  this->poTextureImage, 
														  this->poTextureImageMemory);
                pWindow->createVkImageView(this->name,
									   	   this->poTextureImage, 
										   VK_IMAGE_VIEW_TYPE_CUBE, 
										   this->typeFormat, 
										   VK_IMAGE_ASPECT_COLOR_BIT, 
										   this->poMipMapCount, 
										   6, 
										   this->poTextureImageView);
            }
            else
            {
                String msg = "*********************** VKTexture::LoadTexture: Wrong texture type, Create render target, name: [" + this->name + "] !";
                F_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }
        }

        pWindow->createVkSampler(this->name,
								 this->typeFilter, 
								 this->typeAddressing,
								 this->typeBorderColor,
								 true,
								 pWindow->poPhysicalDeviceProperties.limits.maxSamplerAnisotropy,
								 0.0f,
								 static_cast<float>(this->poMipMapCount),
								 0.0f,
								 this->poTextureSampler);

        this->poTextureImageInfo = {};
        this->poTextureImageInfo.imageLayout = this->poTextureImageLayout;
        this->poTextureImageInfo.imageView = this->poTextureImageView;
        this->poTextureImageInfo.sampler = this->poTextureSampler;

		this->poTextureImageInfo_NoSampler = {};
        this->poTextureImageInfo_NoSampler.imageLayout = this->poTextureImageLayout;
        this->poTextureImageInfo_NoSampler.imageView = this->poTextureImageView;
        this->poTextureImageInfo_NoSampler.sampler = nullptr;
    }   
    void VKTexture::UpdateTexture()
    {
        if (this->typeTexture == F_Texture_3D)
        {
            updateNoiseTexture();
        }
    }
    void VKTexture::updateNoiseTextureData()
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

                    float n = 20.0f * (float)(modulePerlin.GetValue(nx, ny, nz));
                    n = n - floor(n);
                    this->pDataRGBA[x + y * this->width + z * this->width * this->height] = static_cast<uint8>(floor(n * 255));
                }
            }
        }
    }
    void VKTexture::updateNoiseTexture()
    {
		VulkanWindow* pWindow = Base::GetWindowPtr();

        //1> updateNoiseTextureData
        updateNoiseTextureData();

        //2> MapData to stagingBuffer
        VkDeviceSize bufSize = this->width * this->height * this->depth;
        pWindow->updateVKBuffer(0, bufSize, (void*)this->pDataRGBA, this->stagingBufferMemory);

        //3> CopyToImage
        VkCommandBuffer cmdBuffer = pWindow->beginSingleTimeCommands();
        {   
            pWindow->copyBufferToImage(cmdBuffer,
									   this->stagingBuffer, 
									   this->poTextureImage, 
									   static_cast<uint32_t>(this->width), 
									   static_cast<uint32_t>(this->height),
									   static_cast<uint32_t>(this->depth), 
									   1);
        }
        pWindow->endSingleTimeCommands(cmdBuffer);
    }

}; //LostPeterVulkan