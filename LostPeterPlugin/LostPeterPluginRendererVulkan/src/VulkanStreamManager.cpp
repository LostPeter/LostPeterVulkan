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

    StreamVertex* VulkanStreamManager::CreateStreamVertex(size_t nVertexSize, size_t nVertexNum, EStreamUsageType eStreamUsage, bool bUseShadowStream /*= false*/)
    {
        return CreateStreamVertex(nVertexSize * nVertexNum, eStreamUsage, bUseShadowStream);
    }

	StreamVertex* VulkanStreamManager::CreateStreamVertex(size_t nSizeInBytes, EStreamUsageType eStreamUsage, bool bUseShadowStream /*= false*/)
    {
        VulkanStreamVertex* pStreamVertex = new VulkanStreamVertex(m_pDevice, 0, nSizeInBytes, eStreamUsage, false, bUseShadowStream);
        return pStreamVertex;
    }

    StreamIndex* VulkanStreamManager::CreateStreamIndex(EStreamIndexType eStreamIndex, size_t nIndexNum, EStreamUsageType eStreamUsage, bool bUseShadowStream /*= false*/)
    {
        VulkanStreamIndex* pStreamIndex = new VulkanStreamIndex(m_pDevice, 0, eStreamIndex, nIndexNum, eStreamUsage, false, bUseShadowStream);
        return pStreamIndex;
    }

}; //LostPeterPluginRendererVulkan