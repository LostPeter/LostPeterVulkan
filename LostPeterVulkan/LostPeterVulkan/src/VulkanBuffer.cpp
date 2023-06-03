/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-05-30
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/PreInclude.h"
#include "../include/VulkanBuffer.h"
#include "../include/VulkanManager.h"
#include "../include/VulkanDevice.h"

namespace LostPeter
{
    VulkanBuffer::VulkanBuffer(const String& strName)
        : VulkanObject(strName)
        , m_vkBuffer(VK_NULL_HANDLE)
        , m_vkBufferMemory(VK_NULL_HANDLE)
        , m_vkBuffer_Transfer(VK_NULL_HANDLE)
        , m_vkBufferMemory_Transfer(VK_NULL_HANDLE)
        , m_pData(nullptr)
        , m_nDataSize(0)
        , m_bIsDeleteData(false)
        , m_bIsUpdateData(false)
    {
        
    }

    VulkanBuffer::~VulkanBuffer()
    {
        
    }

    void VulkanBuffer::Destroy()
    {
        destroyData();
        m_bIsDeleteData = false;
        m_bIsUpdateData = false;

        VulkanObject::ms_pVulkanManager->GetDevice()->DestroyVkBuffer(m_vkBuffer, m_vkBufferMemory);
        m_vkBuffer = VK_NULL_HANDLE;
        m_vkBufferMemory = VK_NULL_HANDLE;

        VulkanObject::ms_pVulkanManager->GetDevice()->DestroyVkBuffer(m_vkBuffer_Transfer, m_vkBufferMemory_Transfer);
        m_vkBuffer_Transfer = VK_NULL_HANDLE;
        m_vkBufferMemory_Transfer = VK_NULL_HANDLE;
        SetIsInit(false);
    }
    void VulkanBuffer::destroyData()
    {
        if (m_bIsDeleteData)
        {
            F_DELETE_T(m_pData)
        }
        m_pData = nullptr;
        m_nDataSize = 0;
    }

    void* VulkanBuffer::CloneData()
    {
        uint8* pData = new uint8[m_nDataSize];
        memcpy(pData, m_pData, m_nDataSize);
        return pData;
    }

}; //LostPeter