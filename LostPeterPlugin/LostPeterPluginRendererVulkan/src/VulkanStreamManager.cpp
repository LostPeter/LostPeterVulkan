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

namespace LostPeterPluginRendererVulkan
{
    VulkanStreamManager::VulkanStreamManager(VulkanDevice* pDevice)
        : m_pDevice(pDevice)
    {
        F_Assert(m_pDevice != nullptr && "VulkanStreamManager::VulkanStreamManager")
        
    }

    VulkanStreamManager::~VulkanStreamManager()
    {

    }

    StreamVertex* VulkanStreamManager::CreateStreamVertex(uint32 nVertexSize, uint32 nVertexCount, EStreamUsageType eStreamUsage, bool bIsUseShadowStream /*= false*/)
    {
        return CreateStreamVertex(nVertexSize * nVertexCount, eStreamUsage, bIsUseShadowStream);
    }

	StreamVertex* VulkanStreamManager::CreateStreamVertex(uint32 nSizeInBytes, EStreamUsageType eStreamUsage, bool bIsUseShadowStream /*= false*/)
    {
        VulkanStreamVertex* pStreamVertex = new VulkanStreamVertex(m_pDevice, 0, nSizeInBytes, eStreamUsage, false, bIsUseShadowStream);
        return pStreamVertex;
    }

    StreamIndex* VulkanStreamManager::CreateStreamIndex(EStreamIndexType eStreamIndex, uint32 nIndexCount, EStreamUsageType eStreamUsage, bool bIsUseShadowStream /*= false*/)
    {
        VulkanStreamIndex* pStreamIndex = new VulkanStreamIndex(m_pDevice, 0, eStreamIndex, nIndexCount, eStreamUsage, false, bIsUseShadowStream);
        return pStreamIndex;
    }

}; //LostPeterPluginRendererVulkan