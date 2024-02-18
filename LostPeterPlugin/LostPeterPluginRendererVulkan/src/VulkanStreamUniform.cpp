/****************************************************************************
* LostPeterPluginRendererVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-02-18
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/VulkanStreamUniform.h"
#include "../include/VulkanDevice.h"

namespace LostPeterPluginRendererVulkan
{
    VulkanStreamUniform::VulkanStreamUniform(VulkanDevice* pDevice, 
                                             uint32 nTarget, 
                                             uint32 nSizeInBytes, 
                                             EStreamUsageType eStreamUsage,
					                         bool bIsUseSystemMemory, 
                                             bool bIsUseShadowStream)
        : StreamUniform(nSizeInBytes, 
                        eStreamUsage, 
                        bIsUseSystemMemory, 
                        bIsUseShadowStream)
        , m_pDevice(pDevice)
        , m_nTarget(nTarget)
        , m_vkBuffer(nullptr)
        , m_vkDeviceMemory(nullptr)
    {
        F_Assert(pDevice && "VulkanStreamUniform::VulkanStreamUniform")
        createVkBuffer();
        if (m_eStreamUsage == E_StreamUsage_Dynamic)
        {
            F_DELETE(m_pStreamShadow)
            m_pStreamShadow = new VulkanStreamUniform(pDevice, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, nSizeInBytes, E_StreamUsage_Static, true, false);
        }
    }

    VulkanStreamUniform::~VulkanStreamUniform()
    {
        F_DELETE(m_pStreamShadow)
        destroyVkBuffer();
    }

    void VulkanStreamUniform::destroyVkBuffer()
    {
        if (m_vkBuffer != nullptr)
        {
            m_pDevice->DestroyVkBuffer(m_vkBuffer, m_vkDeviceMemory);
        }
        m_vkBuffer = nullptr;
        m_vkDeviceMemory = nullptr;
    }
    void VulkanStreamUniform::createVkBuffer()
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
            F_LogError("*********************** VulkanStreamUniform::createVkBuffer: CreateVkBuffer failed !");
        }
    }

    void VulkanStreamUniform::ReadData(uint32 nOffset, uint32 nLength, void* pDest)
    {
        StreamLock streamLock(this, nOffset, nLength, E_StreamLock_ReadOnly);
        memcpy(pDest, streamLock.m_pData, nLength);
    }

	void VulkanStreamUniform::WriteData(uint32 nOffset, uint32 nLength, const void* pSource, bool bIsDiscardWholeStream /*= false*/)
    {
        StreamLock streamLock(this, nOffset, nLength, bIsDiscardWholeStream ? E_StreamLock_Discard : E_StreamLock_Normal);
        memcpy(streamLock.m_pData, pSource, nLength);
    }

	void VulkanStreamUniform::CopyData(Stream& streamSrc, uint32 nSrcOffset, uint32 nDstOffset, uint32 nLength, bool bIsDiscardWholeStream /*= false*/)
    {
        VulkanStreamUniform* pBufferSrc = dynamic_cast<VulkanStreamUniform*>(&streamSrc);
        if (pBufferSrc && (m_eStreamUsage & E_StreamUsage_Dynamic) == 0)
        {
            m_pDevice->CopyVkBuffer(pBufferSrc->GetVkBuffer(), m_vkBuffer, nSrcOffset, nDstOffset, nLength);
        }
        else
        {
            const void* pDataSrc = streamSrc.Lock(nSrcOffset, nLength, E_StreamLock_ReadOnly);
            WriteData(nDstOffset, nLength, pDataSrc, bIsDiscardWholeStream);
            streamSrc.Unlock();
        }
    }

    void VulkanStreamUniform::UpdateFromShadow()
    {
        if(m_pStreamShadow && m_bIsShadowUpdated && !m_bIsSuppressUpdate)
		{
			bool bIsDiscardWholeStream = m_nLockStart == 0 && m_nLockSize == m_nStreamSizeInBytes;
			CopyData(*m_pStreamShadow, m_nLockStart, m_nLockStart, m_nLockSize, bIsDiscardWholeStream);
			m_bIsShadowUpdated = false;
        }
    }

	void* VulkanStreamUniform::lockImpl(uint32 nOffset, uint32 nLength, EStreamLockType eStreamLock)
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

	void VulkanStreamUniform::unlockImpl()
    {
        F_Assert(!m_pStreamShadow && "VulkanStreamUniform::unlockImpl: Should be handled by UpdateFromShadow !")
        vkUnmapMemory(m_pDevice->GetVkDevice(), m_vkDeviceMemory);
    }

}; //LostPeterPluginRendererVulkan