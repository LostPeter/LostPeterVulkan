/****************************************************************************
* LostPeterPluginRendererVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-11-23
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VULKAN_CONVERTER_H_
#define _VULKAN_CONVERTER_H_

#include "VulkanPreDefine.h"

namespace LostPeterPluginRendererVulkan
{
    class VulkanConverter 
    {
    public:
        static VkPrimitiveTopology Transform2VkPrimitiveTopology(FRenderPrimitiveType eRenderPrimitive);
        static VkCullModeFlags Transform2VkCullModeFlags(FCullingType eCulling);
        static VkCompareOp Transform2VkCompareOp(FCompareFuncType eCompareFunc);
        static VkPolygonMode Transform2VkPolygonMode(FPolygonType ePolygon);
        static VkBlendOp Transform2VkBlendOp(FSceneBlendingOPType eSceneBlendingOP);
        static VkBlendFactor Transform2VkBlendFactor(FSceneBlendingFactorType eSceneBlendingFactor);
        static VkStencilOp Transform2VkStencilOp(FStencilOPType eStencilOP);

        static VkImageType Transform2VkImageType(FTextureType eTexture);
        static VkImageViewType Transform2VkImageViewType(FTextureType eTexture);
        static VkFilter Transform2VkFilter(FTextureFilterPixelType eTextureFilterPixel);
        static VkFilter Transform2VkFilter(FTextureFilterType eTextureFilter, FTextureFilterSizeType eTextureFilterSize);
        static VkSamplerMipmapMode Transform2VkSamplerMipmapMode(FTextureFilterPixelType eTextureFilterPixel);
        static VkSamplerMipmapMode Transform2VkSamplerMipmapMode(FTextureFilterType eTextureFilter);
        static VkSamplerAddressMode Transform2VkSamplerAddressMode(FTextureAddressingType eTextureAddressing);
        static VkBorderColor Transform2VkBorderColor(FTextureBorderColorType eTextureBorderColor);
        static VkSampleCountFlagBits Transform2VkSampleCountFlagBits(FMSAASampleCountType eMSAASampleCount);

        static VkShaderStageFlagBits Transform2VkShaderStageFlagBits(FShaderType eShader);    
        static VkShaderStageFlagBits Transform2VkShaderStageFlagBits(const Uint32Vector& aShaderTypes);    

        static VkFormat Transform2VkFormat(FPixelFormatType ePixelFormat);
        static VkComponentMapping Transform2VkComponentMapping(FPixelFormatType ePixelFormat);
        static VkImageAspectFlags Transform2VkImageAspectFlags(FPixelFormatType ePixelFormat, const bool bPreferDepthOverStencil = false);
    };

}; //LostPeterPluginRendererVulkan

#endif