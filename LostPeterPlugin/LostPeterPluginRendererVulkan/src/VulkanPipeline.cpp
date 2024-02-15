/****************************************************************************
* LostPeterPluginRendererVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-02-15
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/VulkanPipeline.h"
#include "../include/VulkanDevice.h"
#include "../include/VulkanRenderWindow.h"
#include "../include/VulkanRenderPass.h"
#include "../include/VulkanShaderModule.h"
#include "../include/VulkanShaderModuleManager.h"
#include "../include/VulkanPipelineCache.h"
#include "../include/VulkanPipelineLayout.h"
#include "../include/VulkanPipelineLayoutManager.h"
#include "../include/VulkanConverter.h"

namespace LostPeterPluginRendererVulkan
{
    VulkanPipeline::VulkanPipeline(const String& namePipeline, VulkanDevice* pDevice)
        : Base(namePipeline)
        , m_pDevice(pDevice)
        , m_vkPipeline(VK_NULL_HANDLE)

        , m_bIsPipelineGraphics(true)
        , m_pPipelineLayout(nullptr)
    {
        F_Assert(m_pDevice && "VulkanPipeline::VulkanPipeline")
    }

    VulkanPipeline::~VulkanPipeline()
    {
        Destroy();
    }

    void VulkanPipeline::Destroy()
    {
        if (m_vkPipeline != VK_NULL_HANDLE)
        {
            m_pDevice->DestroyVkPipeline(m_vkPipeline);
        }
        m_vkPipeline = VK_NULL_HANDLE;

        int count = (int)m_aShaderModule.size();
        for (int i = 0; i < count; i++)
        {
            VulkanShaderModule* pShaderModule = m_aShaderModule[i];
            VulkanShaderModuleManager::GetSingleton().DeleteShaderModule(pShaderModule);
        }   
        m_aShaderModule.clear();

        if (m_pPipelineLayout != nullptr)
        {
            VulkanPipelineLayoutManager::GetSingleton().DeletePipelineLayout(m_pPipelineLayout);
        }
        m_pPipelineLayout = nullptr;
    }

    
    bool VulkanPipeline::InitPipelineGraphics(VulkanRenderWindow* pRenderWindow,
                                              VulkanRenderPass* pRenderPass,
                                              VulkanShaderModulePtrVector& aShaderModule,
                                              VulkanPipelineLayout* pPipelineLayout,
                                              FMeshVertexType eMeshVertex,
                                              FRenderPrimitiveType eRenderPrimitive,
                                              RenderState* pRenderState)
    {
        VkVertexInputBindingDescriptionVector* pBindingDescriptions = E_GetVkVertexInputBindingDescriptionVectorPtr(eMeshVertex);
        VkVertexInputAttributeDescriptionVector* pAttributeDescriptions = E_GetVkVertexInputAttributeDescriptionVectorPtr(eMeshVertex);
        return InitPipelineGraphics(pRenderWindow,
                                    pRenderPass,
                                    aShaderModule,
                                    pPipelineLayout,
                                    pBindingDescriptions,
                                    pAttributeDescriptions,
                                    eRenderPrimitive,
                                    pRenderState);
    }
    bool VulkanPipeline::InitPipelineGraphics(VulkanRenderWindow* pRenderWindow,
                                              VulkanRenderPass* pRenderPass,
                                              VulkanShaderModulePtrVector& aShaderModule,
                                              VulkanPipelineLayout* pPipelineLayout,
                                              VkVertexInputBindingDescriptionVector* pBindingDescriptions,
                                              VkVertexInputAttributeDescriptionVector* pAttributeDescriptions,
                                              FRenderPrimitiveType eRenderPrimitive,
                                              RenderState* pRenderState)
    {
        F_Assert(pRenderWindow && pRenderPass && pPipelineLayout && pBindingDescriptions && pAttributeDescriptions && pRenderState && "VulkanPipeline::InitPipelineCompute")

        VkPipelineShaderStageCreateInfoVector aShaderStageCreateInfos;
        int count = (int)aShaderModule.size();
        for (int i = 0; i < count; i++)
        {
            VulkanShaderModule* pShaderModule = aShaderModule[i];
            aShaderStageCreateInfos.push_back(pShaderModule->GetVkPipelineShaderStageCreateInfo());
        }

        VkPrimitiveTopology primitiveTopology = VulkanConverter::Transform2VkPrimitiveTopology(eRenderPrimitive);
        VkFrontFace frontFace = VulkanConverter::Transform2VkFrontFace(pRenderState->GetFrontFaceType());
        VkPolygonMode polygonMode = VulkanConverter::Transform2VkPolygonMode(pRenderState->GetPolygonType());
        VkCullModeFlagBits cullMode = VulkanConverter::Transform2VkCullModeFlagBits(pRenderState->GetCullingType());
        float lineWidth = pRenderState->GetPointSize();
        VkBool32 bDepthTest = pRenderState->GetDepthTestEnabled() ? VK_TRUE : VK_FALSE;
        VkBool32 bDepthWrite = pRenderState->GetDepthWriteEnabled() ? VK_TRUE : VK_FALSE;
        VkCompareOp depthCompareOp = VulkanConverter::Transform2VkCompareOp(pRenderState->GetDepthCompareFunc());
        VkBool32 bStencilTest = pRenderState->GetStencilEnabled() ? VK_TRUE : VK_FALSE;
        VkStencilOpState stencilOpFront = {};
        VkStencilOpState stencilOpBack = {};
        if (pRenderState->GetStencilEnabled())
        {
            stencilOpFront = VulkanConverter::Transform2VkStencilOpState(pRenderState->GetStencilCompareFunc_Front(),
                                                                         pRenderState->GetStencilRefValue_Front(),
                                                                         pRenderState->GetStencilCompareMask_Front(),
                                                                         pRenderState->GetStencilWriteMask_Front(),
                                                                         pRenderState->GetStencilFailOP_Front(),
                                                                         pRenderState->GetStencilDepthFailOP_Front(),
                                                                         pRenderState->GetStencilPassOP_Front());

            if (pRenderState->GetStencilTwoSidedEnabled())
            {
                stencilOpBack = VulkanConverter::Transform2VkStencilOpState(pRenderState->GetStencilCompareFunc_Back(),
                                                                            pRenderState->GetStencilRefValue_Back(),
                                                                            pRenderState->GetStencilCompareMask_Back(),
                                                                            pRenderState->GetStencilWriteMask_Back(),
                                                                            pRenderState->GetStencilFailOP_Back(),
                                                                            pRenderState->GetStencilDepthFailOP_Back(),
                                                                            pRenderState->GetStencilPassOP_Back());
            }
        }
        VkBool32 bBlend = pRenderState->GetSceneBlendingEnabled() ? VK_TRUE : VK_FALSE;
        VkBlendFactor blendColorFactorSrc = VulkanConverter::Transform2VkBlendFactor(pRenderState->GetSceneBlendingFactorSrcType_Color());
        VkBlendFactor blendColorFactorDst = VulkanConverter::Transform2VkBlendFactor(pRenderState->GetSceneBlendingFactorDstType_Color());
        VkBlendOp blendColorOp = VulkanConverter::Transform2VkBlendOp(pRenderState->GetSceneBlendingOperationOPType_Color());
        VkBlendFactor blendAlphaFactorSrc = VulkanConverter::Transform2VkBlendFactor(pRenderState->GetSceneBlendingFactorSrcType_Alpha());
        VkBlendFactor blendAlphaFactorDst = VulkanConverter::Transform2VkBlendFactor(pRenderState->GetSceneBlendingFactorDstType_Alpha());
        VkBlendOp blendAlphaOp = VulkanConverter::Transform2VkBlendOp(pRenderState->GetSceneBlendingOperationOPType_Alpha());
        VkColorComponentFlags colorWriteMask = VulkanConverter::Transform2VkColorComponentFlags(pRenderState->GetColorRWriteEnabled(),
                                                                                                pRenderState->GetColorGWriteEnabled(),
                                                                                                pRenderState->GetColorBWriteEnabled(),
                                                                                                pRenderState->GetColorAWriteEnabled());
        VkSampleCountFlagBits msaaSamples = VulkanConverter::Transform2VkSampleCountFlagBits(pRenderWindow->GetMSAASampleCountType());

        bool bRet = InitPipelineGraphics(aShaderStageCreateInfos,
                                         false, 0, 3,
                                         pBindingDescriptions,
                                         pAttributeDescriptions,
                                         pRenderPass->GetVkRenderPass(), pPipelineLayout->GetVkPipelineLayout(), pRenderWindow->GetVkViewports(), pRenderWindow->GetVkScissors(),
                                         primitiveTopology, frontFace, polygonMode, cullMode, lineWidth,
                                         bDepthTest, bDepthWrite, depthCompareOp,
                                         bStencilTest, stencilOpFront, stencilOpBack,
                                         bBlend, blendColorFactorSrc, blendColorFactorDst, blendColorOp,
                                         blendAlphaFactorSrc, blendAlphaFactorDst, blendAlphaOp,
                                         colorWriteMask,
                                         msaaSamples);

        if (bRet)
        {
            for (int i = 0; i < count; i++)
            {
                VulkanShaderModule* pShaderModule = aShaderModule[i];
                pShaderModule->AddRef();
                m_aShaderModule.push_back(pShaderModule);
            }
            pPipelineLayout->AddRef();
            m_pPipelineLayout = pPipelineLayout;
        }
        return bRet;
    }
    bool VulkanPipeline::InitPipelineGraphics(const VkPipelineShaderStageCreateInfoVector& aShaderStageCreateInfos,
                                              bool tessellationIsUsed, VkPipelineTessellationStateCreateFlags tessellationFlags, uint32_t tessellationPatchControlPoints,
                                              VkVertexInputBindingDescriptionVector* pBindingDescriptions,
                                              VkVertexInputAttributeDescriptionVector* pAttributeDescriptions,
                                              VkRenderPass renderPass, VkPipelineLayout pipelineLayout, const VkViewportVector& aViewports, const VkRect2DVector& aScissors,
                                              VkPrimitiveTopology primitiveTopology, VkFrontFace frontFace, VkPolygonMode polygonMode, VkCullModeFlagBits cullMode, float lineWidth,
                                              VkBool32 bDepthTest, VkBool32 bDepthWrite, VkCompareOp depthCompareOp, 
                                              VkBool32 bStencilTest, const VkStencilOpState& stencilOpFront, const VkStencilOpState& stencilOpBack, 
                                              VkBool32 bBlend, VkBlendFactor blendColorFactorSrc, VkBlendFactor blendColorFactorDst, VkBlendOp blendColorOp,
                                              VkBlendFactor blendAlphaFactorSrc, VkBlendFactor blendAlphaFactorDst, VkBlendOp blendAlphaOp,
                                              VkColorComponentFlags colorWriteMask,
                                              VkSampleCountFlagBits msaaSamples)
    {
        Destroy();

        if (!m_pDevice->CreateVkPipeline_Graphics(aShaderStageCreateInfos,
                                                  tessellationIsUsed, tessellationFlags, tessellationPatchControlPoints,
                                                  pBindingDescriptions, 
                                                  pAttributeDescriptions, 
                                                  renderPass, pipelineLayout, aViewports, aScissors,
                                                  primitiveTopology, frontFace, polygonMode, cullMode, lineWidth,
                                                  bDepthTest, bDepthWrite, depthCompareOp,
                                                  bStencilTest, stencilOpFront, stencilOpBack,
                                                  bBlend, blendColorFactorSrc, blendColorFactorDst, blendColorOp,
                                                  blendAlphaFactorSrc, blendAlphaFactorDst, blendAlphaOp,
                                                  colorWriteMask, 
                                                  msaaSamples,
                                                  VulkanPipelineCache::GetSingleton().GetVkPipelineCache(),
                                                  m_vkPipeline))
        {
            F_LogError("*********************** VulkanPipeline::InitPipelineGraphics: Failed to CreateVkPipeline_Graphics, name: [%s] !", GetName().c_str());
            return false;
        }

        return true;
    }

    bool VulkanPipeline::InitPipelineCompute(VulkanShaderModule* pShaderModule,
                                             VulkanPipelineLayout* pPipelineLayout,
                                             VkPipelineCreateFlags flags)
    {
        F_Assert(pShaderModule && pPipelineLayout && "VulkanPipeline::InitPipelineCompute")

        Destroy();
        if (!m_pDevice->CreateVkPipeline_Compute(pShaderModule->GetVkPipelineShaderStageCreateInfo(),
                                                 pPipelineLayout->GetVkPipelineLayout(),
                                                 flags,
                                                 VulkanPipelineCache::GetSingleton().GetVkPipelineCache(),
                                                 m_vkPipeline))
        {
            F_LogError("*********************** VulkanPipeline::InitPipelineCompute: Failed to CreateVkPipeline_Compute, name: [%s] !", GetName().c_str());
            return false;
        }

        m_bIsPipelineGraphics = false;
        pShaderModule->AddRef();
        m_aShaderModule.push_back(pShaderModule);
        pPipelineLayout->AddRef();
        m_pPipelineLayout = pPipelineLayout;

        return true;
    }

}; //LostPeterPluginRendererVulkan