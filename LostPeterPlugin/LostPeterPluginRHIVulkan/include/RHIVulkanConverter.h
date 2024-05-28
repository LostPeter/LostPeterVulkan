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
        static RHIExtent<2> TransformFromVkExtent2D(const VkExtent2D& vkExtent2D);
        static RHIExtent<3> TransformFromVkExtent3D(const VkExtent3D& vkExtent3D);
        static RHIPhysicalDeviceType TransformFromVkPhysicalDeviceType(VkPhysicalDeviceType vkPhysicalDevice);
        static RHIPixelFormatType TransformFromVkFormat(VkFormat vkFormat);
        static RHIColorSpaceType TransformFromVkColorSpaceKHR(VkColorSpaceKHR vkColorSpaceKHR);
        static RHIPrimitiveTopologyType TransformFromVkPrimitiveTopology(VkPrimitiveTopology vkPrimitiveTopology);
        static RHIFrontFaceType TransformFromVkFrontFace(VkFrontFace vkFrontFace);
        static RHICullType TransformFromVkCullModeFlagBits(VkCullModeFlagBits vkCullModeFlagBits);
        static RHIPolygonType TransformFromVkPolygonMode(VkPolygonMode vkPolygonMode);
        static RHIComparisonFuncType TransformFromVkCompareOp(VkCompareOp vkCompareOp);
        static RHIBlendOpType TransformFromVkBlendOp(VkBlendOp vkBlendOp);
        static RHIBlendFactorType TransformFromVkBlendFactor(VkBlendFactor vkBlendFactor);
        static RHIStencilOpType TransformFromVkStencilOp(VkStencilOp vkStencilOp);

        static RHIFilterType TransformFromVkFilter(VkFilter vkFilter);
        static RHIFilterType TransformFromVkSamplerMipmapMode(VkSamplerMipmapMode vkSamplerMipmapMode);
        static RHIAddressType TransformFromVkSamplerAddressMode(VkSamplerAddressMode vkSamplerAddressMode);
        static RHIBorderColorType TransformFromVkBorderColor(VkBorderColor vkBorderColor);
        static RHITextureDimensionType TransformFromVkImageType(VkImageType vkImageType);
        static RHITextureViewDimensionType TransformFromVkImageViewType(VkImageViewType vkImageViewType);
        static RHISampleCountType TransformFromVkSampleCountFlagBits(VkSampleCountFlagBits vkSampleCountFlagBits);
        static RHIPresentType TransformFromVkPresentModeKHR(VkPresentModeKHR vkPresentModeKHR);
        static RHIBindingType TransformFromVkDescriptorType(VkDescriptorType vkDescriptorType);
        static RHITextureStateType TransformFromVkImageLayout(VkImageLayout vkImageLayout); 

        static RHIBufferUsageBitsType TransformFromVkBufferUsageFlags(VkBufferUsageFlags vkBufferUsageFlags);
        static RHITextureUsageBitsType TransformFromVkImageUsageFlags(VkImageUsageFlags vkImageUsageFlags);
        static RHIShaderStageBitsType TransformFromVkShaderStageFlags(VkShaderStageFlags vkShaderStageFlags);

    public:
        ////////////////////// TransformToXXXX ////////////////////////
        static VkExtent2D TransformToVkExtent2D(const RHIExtent<2>& sExtent);
        static VkExtent3D TransformToVkExtent3D(const RHIExtent<3>& sExtent);
        static VkPhysicalDeviceType TransformToVkPhysicalDeviceType(RHIPhysicalDeviceType ePhysicalDevice);
        static VkFormat TransformToVkFormat(RHIPixelFormatType ePixelFormat);
        static const String& TransformToVkFormatName(RHIPixelFormatType ePixelFormat);
        static VkColorSpaceKHR TransformToVkColorSpaceKHR(RHIColorSpaceType eColorSpace);
        static VkPrimitiveTopology TransformToVkPrimitiveTopology(RHIPrimitiveTopologyType ePrimitiveTopology);
        static VkFrontFace TransformToVkFrontFace(RHIFrontFaceType eFrontFace); 
        static VkCullModeFlagBits TransformToVkCullModeFlagBits(RHICullType eCull);
        static VkPolygonMode TransformToVkPolygonMode(RHIPolygonType ePolygon);
        static VkCompareOp TransformToVkCompareOp(RHIComparisonFuncType eComparisonFunc);
        static VkBlendOp TransformToVkBlendOp(RHIBlendOpType eBlendOp);
        static VkBlendFactor TransformToVkBlendFactor(RHIBlendFactorType eBlendFactor);
        static VkStencilOp TransformToVkStencilOp(RHIStencilOpType eStencilOP);

        static VkFilter TransformToVkFilter(RHIFilterType eFilter);
        static VkSamplerMipmapMode TransformToVkSamplerMipmapMode(RHIFilterType eFilter);
        static VkSamplerAddressMode TransformToVkSamplerAddressMode(RHIAddressType eAddress);
        static VkBorderColor TransformToVkBorderColor(RHIBorderColorType eBorderColor);
        static VkImageType TransformToVkImageType(RHITextureDimensionType eTextureDimension);
        static VkImageViewType TransformToVkImageViewType(RHITextureViewDimensionType eTextureViewDimension);
        static VkSampleCountFlagBits TransformToVkSampleCountFlagBits(RHISampleCountType eSampleCount);
        static VkPresentModeKHR TransformToVkPresentModeKHR(RHIPresentType ePresent);
        static VkDescriptorType TransformToVkDescriptorType(RHIBindingType eBinding);
        static VkImageLayout TransformToVkImageLayout(RHITextureStateType eTextureState); 

        static VkBufferUsageFlags TransformToVkBufferUsageFlags(RHIBufferUsageBitsType eBufferUsageBits);
        static VkBufferUsageFlags TransformToVkBufferUsageFlagsFromBufferUsageFlags(RHIBufferUsageFlags flagsBufferUsages);
        static VkImageUsageFlags TransformToVkImageUsageFlags(RHITextureUsageBitsType eTextureUsageBits);
        static VkImageUsageFlags TransformToVkImageUsageFlagsFromTextureUsageFlags(RHITextureUsageFlags flagsTextureUsages);
        static VkShaderStageFlags TransformToVkShaderStageFlags(RHIShaderStageBitsType eShaderStageBits);
        static VkShaderStageFlags TransformToVkShaderStageFlagsFromShaderStagelags(RHIShaderStageFlags flagsShaderStages);
    };

}; //LostPeterPluginRHIVulkan

#endif