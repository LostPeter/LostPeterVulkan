/****************************************************************************
* LostPeterPluginRendererVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-11-19
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/VulkanBufferIndex.h"
#include "../include/VulkanDevice.h"

namespace LostPeter
{
    uint32_t VulkanBufferIndex::ms_nMaxID = 0;
    VulkanBufferIndex::VulkanBufferIndex(const String& strName, VulkanDevice* pDevice)
        : VulkanBuffer(strName, pDevice)
        , m_vkIndexType(VK_INDEX_TYPE_UINT16)
        , m_nIndexCount(0)
        , m_nTriangleCount(0)
    {
        
    }

    VulkanBufferIndex::~VulkanBufferIndex()
    {
        Destroy();
    }

    void VulkanBufferIndex::Destroy()
    {
        VulkanBuffer::Destroy();

        m_vkIndexType = VK_INDEX_TYPE_UINT16;
        m_nIndexCount = 0;
        m_nTriangleCount = 0;
    }

    bool VulkanBufferIndex::Init(uint32_t sizeDataByte, uint32_t countData)
    {
        uint8* pData = new uint8[sizeDataByte];
        memset(pData, 0, sizeDataByte);
        return Init(pData, sizeDataByte, countData, true, true);
    }
    bool VulkanBufferIndex::Init(void* pData, 
                                 uint32_t sizeDataByte, 
                                 uint32_t countData,
                                 bool bIsDeleteData,
                                 bool bIsUpdateData)
    {
        UpdateData(pData, sizeDataByte, countData, bIsDeleteData, bIsUpdateData);

        return true;
    }
    bool VulkanBufferIndex::Init(FMeshDataPCT& data, bool bIsUpdateData)
    {
        return Init(data.GetIndexData(), data.GetIndexDataSize(), data.GetIndexCount(), false, bIsUpdateData);
    }
    bool VulkanBufferIndex::Init(FMeshData& data, bool bIsUpdateData)
    {
        return Init(data.GetIndexData(), data.GetIndexDataSize(), data.GetIndexCount(), false, bIsUpdateData);
    }
    bool VulkanBufferIndex::Init(FMeshDataUV2& data, bool bIsUpdateData)
    {
        return Init(data.GetIndexData(), data.GetIndexDataSize(), data.GetIndexCount(), false, bIsUpdateData);
    }
    bool VulkanBufferIndex::Init(FMeshDataSkin& data, bool bIsUpdateData)
    {
        return Init(data.GetIndexData(), data.GetIndexDataSize(), data.GetIndexCount(), false, bIsUpdateData);
    }

    bool VulkanBufferIndex::UpdateData(void* pData, uint32_t sizeDataByte, uint32_t countData, bool bIsDeleteData, bool bIsUpdateData)
    {
        bool isInit = IsInit();
        if (isInit)
        {
            if (m_nDataSize < sizeDataByte)
            {
                Destroy();
            }
        } 

        m_nIndexCount = countData;
        if (countData * sizeof(uint16_t) == sizeDataByte)
        {
            m_vkIndexType = VK_INDEX_TYPE_UINT16;
        }
        else if (countData * sizeof(uint32_t) == sizeDataByte)
        {
            m_vkIndexType = VK_INDEX_TYPE_UINT32;
        }
        else
        {
            F_LogError("*********************** VulkanBufferIndex::Init: Wrong data size !");
            return false;
        }
        m_nTriangleCount = m_nIndexCount / 3;
        if (sizeDataByte > m_nDataSize)
            setDataSize(sizeDataByte);
        setData(pData, bIsDeleteData, bIsUpdateData);

        if (isInit)
        {
            if (!bIsUpdateData || m_vkBufferMemory_Transfer == VK_NULL_HANDLE)
            {
                F_LogError("*********************** VulkanBufferIndex::UpdateData: UpdateData failed, not update data buffer !");
                return false;
            }

            m_pDevice->WriteVkBuffer(m_vkBufferMemory_Transfer, pData, sizeDataByte, 0);
            m_pDevice->CopyVkBuffer(m_vkBuffer_Transfer, m_vkBuffer, sizeDataByte);
        }
        else
        {
            if (bIsUpdateData)
            {
                if (!m_pDevice->CreateVkBufferIndex(pData, 
                                                    sizeDataByte, 
                                                    m_vkBuffer, 
                                                    m_vkBufferMemory, 
                                                    m_vkBuffer_Transfer, 
                                                    m_vkBufferMemory_Transfer))
                {
                    F_LogError("*********************** VulkanBufferIndex::Init: 1 CreateVkBufferIndex failed !");
                    return false;
                }
            }
            else
            {
                if (!m_pDevice->CreateVkBufferIndex(pData, 
                                                    sizeDataByte, 
                                                    m_vkBuffer, 
                                                    m_vkBufferMemory))
                {
                    F_LogError("*********************** VulkanBufferIndex::Init: 2 CreateVkBufferIndex failed !");
                    return false;
                }
            }
        }
        
        SetIsInit(true);
        return true;
    }

    VulkanBufferIndex* VulkanBufferIndex::Clone()
    {
        String strName = FUtilString::FormatString("%s_Copy_%d", GetName().c_str(), GetNextID());
        VulkanBufferIndex* pBufferIndex = new VulkanBufferIndex(strName, m_pDevice);
        void* pData = CloneData();
        if (!pBufferIndex->Init(pData, m_nDataSize, m_nIndexCount, true, m_bIsUpdateData))
        {
            F_DELETE(pBufferIndex)
            return nullptr;
        }
        return pBufferIndex;
    }

}; //LostPeter