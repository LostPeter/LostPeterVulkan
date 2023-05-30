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

#ifndef _VULKAN_BUFFER_H_
#define _VULKAN_BUFFER_H_

#include "VulkanObject.h"

namespace LostPeter
{
    class utilExport VulkanBuffer : public VulkanObject
    {
    public:
        VulkanBuffer(const String& strName);
        virtual ~VulkanBuffer();

    public:
    protected:
        VkBuffer m_vkBuffer;
        VkDeviceMemory m_vkBufferMemory;
        VkBuffer m_vkBuffer_Transfer;
        VkDeviceMemory m_vkBufferMemory_Transfer;

        uint8* m_pData;
        uint32_t m_nDataSize;

        bool m_bIsDeleteData;
        bool m_bIsUpdateData;

    public:
        UTIL_FORCEINLINE const VkBuffer& GetVkBuffer() const { return m_vkBuffer; }
        UTIL_FORCEINLINE VkBuffer& GetVkBuffer() { return m_vkBuffer; }
        UTIL_FORCEINLINE const VkDeviceMemory& GetVkBufferMemory() const { return m_vkBufferMemory; }
        UTIL_FORCEINLINE VkDeviceMemory& GetVkBufferMemory() { return m_vkBufferMemory; }

        UTIL_FORCEINLINE void* GetData() const { return m_pData; }
        UTIL_FORCEINLINE uint32_t GetDataSize() const { return m_nDataSize; }
        UTIL_FORCEINLINE bool IsDeleteData() const { return m_bIsDeleteData; }
        UTIL_FORCEINLINE bool IsUpdateData() const { return m_bIsUpdateData; }

    public:
        virtual void Destroy();
        virtual bool Init(uint32_t sizeDataByte, uint32_t countData) = 0;
        virtual bool Init(void* pData, 
                          uint32_t sizeDataByte,
                          uint32_t countData,
                          bool bIsDeleteData,
                          bool bIsUpdateData) = 0;
        // virtual bool Init(MeshPCTData& data, bool bIsUpdateData) = 0;
        // virtual bool Init(MeshData& data, bool bIsUpdateData) = 0;
        // virtual bool Init(MeshUV2Data& data, bool bIsUpdateData) = 0;
        // virtual bool Init(SkinMeshData& data, bool bIsUpdateData) = 0;

        virtual bool UpdateData(void* pData, uint32_t sizeDataByte, uint32_t countData, bool bIsDeleteData, bool bIsUpdateData) = 0;

        virtual void* CloneData();

    protected:
        virtual void destroyData();

        virtual void setData(void* pData, bool bIsDeleteData, bool bIsUpdateData)
        {
            if (bIsDeleteData)
                m_pData = (uint8*)pData;
            else
                m_pData = nullptr;
            m_bIsDeleteData = bIsDeleteData;
            m_bIsUpdateData = bIsUpdateData;
        }
        virtual void setDataSize(uint32_t nDataSize) { m_nDataSize = nDataSize; }

    //override
    public:
        virtual bool IsInit() const { return true; }
        virtual void SetIsInit(bool b) { }
        virtual bool IsShowSphere() const { return false; }
        virtual void SetIsShowSphere(bool bIsShowSphere) { }
        virtual bool IsShowAABB() const { return false; }
        virtual void SetIsShowAABB(bool bIsShowAABB) { }
    };

}; //LostPeter

#endif