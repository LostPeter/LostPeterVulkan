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

#include "../include/VulkanPipelineManager.h"
#include "../include/VulkanDevice.h"
#include "../include/VulkanRenderWindow.h"
#include "../include/VulkanRenderPass.h"
#include "../include/VulkanShaderModule.h"
#include "../include/VulkanPipelineLayout.h"
#include "../include/VulkanPipeline.h"

template<> LostPeterPluginRendererVulkan::VulkanPipelineManager* LostPeterFoundation::FSingleton<LostPeterPluginRendererVulkan::VulkanPipelineManager>::ms_Singleton = nullptr;

namespace LostPeterPluginRendererVulkan
{
    VulkanPipelineManager* VulkanPipelineManager::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	VulkanPipelineManager& VulkanPipelineManager::GetSingleton()
	{  
		F_Assert(ms_Singleton && "VulkanPipelineManager::GetSingleton")
		return (*ms_Singleton);     
	}


    VulkanPipelineManager::VulkanPipelineManager(VulkanDevice* pDevice)
        : m_pDevice(pDevice)
    {
        F_Assert(m_pDevice && "VulkanPipelineManager::VulkanPipelineManager")
    }   

    VulkanPipelineManager::~VulkanPipelineManager()
    {
        Destroy();
    }

    void VulkanPipelineManager::Destroy()
    {
        DeletePipelineAll();
    }

    bool VulkanPipelineManager::HasPipeline(const String& strName)
    {
        return GetPipeline(strName) != nullptr;
    }
    VulkanPipeline* VulkanPipelineManager::GetPipeline(const String& strName)
    {
        VulkanPipelinePtrMap::iterator itFind = m_mapPipeline.find(strName);
        if (itFind == m_mapPipeline.end())
        {
            return nullptr;
        }
        return itFind->second;
    }
    bool VulkanPipelineManager::AddPipeline(VulkanPipeline* pPipeline)
    {
        const String& strName = pPipeline->GetName();
        VulkanPipelinePtrMap::iterator itFind = m_mapPipeline.find(strName);
        if (itFind != m_mapPipeline.end())
        {
            F_LogError("*********************** VulkanPipelineManager::AddPipeline: Pipeline name already exist: [%s] !", strName.c_str());
            return false;
        }
        
        m_mapPipeline.insert(VulkanPipelinePtrMap::value_type(strName, pPipeline));
        m_aPipeline.push_back(pPipeline);
        return true;
    }


