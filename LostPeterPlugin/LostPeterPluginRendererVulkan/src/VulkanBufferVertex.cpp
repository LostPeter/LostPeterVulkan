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

#include "../include/VulkanBufferVertex.h"
#include "../include/VulkanDevice.h"

namespace LostPeter
{
    uint32_t VulkanBufferVertex::ms_nMaxID = 0;
    VulkanBufferVertex::VulkanBufferVertex(const String& strName, VulkanDevice* pDevice)
        : VulkanBuffer(strName, pDevice)
        , m_nVertexCount(0)
    {
        
    }

    VulkanBufferVertex::~VulkanBufferVertex()
    {
        Destroy();
    }

    void VulkanBufferVertex::Destroy()
    {
        VulkanBuffer::Destroy();

        m_nVertexCount = 0;
    }
    
    bool VulkanBufferVertex::Init(uint32_t sizeDataByte, uint32_t countData)
    {
        uint8* pData = new uint8[sizeDataByte];
        memset(pData, 0, sizeDataByte);
        return Init(pData, sizeDataByte, countData, true, true);
    }

    bool VulkanBufferVertex::Init(void* pData, 
                                  uint32_t sizeDataByte, 
                                  uint32_t countData,
                                  bool bIsDeleteData,
                                  bool bIsUpdateData)
    {
        UpdateData(pData, sizeDataByte, countData, bIsDeleteData, bIsUpdateData);

        return true;
    }
    bool VulkanBufferVertex::Init(FMeshDataPCT& data, bool bIsUpdateData)
    {
        return Init(data.GetVertexData(), data.GetVertexDataSize(), data.GetVertexCount(), false, bIsUpdateData);
    }
    bool VulkanBufferVertex::Init(FMeshData& data, bool bIsUpdateData)
    {
        return Init(data.GetVertexData(), data.GetVertexDataSize(), data.GetVertexCount(), false, bIsUpdateData);
    }
    bool VulkanBufferVertex::Init(FMeshDataUV2& data, bool bIsUpdateData)
    {
        return Init(data.GetVertexData(), data.GetVertexDataSize(), data.GetVertexCount(), false, bIsUpdateData);
    }
    bool VulkanBufferVertex::Init(FMeshDataSkin& data, bool bIsUpdateData)
    {
        return Init(data.GetVertexData(), data.GetVertexDataSize(), data.GetVertexCount(), false, bIsUpdateData);
    }

    bool VulkanBufferVertex::UpdateData(void* pData, uint32_t sizeDataByte, uint32_t countData, bool bIsDeleteData, bool bIsUpdateData)
    {
        bool isInit = IsInit();
        if (isInit)
        {
            if (m_nDataSize < sizeDataByte)
            {
                Destroy();
            }
        } 

        m_nVertexCount = countData;
        if (sizeDataByte > m_nDataSize)
            setDataSize(sizeDataByte);
        setData(pData, bIsDeleteData, bIsUpdateData);

        if (isInit)
        {
            if (!bIsUpdateData || m_vkBufferMemory_Transfer == VK_NULL_HANDLE)
            {
                F_LogError("*********************** VulkanBufferVertex::UpdateData: UpdateData failed, not update data buffer !");
                return false;
            }

            m_pDevice->WriteVkBuffer(m_vkBufferMemory_Transfer, pData, sizeDataByte, 0);
            m_pDevice->CopyVkBuffer(m_vkBuffer_Transfer, m_vkBuffer, sizeDataByte);
        }
        else
        {
            if (bIsUpdateData)
            {
                if (!Vulkanm_pDevice->CreateVkBufferVertex(pData, 
                                                           sizeDataByte, 
                                                           m_vkBuffer, 
                                                           m_vkBufferMemory, 
                                                           m_vkBuffer_Transfer, 
                                                           m_vkBufferMemory_Transfer))
                {
                    F_LogError("*********************** VulkanBufferVertex::UpdateData: 1 CreateVkBufferVertex failed !");
                    return false;
                }
            }
            else
            {
                if (!m_pDevice->CreateVkBufferVertex(pData, 
                                                     sizeDataByte, 
                                                     m_vkBuffer, 
                                                     m_vkBufferMemory))
                {
                    F_LogError("*********************** VulkanBufferVertex::UpdateData: 2 CreateVkBufferVertex failed !");
                    return false;
                }
            }
        }
        
        SetIsInit(true);
        return true;
    }

    VulkanBufferVertex* VulkanBufferVertex::Clone()
    {
        String strName =  FUtilString::FormatString("%s_Copy_%d", m_strName.c_str(), GetNextID());
        VulkanBufferVertex* pBufferVertex = new VulkanBufferVertex(strName);
        void* pData = CloneData();
        if (!pBufferVertex->Init(pData, m_nDataSize, m_nVertexCount, true, m_bIsUpdateData))
        {
            F_DELETE(pBufferVertex)
            return nullptr;
        }
        return pBufferVertex;
    }

}; //LostPeter