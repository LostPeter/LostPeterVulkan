/****************************************************************************
* LostPeterPluginRHIVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-15
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _RHI_VULKAN_CONVERTER_H_
#define _RHI_VULKAN_CONVERTER_H_

#include "RHIVulkanPreDefine.h"

namespace LostPeterPluginRHIVulkan
{
    class RHIVulkanConverter 
    {
    public:
        ////////////////////// TransformFromXXXX //////////////////////
        static RHIExtent<3> TransformFromVkExtent3D(const VkExtent3D& vkExtent3D);
        static RHIPhysicalDeviceType TransformFromVkPhysicalDeviceType(VkPhysicalDeviceType vkPhysicalDevice);
        static RHIPixelFormatType TransformFromVkFormat(VkFormat vkFormat);
        static RHIFilterType TransformFromVkFilter(VkFilter vkFilter);
        static RHIFilterType TransformFromVkSamplerMipmapMode(VkSamplerMipmapMode vkSamplerMipmapMode);
        static RHIAddressType TransformFromVkSamplerAddressMode(VkSamplerAddressMode vkSamplerAddressMode);
        static RHIBorderColorType TransformFromVkBorderColor(VkBorderColor vkBorderColor);
        static RHIComparisonFuncType TransformFromVkCompareOp(VkCompareOp vkCompareOp);
        static RHITextureDimensionType TransformFromVkImageType(VkImageType vkImageType);
        static RHITextureViewDimensionType TransformFromVkImageViewType(VkImageViewType vkImageViewType);
        
        static RHISampleCountType TransformFromVkSampleCountFlagBits(VkSampleCountFlagBits vkSampleCountFlagBits);


        static RHIBufferUsageBitsType TransformFromVkBufferUsageFlags(VkBufferUsageFlags vkBufferUsageFlags);
        static RHITextureUsageBitsType TransformFromVkImageUsageFlags(VkImageUsageFlags vkImageUsageFlags);

    public:
        ////////////////////// TransformToXXXX ////////////////////////
        static VkExtent3D TransformToVkExtent3D(const RHIExtent<3>& sExtent);
        static VkPhysicalDeviceType TransformToVkPhysicalDeviceType(RHIPhysicalDeviceType ePhysicalDevice);
        static VkFormat TransformToVkFormat(RHIPixelFormatType ePixelFormat);
        static const String& TransformToVkFormatName(RHIPixelFormatType ePixelFormat);
        static VkFilter TransformToVkFilter(RHIFilterType eFilter);
        static VkSamplerMipmapMode TransformToVkSamplerMipmapMode(RHIFilterType eFilter);
        static VkSamplerAddressMode TransformToVkSamplerAddressMode(RHIAddressType eAddress);
        static VkBorderColor TransformToVkBorderColor(RHIBorderColorType eBorderColor);
        static VkCompareOp TransformToVkCompareOp(RHIComparisonFuncType eComparisonFunc);
        static VkImageType TransformToVkImageType(RHITextureDimensionType eTextureDimension);
        static VkImageViewType TransformToVkImageViewType(RHITextureViewDimensionType eTextureViewDimension);
        
        static VkSampleCountFlagBits TransformToVkSampleCountFlagBits(RHISampleCountType eSampleCount);
        

        static VkBufferUsageFlags TransformToVkBufferUsageFlags(RHIBufferUsageBitsType eBufferUsageBits);
        static VkBufferUsageFlags TransformToVkBufferUsageFlagsFromBufferUsageFlags(RHIBufferUsageFlags flagsBufferUsages);
        static VkImageUsageFlags TransformToVkImageUsageFlags(RHITextureUsageBitsType eTextureUsageBits);
        static VkImageUsageFlags TransformToVkImageUsageFlagsFromTextureUsageFlags(RHITextureUsageFlags flagsTextureUsages);
    };

}; //LostPeterPluginRHIVulkan

#endif