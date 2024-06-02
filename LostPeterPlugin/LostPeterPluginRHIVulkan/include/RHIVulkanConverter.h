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
        static RHILoadOpType TransformFromVkAttachmentLoadOp(VkAttachmentLoadOp vkAttachmentLoadOp); 
        static RHIStoreOpType TransformFromVkAttachmentStoreOp(VkAttachmentStoreOp vkAttachmentStoreOp);

        static RHIBufferUsageBitsType TransformFromVkBufferUsageFlags(VkBufferUsageFlags vkBufferUsageFlags);
        static RHITextureUsageBitsType TransformFromVkImageUsageFlags(VkImageUsageFlags vkImageUsageFlags);
        static RHIShaderStageBitsType TransformFromVkShaderStageFlags(VkShaderStageFlags vkShaderStageFlags);
        static RHIColorWriteBitsType TransformFromVkColorComponentFlags(VkColorComponentFlags vkColorComponentFlags);
        static RHIPipelineStageBitsType TransformFromVkPipelineStageFlags(VkPipelineStageFlags vkPipelineStageFlags);
        static RHIAccessBitsType TransformFromVkAccessFlags(VkAccessFlags vkAccessFlags);
        static RHIDependencyBitsType TransformFromVkDependencyFlags(VkDependencyFlags vkDependencyFlags);

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
        static VkAttachmentLoadOp TransformToVkAttachmentLoadOp(RHILoadOpType eLoadOp); 
        static VkAttachmentStoreOp TransformToVkAttachmentStoreOp(RHIStoreOpType eStoreOp);
        
        static VkBufferUsageFlags TransformToVkBufferUsageFlags(RHIBufferUsageBitsType eBufferUsageBits);
        static VkBufferUsageFlags TransformToVkBufferUsageFlagsFromBufferUsageFlags(RHIBufferUsageFlags flagsBufferUsages);
        static VkImageUsageFlags TransformToVkImageUsageFlags(RHITextureUsageBitsType eTextureUsageBits);
        static VkImageUsageFlags TransformToVkImageUsageFlagsFromTextureUsageFlags(RHITextureUsageFlags flagsTextureUsages);
        static VkShaderStageFlags TransformToVkShaderStageFlags(RHIShaderStageBitsType eShaderStageBits);
        static VkShaderStageFlagBits TransformToVkShaderStageFlagBits(RHIShaderStageBitsType eShaderStageBits);
        static VkShaderStageFlags TransformToVkShaderStageFlagsFromShaderStagelags(RHIShaderStageFlags flagsShaderStages);
        static VkColorComponentFlags TransformToVkColorComponentFlags(RHIColorWriteBitsType eColorWriteBits);
        static VkColorComponentFlags TransformToVkColorComponentFlagsFromColorWriteFlags(RHIColorWriteFlags flagsColorWrite);
        static VkPipelineStageFlags TransformToVkPipelineStageFlags(RHIPipelineStageBitsType ePipelineStageBits);
        static VkPipelineStageFlags TransformToVkPipelineStageFlagsFromPipelineStageFlags(RHIPipelineStageFlags flagsPipelineStage);
        static VkAccessFlags TransformToVkAccessFlags(RHIAccessBitsType eAccessBits);
        static VkAccessFlags TransformToVkAccessFlagsFromAccessFlags(RHIAccessFlags flagsAccess);
        static VkDependencyFlags TransformToVkDependencyFlags(RHIDependencyBitsType eDependencyBits);
        static VkDependencyFlags TransformToVkDependencyFlagsFromPipelineStageFlags(RHIDependencyFlags flagsDependency);

        //0> VkPipelineShaderStageCreateInfo
        static VkPipelineShaderStageCreateInfo TransformToVkPipelineShaderStageCreateInfo(VkShaderStageFlagBits stage,
                                                                                          VkShaderModule module,
                                                                                          const char* pName,
                                                                                          const VkSpecializationInfo* pSpecializationInfo);
        static VkPipelineShaderStageCreateInfo TransformToVkPipelineShaderStageCreateInfo(RHIVulkanShaderModule* pShader, 
                                                                                          RHIShaderStageBitsType eShaderStageBits);
        static void TransformToVkPipelineShaderStageCreateInfo(RHIVulkanShaderModule* pShader, 
                                                               RHIShaderStageBitsType eShaderStageBits,
                                                               VkPipelineShaderStageCreateInfo& createInfo);
        static void TransformToVkPipelineShaderStageCreateInfoVector(VkPipelineShaderStageCreateInfoVector& aShaderStageCreateInfos,
                                                                     RHIVulkanShaderModule* pShaderVertex, 
                                                                     RHIVulkanShaderModule* pShaderPixel, 
                                                                     RHIVulkanShaderModule* pShaderGeometry, 
                                                                     RHIVulkanShaderModule* pShaderDomain, 
                                                                     RHIVulkanShaderModule* pShaderHull);

        //1> VkPipelineVertexInputStateCreateInfo
        static VkPipelineVertexInputStateCreateInfo TransformToVkPipelineVertexInputStateCreateInfo(VkVertexInputBindingDescriptionVector* pBindingDescriptions,
                                                                                                    VkVertexInputAttributeDescriptionVector* pAttributeDescriptions);                                                   
        static void TransformToVkPipelineVertexInputStateCreateInfo(VkVertexInputBindingDescriptionVector* pBindingDescriptions,
                                                                    VkVertexInputAttributeDescriptionVector* pAttributeDescriptions, 
                                                                    VkPipelineVertexInputStateCreateInfo& createInfo);

        //2> VkPipelineInputAssemblyStateCreateInfo
        static VkPipelineInputAssemblyStateCreateInfo TransformToVkPipelineInputAssemblyStateCreateInfo(VkPrimitiveTopology topology, VkBool32 primitiveRestartEnable);
        static VkPipelineInputAssemblyStateCreateInfo TransformToVkPipelineInputAssemblyStateCreateInfo(RHIPrimitiveTopologyType ePrimitiveTopology, bool bPrimitiveRestartEnable);
        static void TransformToVkPipelineInputAssemblyStateCreateInfo(RHIPrimitiveTopologyType ePrimitiveTopology, bool bPrimitiveRestartEnable, VkPipelineInputAssemblyStateCreateInfo& createInfo);
        
        //3> VkPipelineViewportStateCreateInfo
        static VkPipelineViewportStateCreateInfo TransformToVkPipelineViewportStateCreateInfo(const VkViewportVector& aViewports, const VkRect2DVector& aScissors);
        static void TransformToVkPipelineViewportStateCreateInfo(const VkViewportVector& aViewports, const VkRect2DVector& aScissors, VkPipelineViewportStateCreateInfo& createInfo); 
        
        //4> VkPipelineRasterizationStateCreateInfo
        static VkPipelineRasterizationStateCreateInfo TransformToVkPipelineRasterizationStateCreateInfo(VkBool32 depthClampEnable,
                                                                                                        VkBool32 rasterizerDiscardEnable,
                                                                                                        VkPolygonMode polygonMode,
                                                                                                        VkCullModeFlags cullMode,
                                                                                                        VkFrontFace frontFace,
                                                                                                        VkBool32 depthBiasEnable,
                                                                                                        float depthBiasConstantFactor,
                                                                                                        float depthBiasClamp,
                                                                                                        float depthBiasSlopeFactor,
                                                                                                        float lineWidth);
        static void TransformToVkPipelineRasterizationStateCreateInfo(VkBool32 depthClampEnable,
                                                                      VkBool32 rasterizerDiscardEnable,
                                                                      VkPolygonMode polygonMode,
                                                                      VkCullModeFlags cullMode,
                                                                      VkFrontFace frontFace,
                                                                      VkBool32 depthBiasEnable,
                                                                      float depthBiasConstantFactor,
                                                                      float depthBiasClamp,
                                                                      float depthBiasSlopeFactor,
                                                                      float lineWidth,
                                                                      VkPipelineRasterizationStateCreateInfo& createInfo); 
        static VkPipelineRasterizationStateCreateInfo TransformToVkPipelineRasterizationStateCreateInfo(const RHIPrimitiveState& statePrimitive, 
                                                                                                        const RHIDepthStencilState& stateDepthStencil);
        
        //5> VkPipelineMultisampleStateCreateInfo
        static VkPipelineMultisampleStateCreateInfo TransformToVkPipelineMultisampleStateCreateInfo(VkSampleCountFlagBits rasterizationSamples,
                                                                                                    VkBool32 alphaToCoverageEnable,
                                                                                                    VkSampleMask* pSampleMask);
        static void TransformToVkPipelineMultisampleStateCreateInfo(VkSampleCountFlagBits rasterizationSamples,
                                                                    VkBool32 alphaToCoverageEnable,
                                                                    VkSampleMask* pSampleMask,
                                                                    VkPipelineMultisampleStateCreateInfo& createInfo);
        static VkPipelineMultisampleStateCreateInfo TransformToVkPipelineMultisampleStateCreateInfo(const RHIMultiSampleState& state);
        
        //6> VkPipelineDepthStencilStateCreateInfo
        static VkStencilOpState TransformToVkStencilOpState(VkStencilOp failOp,
                                                            VkStencilOp passOp,
                                                            VkStencilOp depthFailOp,
                                                            VkCompareOp compareOp,
                                                            uint32_t compareMask,
                                                            uint32_t writeMask,
                                                            uint32_t reference);
        static VkStencilOpState TransformToVkStencilOpState(const RHIStencilFaceState& stencilFace);
        static VkPipelineDepthStencilStateCreateInfo TransformToVkPipelineDepthStencilStateCreateInfo(VkBool32 depthTestEnable,
                                                                                                      VkBool32 depthWriteEnable,
                                                                                                      VkCompareOp depthCompareOp,
                                                                                                      VkBool32 depthBoundsTestEnable,
                                                                                                      VkBool32 stencilTestEnable,
                                                                                                      const VkStencilOpState& front,
                                                                                                      const VkStencilOpState& back,
                                                                                                      float minDepthBounds,
                                                                                                      float maxDepthBounds);
        static void TransformToVkPipelineDepthStencilStateCreateInfo(VkBool32 depthTestEnable,
                                                                     VkBool32 depthWriteEnable,
                                                                     VkCompareOp depthCompareOp,
                                                                     VkBool32 depthBoundsTestEnable,
                                                                     VkBool32 stencilTestEnable,
                                                                     const VkStencilOpState& front,
                                                                     const VkStencilOpState& back,
                                                                     float minDepthBounds,
                                                                     float maxDepthBounds,
                                                                     VkPipelineDepthStencilStateCreateInfo& createInfo); 
        static VkPipelineDepthStencilStateCreateInfo TransformToVkPipelineDepthStencilStateCreateInfo(const RHIDepthStencilState& state);
        
        //7> VkPipelineColorBlendAttachmentState/VkPipelineColorBlendStateCreateInfo
        static VkPipelineColorBlendAttachmentState TransformToVkPipelineColorBlendAttachmentState(VkBool32 blendEnable,
                                                                                                  VkBlendFactor srcColorBlendFactor,
                                                                                                  VkBlendFactor dstColorBlendFactor,
                                                                                                  VkBlendOp colorBlendOp,
                                                                                                  VkBlendFactor srcAlphaBlendFactor,
                                                                                                  VkBlendFactor dstAlphaBlendFactor,
                                                                                                  VkBlendOp alphaBlendOp,
                                                                                                  VkColorComponentFlags colorWriteMask);
        static void TransformToVkPipelineColorBlendAttachmentState(VkBool32 blendEnable,
                                                                   VkBlendFactor srcColorBlendFactor,
                                                                   VkBlendFactor dstColorBlendFactor,
                                                                   VkBlendOp colorBlendOp,
                                                                   VkBlendFactor srcAlphaBlendFactor,
                                                                   VkBlendFactor dstAlphaBlendFactor,
                                                                   VkBlendOp alphaBlendOp,
                                                                   VkColorComponentFlags colorWriteMask,
                                                                   VkPipelineColorBlendAttachmentState& createInfo);
        static VkPipelineColorBlendAttachmentState TransformToVkPipelineColorBlendAttachmentState(const RHIColorTargetState& state);

        static VkPipelineColorBlendStateCreateInfo TransformToVkPipelineColorBlendStateCreateInfo(const VkPipelineColorBlendAttachmentStateVector& aState);
        static void TransformToVkPipelineColorBlendStateCreateInfo(const VkPipelineColorBlendAttachmentStateVector& aState, VkPipelineColorBlendStateCreateInfo& createInfo); 
        static VkPipelineColorBlendStateCreateInfo TransformToVkPipelineColorBlendStateCreateInfo(const RHIColorTargetStateVector& aColorTargetState); 

        //8> VkPipelineDynamicStateCreateInfo
        static VkPipelineDynamicStateCreateInfo TransformToVkPipelineDynamicStateCreateInfo(const VkDynamicStateVector& aState);
        static void TransformToVkPipelineDynamicStateCreateInfo(const VkDynamicStateVector& aState,
                                                                VkPipelineDynamicStateCreateInfo& createInfo); 
        
        //9> VkPipelineTessellationStateCreateInfo
        static VkPipelineTessellationStateCreateInfo TransformToVkPipelineTessellationStateCreateInfo(VkPipelineTessellationStateCreateFlags flags,
                                                                                                      uint32_t patchControlPoints);
        static void TransformToVkPipelineTessellationStateCreateInfo(VkPipelineTessellationStateCreateFlags flags,
                                                                     uint32_t patchControlPoints,
                                                                     VkPipelineTessellationStateCreateInfo& createInfo); 
        
        //10> VkGraphicsPipelineCreateInfo
        static VkGraphicsPipelineCreateInfo TransformToVkGraphicsPipelineCreateInfo(VkPipelineCreateFlags flags,
                                                                                    uint32_t stageCount,
                                                                                    const VkPipelineShaderStageCreateInfo* pStages,
                                                                                    const VkPipelineVertexInputStateCreateInfo* pVertexInputState,
                                                                                    const VkPipelineInputAssemblyStateCreateInfo* pInputAssemblyState,
                                                                                    const VkPipelineTessellationStateCreateInfo* pTessellationState,
                                                                                    const VkPipelineViewportStateCreateInfo* pViewportState,
                                                                                    const VkPipelineRasterizationStateCreateInfo* pRasterizationState,
                                                                                    const VkPipelineMultisampleStateCreateInfo* pMultisampleState,
                                                                                    const VkPipelineDepthStencilStateCreateInfo* pDepthStencilState,
                                                                                    const VkPipelineColorBlendStateCreateInfo* pColorBlendState,
                                                                                    const VkPipelineDynamicStateCreateInfo* pDynamicState,
                                                                                    VkPipelineLayout layout,
                                                                                    VkRenderPass renderPass,
                                                                                    uint32_t subpass,
                                                                                    VkPipeline basePipelineHandle,
                                                                                    int32_t basePipelineIndex);
        static void TransformToVkGraphicsPipelineCreateInfo(VkPipelineCreateFlags flags,
                                                            uint32_t stageCount,
                                                            const VkPipelineShaderStageCreateInfo* pStages,
                                                            const VkPipelineVertexInputStateCreateInfo* pVertexInputState,
                                                            const VkPipelineInputAssemblyStateCreateInfo* pInputAssemblyState,
                                                            const VkPipelineTessellationStateCreateInfo* pTessellationState,
                                                            const VkPipelineViewportStateCreateInfo* pViewportState,
                                                            const VkPipelineRasterizationStateCreateInfo* pRasterizationState,
                                                            const VkPipelineMultisampleStateCreateInfo* pMultisampleState,
                                                            const VkPipelineDepthStencilStateCreateInfo* pDepthStencilState,
                                                            const VkPipelineColorBlendStateCreateInfo* pColorBlendState,
                                                            const VkPipelineDynamicStateCreateInfo* pDynamicState,
                                                            VkPipelineLayout layout,
                                                            VkRenderPass renderPass,
                                                            uint32_t subpass,
                                                            VkPipeline basePipelineHandle,
                                                            int32_t basePipelineIndex,
                                                            VkGraphicsPipelineCreateInfo& createInfo); 
        
        
        //11> VkComputePipelineCreateInfo
        static VkComputePipelineCreateInfo TransformToVkComputePipelineCreateInfo(const VkPipelineShaderStageCreateInfo& shaderStageCreateInfo,
                                                                                  VkPipelineLayout vkPipelineLayout, 
                                                                                  VkPipelineCreateFlags flags);
        static void TransformToVkComputePipelineCreateInfo(const VkPipelineShaderStageCreateInfo& shaderStageCreateInfo,
                                                           VkPipelineLayout vkPipelineLayout, 
                                                           VkPipelineCreateFlags flags,
                                                           VkComputePipelineCreateInfo& createInfo); 
        
        //12> VkRenderPassCreateInfo/VkAttachmentDescription/
        static VkRenderPassCreateInfo TransformToVkRenderPassCreateInfo(const VkAttachmentDescriptionVector& aAttachmentDescription,
                                                                        const VkSubpassDescriptionVector& aSubpassDescription,
                                                                        const VkSubpassDependencyVector& aSubpassDependency,
                                                                        VkRenderPassMultiviewCreateInfo* pMultiviewCI);
        static void TransformToVkRenderPassCreateInfo(const VkAttachmentDescriptionVector& aAttachmentDescription,
                                                      const VkSubpassDescriptionVector& aSubpassDescription,
                                                      const VkSubpassDependencyVector& aSubpassDependency,
                                                      VkRenderPassMultiviewCreateInfo* pMultiviewCI,
                                                      VkRenderPassCreateInfo& createInfo);
        static VkAttachmentDescription TransformToVkAttachmentDescription(VkAttachmentDescriptionFlags flags,
                                                                          VkFormat typeFormat,
                                                                          VkSampleCountFlagBits samples,
                                                                          VkAttachmentLoadOp loadOp,
                                                                          VkAttachmentStoreOp storeOp,
                                                                          VkAttachmentLoadOp stencilLoadOp,
                                                                          VkAttachmentStoreOp stencilStoreOp,
                                                                          VkImageLayout initialLayout,
                                                                          VkImageLayout finalLayout);
        static VkAttachmentDescription TransformToVkAttachmentDescriptionFromColorAttachment(const RHIGraphicsPassColorAttachment& colorAttachment);
        static VkAttachmentDescription TransformToVkAttachmentDescriptionFromDepthStencilAttachment(const RHIGraphicsPassDepthStencilAttachment& depthStencilAttachment);
        static void TransformToVkAttachmentDescription(VkAttachmentDescriptionVector& aAttachmentDescription,   
                                                       const RHIGraphicsPassColorAttachmentVector& aColorAttachment,
                                                       const RHIGraphicsPassDepthStencilAttachmentVector& aDepthStencilAttachment);

        static VkSubpassDescription TransformToVkSubpassDescription(VkSubpassDescriptionFlags flags,
                                                                    VkPipelineBindPoint pipelineBindPoint,
                                                                    uint32_t inputAttachmentCount,
                                                                    const VkAttachmentReference* pInputAttachments,
                                                                    uint32_t colorAttachmentCount,
                                                                    const VkAttachmentReference* pColorAttachments,
                                                                    const VkAttachmentReference* pResolveAttachments,
                                                                    const VkAttachmentReference* pDepthStencilAttachment,
                                                                    uint32_t preserveAttachmentCount,
                                                                    const uint32_t* pPreserveAttachments);
        static VkSubpassDescription TransformToVkSubpassDescription(const RHIGraphicsSubpassDescription& desc);
        static void TransformToVkSubpassDescription(VkSubpassDescriptionVector& aVkDesc,
                                                    const RHIGraphicsSubpassDescriptionVector& aDesc);

        static VkSubpassDependency TransformToVkSubpassDependency(uint32_t srcSubpass,
                                                                  uint32_t dstSubpass,
                                                                  VkPipelineStageFlags srcStageMask,
                                                                  VkPipelineStageFlags dstStageMask,
                                                                  VkAccessFlags srcAccessMask,
                                                                  VkAccessFlags dstAccessMask,
                                                                  VkDependencyFlags dependencyFlags);
        static VkSubpassDependency TransformToVkSubpassDependency(const RHIGraphicsSubpassDependency& dependency);
        static void TransformToVkSubpassDependency(VkSubpassDependencyVector& aVkDependency,
                                                   const RHIGraphicsSubpassDependencyVector& aDependency);
    };

}; //LostPeterPluginRHIVulkan

#endif