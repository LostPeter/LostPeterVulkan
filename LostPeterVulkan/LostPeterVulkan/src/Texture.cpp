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

#include "../include/Texture.h"
#include "../include/VulkanWindow.h"

namespace LostPeter
{
    Texture::Texture(const String& _nameTexture,
                     FTextureType _typeTexture,
                     bool _isRenderTarget,
                     bool _isGraphicsComputeShared,
                     VkFormat _typeFormat,
                     FTextureFilterType _typeFilter,
                     FTextureAddressingType _typeAddressing,
                     FTextureBorderColorType _typeBorderColor,
                    const StringVector& _aPathTexture)
        : Base(_nameTexture)
        , typeTexture(_typeTexture)
        , isRenderTarget(_isRenderTarget)
        , isGraphicsComputeShared(_isGraphicsComputeShared)
        , typeFormat(_typeFormat)
        , typeFilter(_typeFilter)
        , typeAddressing(_typeAddressing)
        , typeBorderColor(_typeBorderColor)
        , aPathTexture(_aPathTexture)
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
        if (this->stagingBuffer != VK_NULL_HANDLE)
        {
            Base::GetWindowPtr()->destroyVkBuffer(this->stagingBuffer, this->stagingBufferMemory);
        }
        Base::GetWindowPtr()->destroyVkImage(this->poTextureImage, this->poTextureImageMemory, this->poTextureImageView);
        this->poTextureImage = VK_NULL_HANDLE;
        this->poTextureImageMemory = VK_NULL_HANDLE;
        this->poTextureImageView = VK_NULL_HANDLE;
        Base::GetWindowPtr()->destroyVkImageSampler(this->poTextureSampler);
        this->poTextureSampler = VK_NULL_HANDLE;
        F_DELETE_T(pDataRGBA)
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
        this->width = width;
        this->height = height;
        this->depth = depth;

