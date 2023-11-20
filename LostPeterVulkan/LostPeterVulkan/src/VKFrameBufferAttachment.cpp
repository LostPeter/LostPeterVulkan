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

#include "../include/VKFrameBufferAttachment.h"
#include "../include/VulkanWindow.h"

namespace LostPeterVulkan
{
    VKFrameBufferAttachment::VKFrameBufferAttachment()
        : isDepth(false)
        , isImageArray(false)
        , image(VK_NULL_HANDLE)
        , memory(VK_NULL_HANDLE)
        , view(VK_NULL_HANDLE)
    {

    }
    VKFrameBufferAttachment::~VKFrameBufferAttachment()
    {

    }
    
    void VKFrameBufferAttachment::Destroy()
    {
        if (this->image != VK_NULL_HANDLE)
        {
            Base::GetWindowPtr()->destroyVkImage(this->image, this->memory, this->view);
        }
        this->image = VK_NULL_HANDLE;
        this->memory = VK_NULL_HANDLE;
        this->view = VK_NULL_HANDLE;
    }

    void VKFrameBufferAttachment::Init(uint32_t width, 
                                       uint32_t height, 
                                       bool _isDepth,
                                       bool _isImageArray)
    {
        this->isDepth = _isDepth;
        this->isImageArray = _isImageArray;

        uint32_t depth = 1;
        uint32_t numArray = 1;
        uint32_t mipMapCount = 1;
        VkImageType imageType = VK_IMAGE_TYPE_2D;
        VkSampleCountFlagBits numSamples = Base::GetWindowPtr()->poMSAASamples;
        VkFormat format = Base::GetWindowPtr()->poSwapChainImageFormat;
        VkImageTiling tiling = VK_IMAGE_TILING_OPTIMAL;
        VkImageUsageFlags usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
        VkSharingMode sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        VkMemoryPropertyFlags properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

        VkImageViewType imageViewType = VK_IMAGE_VIEW_TYPE_2D;
        VkImageAspectFlags aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;

        if (_isDepth)
        {
            format = Base::GetWindowPtr()->poDepthImageFormat;
            usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
            aspectFlags = VK_IMAGE_ASPECT_DEPTH_BIT;
        }

        if (_isImageArray)
        {
            numArray = 2;
            imageViewType = VK_IMAGE_VIEW_TYPE_2D_ARRAY;
        }

        Base::GetWindowPtr()->createVkImage(width, 
                                            height, 
                                            depth,
                                            numArray,
                                            mipMapCount,
                                            imageType, 
                                            false,
                                            numSamples, 
                                            format, 
                                            tiling, 
                                            usage,
                                            sharingMode,
                                            false,
                                            properties, 
                                            this->image, 
                                            this->memory);
        
        Base::GetWindowPtr()->createVkImageView(this->image, 
                                                imageViewType,
                                                format, 
                                                aspectFlags, 
                                                mipMapCount,
                                                numArray,
                                                this->view);
    }

}; //LostPeterVulkan