    VulkanPipeline* VulkanPipelineManager::CreatePipelineGraphics(const String& namePipeline,
                                                                  VulkanRenderWindow* pRenderWindow,
                                                                  VulkanRenderPass* pRenderPass,
                                                                  VulkanShaderModulePtrVector& aShaderModule,
                                                                  VulkanPipelineLayout* pPipelineLayout,
                                                                  FMeshVertexType eMeshVertex,
                                                                  FRenderPrimitiveType eRenderPrimitive,
                                                                  RenderState* pRenderState)
    {
        VulkanPipeline* pPipeline = GetPipeline(namePipeline);
        if (pPipeline != nullptr)
        {
            pPipeline->AddRef();
            return pPipeline;
        }

        pPipeline = new VulkanPipeline(namePipeline, this->m_pDevice);
        if (!pPipeline->InitPipelineGraphics(pRenderWindow,
                                             pRenderPass,
                                             aShaderModule,
                                             pPipelineLayout,
                                             eMeshVertex,
                                             eRenderPrimitive,
                                             pRenderState))
        {
            F_LogError("*********************** VulkanPipelineManager::CreatePipelineGraphics: Failed to init VulkanPipeline, name: [%s] !", namePipeline.c_str());
            F_DELETE(pPipeline)
            return nullptr;
        }
        AddPipeline(pPipeline);
        pPipeline->AddRef();
        return pPipeline;
    }
    VulkanPipeline* VulkanPipelineManager::CreatePipelineGraphics(const String& namePipeline,
                                                                  VulkanRenderWindow* pRenderWindow,
                                                                  VulkanRenderPass* pRenderPass,
                                                                  VulkanShaderModulePtrVector& aShaderModule,
                                                                  VulkanPipelineLayout* pPipelineLayout,
                                                                  VkVertexInputBindingDescriptionVector* pBindingDescriptions,
                                                                  VkVertexInputAttributeDescriptionVector* pAttributeDescriptions,
                                                                  FRenderPrimitiveType eRenderPrimitive,
                                                                  RenderState* pRenderState)
    {
        VulkanPipeline* pPipeline = GetPipeline(namePipeline);
        if (pPipeline != nullptr)
        {
            pPipeline->AddRef();
            return pPipeline;
        }

        pPipeline = new VulkanPipeline(namePipeline, this->m_pDevice);
        if (!pPipeline->InitPipelineGraphics(pRenderWindow,
                                             pRenderPass,
                                             aShaderModule,
                                             pPipelineLayout,
                                             pBindingDescriptions,
                                             pAttributeDescriptions,
                                             eRenderPrimitive,
                                             pRenderState))
        {
            F_LogError("*********************** VulkanPipelineManager::CreatePipelineGraphics: Failed to init VulkanPipeline, name: [%s] !", namePipeline.c_str());
            F_DELETE(pPipeline)
            return nullptr;
        }
        AddPipeline(pPipeline);
        pPipeline->AddRef();
        return pPipeline;
    }
    VulkanPipeline* VulkanPipelineManager::CreatePipelineGraphics(const String& namePipeline,
                                                                  const VkPipelineShaderStageCreateInfoVector& aShaderStageCreateInfos,
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
        VulkanPipeline* pPipeline = GetPipeline(namePipeline);
        if (pPipeline != nullptr)
        {
            pPipeline->AddRef();
            return pPipeline;
        }

        pPipeline = new VulkanPipeline(namePipeline, this->m_pDevice);
        if (!pPipeline->InitPipelineGraphics(aShaderStageCreateInfos,
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
                                             msaaSamples))
        {
            F_LogError("*********************** VulkanPipelineManager::CreatePipelineGraphics: Failed to init VulkanPipeline, name: [%s] !", namePipeline.c_str());
            F_DELETE(pPipeline)
            return nullptr;
        }
        AddPipeline(pPipeline);
        pPipeline->AddRef();
        return pPipeline;
    }

    VulkanPipeline* VulkanPipelineManager::CreatePipelineCompute(const String& namePipeline,
                                                                 VulkanShaderModule* pShaderModule,
                                                                 VulkanPipelineLayout* pPipelineLayout,
                                                                 VkPipelineCreateFlags flags)
    {
        VulkanPipeline* pPipeline = GetPipeline(namePipeline);
        if (pPipeline != nullptr)
        {
            pPipeline->AddRef();
            return pPipeline;
        }

        pPipeline = new VulkanPipeline(namePipeline, this->m_pDevice);
        if (!pPipeline->InitPipelineCompute(pShaderModule,
                                            pPipelineLayout,
                                            flags))
        {
            F_LogError("*********************** VulkanPipelineManager::CreatePipelineCompute: Failed to init VulkanPipeline, name: [%s] !", namePipeline.c_str());
            F_DELETE(pPipeline)
            return nullptr;
        }
        AddPipeline(pPipeline);
        pPipeline->AddRef();
        return pPipeline;
    }

    void VulkanPipelineManager::DeletePipeline(const String& strName)
    {
        VulkanPipelinePtrMap::iterator itFind = m_mapPipeline.find(strName);
        if (itFind == m_mapPipeline.end())
        {
            return;
        }

        itFind->second->DelRef();
        if (itFind->second->CanDel())
        {
            VulkanPipelinePtrVector::iterator itFindA = std::find(m_aPipeline.begin(), m_aPipeline.end(), itFind->second);
            if (itFindA != m_aPipeline.end())
                m_aPipeline.erase(itFindA);
            F_DELETE(itFind->second)
            m_mapPipeline.erase(itFind);
        }
    }
    void VulkanPipelineManager::DeletePipeline(VulkanPipeline* pPipeline)
    {
        if (!pPipeline)
            return;
        DeletePipeline(pPipeline->GetName());
    }
    void VulkanPipelineManager::DeletePipelineAll()
    {
        for (VulkanPipelinePtrVector::iterator it = m_aPipeline.begin();
             it != m_aPipeline.end(); ++it)
        {
            F_DELETE(*it)
        }
        m_aPipeline.clear();
        m_mapPipeline.clear();
    }

}; //LostPeterPluginRendererVulkan