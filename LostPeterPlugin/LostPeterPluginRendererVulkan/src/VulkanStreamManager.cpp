/****************************************************************************
* LostPeterPluginRendererVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-12-03
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/VulkanStreamManager.h"
#include "../include/VulkanDevice.h"
#include "../include/VulkanStreamVertex.h"
#include "../include/VulkanStreamIndex.h"
#include "../include/VulkanStreamUniform.h"

namespace LostPeterPluginRendererVulkan
{
    VulkanStreamManager::VulkanStreamManager(VulkanDevice* pDevice)
        : m_pDevice(pDevice)
    {
        F_Assert(m_pDevice != nullptr && "VulkanStreamManager::VulkanStreamManager")
        
    }

    VulkanStreamManager::~VulkanStreamManager()
    {
        Destroy();
    }

    StreamVertex* VulkanStreamManager::CreateStreamVertex(uint32 nVertexSize, uint32 nVertexCount, EStreamUsageType eStreamUsage, bool bIsUseShadowStream /*= false*/)
    {
        return CreateStreamVertex(nVertexSize * nVertexCount, eStreamUsage, bIsUseShadowStream);
    }
	StreamVertex* VulkanStreamManager::CreateStreamVertex(uint32 nSizeInBytes, EStreamUsageType eStreamUsage, bool bIsUseShadowStream /*= false*/)
    {
        VulkanStreamVertex* pStreamVertex = new VulkanStreamVertex(m_pDevice, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, nSizeInBytes, eStreamUsage, false, bIsUseShadowStream);
        return pStreamVertex;
    }

    StreamIndex* VulkanStreamManager::CreateStreamIndex(EStreamIndexType eStreamIndex, uint32 nIndexCount, EStreamUsageType eStreamUsage, bool bIsUseShadowStream /*= false*/)
    {
        VulkanStreamIndex* pStreamIndex = new VulkanStreamIndex(m_pDevice, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, eStreamIndex, nIndexCount, eStreamUsage, false, bIsUseShadowStream);
        return pStreamIndex;
    }

    StreamUniform* VulkanStreamManager::CreateStreamUniform(uint32 nUniformSize, uint32 nUniformCount, EStreamUsageType eStreamUsage, bool bIsUseShadowStream /*= false*/)
    {
        return CreateStreamUniform(nUniformSize * nUniformCount, eStreamUsage, bIsUseShadowStream);
    }
	StreamUniform* VulkanStreamManager::CreateStreamUniform(uint32 nSizeInBytes, EStreamUsageType eStreamUsage, bool bIsUseShadowStream /*= false*/)
    {
        VulkanStreamUniform* pStreamUniform = new VulkanStreamUniform(m_pDevice, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, nSizeInBytes, eStreamUsage, false, bIsUseShadowStream);
        return pStreamUniform;
    }

}; //LostPeterPluginRendererVulkan