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

#include "../include/VulkanStreamIndex.h"
#include "../include/VulkanDevice.h"

namespace LostPeter
{
    VulkanStreamIndex::VulkanStreamIndex(VulkanDevice* pDevice, uint32 nTarget, 
                                         EStreamIndexType eStreamIndex, size_t nIndexNum, 
                                         EStreamUsageType eStreamUsage,
					                     bool bUseSystemMemory, bool bUseShadowStream)
        : StreamIndex(eStreamIndex, nIndexNum, eStreamUsage, bUseSystemMemory, bUseShadowStream)
        , m_pDevice(pDevice)
        , m_nTarget(nTarget)
        , m_vkBuffer(nullptr)
    {
        F_Assert(pDevice && "VulkanStreamIndex::VulkanStreamIndex")
        createVkBuffer();
        if (m_eStreamUsage == E_StreamUsage_Dynamic)
        {
            m_pStreamShadow = new VulkanStreamIndex(pDevice, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, eStreamIndex, nIndexNum, E_StreamUsage_Static, true, false);
        }
    }

    VulkanStreamIndex::~VulkanStreamIndex()
    {
        F_DELETE(m_pStreamShadow)
        destroyVkBuffer();
    }

    void VulkanStreamIndex::destroyVkBuffer()
    {
        if (m_vkBuffer != nullptr)
        {
            m_pDevice->DestroyVkBuffer(m_vkBuffer, m_vkDeviceMemory);
        }
        m_vkBuffer = nullptr;
        m_vkDeviceMemory = nullptr;
    }
    void VulkanStreamIndex::createVkBuffer()
    {
        destroyVkBuffer();

        VkBufferUsageFlags usage = m_nTarget;
        if ((m_eStreamUsage & E_StreamUsage_Static) == 0)
        {
            usage |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        }
        uint32 properties = 0;
        if (m_eStreamUsage != E_StreamUsage_WriteOnly)
            properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
        else
            properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

        if (!m_pDevice->CreateVkBuffer(m_nStreamSizeInBytes,
                                       usage,
                                       properties,
                                       m_vkBuffer,
                                       m_vkDeviceMemory))
        {
            F_LogError("*********************** VulkanStreamIndex::createVkBuffer: CreateVkBuffer failed !");
        }
    }

    void VulkanStreamIndex::ReadData(size_t nOffset, size_t nLength, void* pDest)
    {
        StreamLock streamLock(this, nOffset, nLength, E_StreamLock_ReadOnly);
        memcpy(pDest, streamLock.m_pData, nLength);
    }

	void VulkanStreamIndex::WriteData(size_t nOffset, size_t nLength, const void* pSource, bool bDiscardWholeStream /*= false*/)
    {
        StreamLock streamLock(this, nOffset, nLength, bDiscardWholeStream ? E_StreamLock_Discard : E_StreamLock_Normal);
        memcpy(streamLock.m_pData, pSource, nLength);
    }

	void VulkanStreamIndex::CopyData(Stream& streamSrc, size_t nSrcOffset, size_t nDstOffset, size_t nLength, bool bDiscardWholeStream /*= false*/)
    {
        VulkanStreamIndex* pBufferSrc = dynamic_cast<VulkanStreamIndex*>(&streamSrc);
        if (pBufferSrc && (m_eStreamUsage & E_StreamUsage_Dynamic) == 0)
        {
            m_pDevice->CopyVkBuffer(pBufferSrc->GetVkBuffer(), m_vkBuffer, nSrcOffset, nDstOffset, nLength);
        }
        else
        {
            const void* pDataSrc = streamSrc.Lock(nSrcOffset, nLength, E_StreamLock_ReadOnly);
            WriteData(nDstOffset, nLength, pDataSrc, bDiscardWholeStream);
            streamSrc.Unlock();
        }
    }

    void VulkanStreamIndex::UpdateFromShadow()
    {
        if(m_pStreamShadow && m_bShadowUpdated && !m_bSuppressUpdate)
		{
			bool bDiscardWholeStream = m_nLockStart == 0 && m_nLockSize == m_nStreamSizeInBytes;
			CopyData(*m_pStreamShadow, m_nLockStart, m_nLockStart, m_nLockSize, bDiscardWholeStream);
			m_bShadowUpdated = false;
        }
    }

	void* VulkanStreamIndex::lockImpl(size_t nOffset, size_t nLength, EStreamLockType eStreamLock)
    {
        if (m_pStreamShadow)
            return m_pStreamShadow->Lock(nOffset, nLength, eStreamLock);

        if (eStreamLock == E_StreamLock_Discard && m_eStreamUsage == E_StreamUsage_DynamicWriteOnly)
        {
            createVkBuffer();
        }

        void* pRet = 0;
        E_VK_CHECK(vkMapMemory(m_pDevice->GetVkDevice(), m_vkDeviceMemory, 0, m_nStreamSizeInBytes, 0, &pRet))
        return static_cast<uint8*>(pRet) + nOffset;
    }

	void VulkanStreamIndex::unlockImpl()
    {
        F_Assert(!m_pStreamShadow && "VulkanStreamIndex::unlockImpl: Should be handled by UpdateFromShadow !")
        vkUnmapMemory(m_pDevice->GetVkDevice(), m_vkDeviceMemory);
    }

}; //LostPeter