        if (!this->isRenderTarget)
        {
            if (this->typeTexture == F_Texture_1D)
            {
                Base::GetWindowPtr()->createTexture1D(this->aPathTexture[0], 
                                                      this->poMipMapCount, 
                                                      this->poTextureImage, 
                                                      this->poTextureImageMemory);
                Base::GetWindowPtr()->createVkImageView(this->poTextureImage, 
                                                        VK_IMAGE_VIEW_TYPE_1D, 
                                                        this->typeFormat, 
                                                        VK_IMAGE_ASPECT_COLOR_BIT, 
                                                        this->poMipMapCount, 
                                                        1, 
                                                        this->poTextureImageView);
            }
            else if (this->typeTexture == F_Texture_2D)
            {
                Base::GetWindowPtr()->createTexture2D(this->aPathTexture[0], 
                                                      VK_IMAGE_TYPE_2D, 
                                                      VK_SAMPLE_COUNT_1_BIT, 
                                                      this->typeFormat, true, 
                                                      this->poMipMapCount, 
                                                      this->poTextureImage, 
                                                      this->poTextureImageMemory);
                Base::GetWindowPtr()->createVkImageView(this->poTextureImage, 
                                                        VK_IMAGE_VIEW_TYPE_2D, 
                                                        this->typeFormat, 
                                                        VK_IMAGE_ASPECT_COLOR_BIT, 
                                                        this->poMipMapCount, 
                                                        1, 
                                                        this->poTextureImageView);
            }
            else if (this->typeTexture == F_Texture_2DArray)
            {
                Base::GetWindowPtr()->createTexture2DArray(this->aPathTexture, 
                                                           VK_IMAGE_TYPE_2D,
                                                           VK_SAMPLE_COUNT_1_BIT, 
                                                           this->typeFormat, 
                                                           true, 
                                                           this->poMipMapCount, 
                                                           this->poTextureImage, 
                                                           this->poTextureImageMemory);
                Base::GetWindowPtr()->createVkImageView(this->poTextureImage, 
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
                Base::GetWindowPtr()->createTexture3D(this->typeFormat, 
                                                      this->pDataRGBA, 
                                                      size, 
                                                      width, 
                                                      height, 
                                                      depth, 
                                                      this->poTextureImage, 
                                                      this->poTextureImageMemory, 
                                                      this->stagingBuffer, 
                                                      this->stagingBufferMemory);
                Base::GetWindowPtr()->createVkImageView(this->poTextureImage, 
                                                        VK_IMAGE_VIEW_TYPE_3D, 
                                                        this->typeFormat, 
                                                        VK_IMAGE_ASPECT_COLOR_BIT, 
                                                        this->poMipMapCount, 
                                                        1, 
                                                        this->poTextureImageView);
            }
            else if (this->typeTexture == F_Texture_CubeMap)
            {
                Base::GetWindowPtr()->createTextureCubeMap(this->aPathTexture, 
                                                           this->poMipMapCount, 
                                                           this->poTextureImage, 
                                                           this->poTextureImageMemory);
                Base::GetWindowPtr()->createVkImageView(this->poTextureImage, 
                                                        VK_IMAGE_VIEW_TYPE_CUBE, 
                                                        this->typeFormat, 
                                                        VK_IMAGE_ASPECT_COLOR_BIT, 
                                                        this->poMipMapCount, 
                                                        (int)this->aPathTexture.size(), 
                                                        this->poTextureImageView);
            }   
            else
            {
                String msg = "*********************** Texture::LoadTexture: Wrong texture type, Create from file, name: [" + this->name + "] !";
                F_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }
        }
        else
        {
            if (this->typeTexture == F_Texture_1D)
            {
                Base::GetWindowPtr()->createTextureRenderTarget1D(this->rtColorDefault, 
                                                                  this->rtIsSetColor, 
                                                                  this->width, 
                                                                  this->poMipMapCount, 
                                                                  VK_SAMPLE_COUNT_1_BIT, 
                                                                  this->typeFormat, 
                                                                  this->rtImageUsage,
                                                                  this->isGraphicsComputeShared,
                                                                  this->poTextureImage, 
                                                                  this->poTextureImageMemory);
                Base::GetWindowPtr()->createVkImageView(this->poTextureImage, 
                                                        VK_IMAGE_VIEW_TYPE_1D, 
                                                        this->typeFormat, 
                                                        VK_IMAGE_ASPECT_COLOR_BIT, 
                                                        this->poMipMapCount, 
                                                        1, 
                                                        this->poTextureImageView);
            } 
            else if (this->typeTexture == F_Texture_2D)
            {
                Base::GetWindowPtr()->createTextureRenderTarget2D(this->rtColorDefault, 
                                                                  this->rtIsSetColor, 
                                                                  this->width, 
                                                                  this->height,
                                                                  this->poMipMapCount, 
                                                                  VK_SAMPLE_COUNT_1_BIT, 
                                                                  this->typeFormat, 
                                                                  this->rtImageUsage,
                                                                  this->isGraphicsComputeShared,
                                                                  this->poTextureImage, 
                                                                  this->poTextureImageMemory);
                Base::GetWindowPtr()->createVkImageView(this->poTextureImage, 
                                                        VK_IMAGE_VIEW_TYPE_2D, 
                                                        this->typeFormat, 
                                                        VK_IMAGE_ASPECT_COLOR_BIT, 
                                                        this->poMipMapCount, 
                                                        1, 
                                                        this->poTextureImageView);
            }
            else if (this->typeTexture == F_Texture_2DArray)
            {
                Base::GetWindowPtr()->createTextureRenderTarget2DArray(this->rtColorDefault, 
                                                                       this->rtIsSetColor, 
                                                                       this->width, 
                                                                       this->height,
                                                                       this->poMipMapCount, 
                                                                       VK_IMAGE_TYPE_2D,
                                                                       VK_SAMPLE_COUNT_1_BIT, 
                                                                       this->typeFormat, 
                                                                       this->rtImageUsage,
                                                                       this->isGraphicsComputeShared,
                                                                       this->poTextureImage, 
                                                                       this->poTextureImageMemory);
                Base::GetWindowPtr()->createVkImageView(this->poTextureImage, 
                                                        VK_IMAGE_VIEW_TYPE_2D_ARRAY, 
                                                        this->typeFormat, 
                                                        VK_IMAGE_ASPECT_COLOR_BIT, 
                                                        this->poMipMapCount, 
                                                        (int)this->aPathTexture.size(), 
                                                        this->poTextureImageView);
            }
            else if (this->typeTexture == F_Texture_3D)
            {
                Base::GetWindowPtr()->createTextureRenderTarget3D(this->rtColorDefault, 
                                                                  this->rtIsSetColor, 
                                                                  this->width, 
                                                                  this->height,
                                                                  this->depth,
                                                                  this->poMipMapCount, 
                                                                  VK_SAMPLE_COUNT_1_BIT,
                                                                  this->typeFormat, 
                                                                  this->rtImageUsage,
                                                                  this->isGraphicsComputeShared,
                                                                  this->poTextureImage, 
                                                                  this->poTextureImageMemory);
                Base::GetWindowPtr()->createVkImageView(this->poTextureImage, 
                                                        VK_IMAGE_VIEW_TYPE_3D, 
                                                        this->typeFormat, 
                                                        VK_IMAGE_ASPECT_COLOR_BIT, 
                                                        this->poMipMapCount, 
                                                        1, 
                                                        this->poTextureImageView);
            }
            else if (this->typeTexture == F_Texture_CubeMap)
            {
                Base::GetWindowPtr()->createTextureRenderTargetCubeMap(this->width, 
                                                                       this->height,
                                                                       this->poMipMapCount, 
                                                                       VK_SAMPLE_COUNT_1_BIT,
                                                                       this->typeFormat, 
                                                                       this->rtImageUsage,
                                                                       this->isGraphicsComputeShared,
                                                                       this->poTextureImage, 
                                                                       this->poTextureImageMemory);
                Base::GetWindowPtr()->createVkImageView(this->poTextureImage, 
                                                        VK_IMAGE_VIEW_TYPE_CUBE, 
                                                        this->typeFormat, 
                                                        VK_IMAGE_ASPECT_COLOR_BIT, 
                                                        this->poMipMapCount, 
                                                        6, 
                                                        this->poTextureImageView);
            }
            else
            {
                String msg = "*********************** Texture::LoadTexture: Wrong texture type, Create render target, name: [" + this->name + "] !";
                F_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }
        }

        Base::GetWindowPtr()->createVkSampler(this->typeFilter, 
                                              this->typeAddressing,
                                              this->typeBorderColor,
                                              true,
                                              Base::GetWindowPtr()->poPhysicalDeviceProperties.limits.maxSamplerAnisotropy,
                                              0.0f,
                                              static_cast<float>(this->poMipMapCount),
                                              0.0f,
                                              this->poTextureSampler);

        this->poTextureImageInfo = {};
        this->poTextureImageInfo.imageLayout = this->poTextureImageLayout;
        this->poTextureImageInfo.imageView = this->poTextureImageView;
        this->poTextureImageInfo.sampler = this->poTextureSampler;
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
        //1> updateNoiseTextureData
        updateNoiseTextureData();

        //2> MapData to stagingBuffer
        VkDeviceSize bufSize = this->width * this->height * this->depth;
        Base::GetWindowPtr()->updateVKBuffer(0, bufSize, (void*)this->pDataRGBA, this->stagingBufferMemory);

        //3> CopyToImage
        VkCommandBuffer cmdBuffer = Base::GetWindowPtr()->beginSingleTimeCommands();
        {   
            Base::GetWindowPtr()->copyBufferToImage(cmdBuffer,
                                                    this->stagingBuffer, 
                                                    this->poTextureImage, 
                                                    static_cast<uint32_t>(this->width), 
                                                    static_cast<uint32_t>(this->height),
                                                    static_cast<uint32_t>(this->depth), 
                                                    1);
        }
        Base::GetWindowPtr()->endSingleTimeCommands(cmdBuffer);
    }

}; //LostPeter