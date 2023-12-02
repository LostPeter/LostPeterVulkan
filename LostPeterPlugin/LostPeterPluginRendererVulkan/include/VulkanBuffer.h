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

#ifndef _VULKAN_BUFFER_H_
#define _VULKAN_BUFFER_H_

#include "VulkanPreDefine.h"

namespace LostPeter
{
    class VulkanBuffer : public Base
    {
    public:
        VulkanBuffer(const String& strName, VulkanDevice* pDevice);
        virtual ~VulkanBuffer();

    public:
    protected:
        VulkanDevice* m_pDevice;

        VkBuffer m_vkBuffer;
        VkDeviceMemory m_vkBufferMemory;
        VkBuffer m_vkBuffer_Transfer;
        VkDeviceMemory m_vkBufferMemory_Transfer;

        uint8* m_pData;
        uint32_t m_nDataSize;

        bool m_bIsDeleteData;
        bool m_bIsUpdateData;

    public:
        E_FORCEINLINE const VkBuffer& GetVkBuffer() const { return m_vkBuffer; }
        E_FORCEINLINE VkBuffer& GetVkBuffer() { return m_vkBuffer; }
        E_FORCEINLINE const VkDeviceMemory& GetVkBufferMemory() const { return m_vkBufferMemory; }
        E_FORCEINLINE VkDeviceMemory& GetVkBufferMemory() { return m_vkBufferMemory; }

        E_FORCEINLINE void* GetData() const { return m_pData; }
        E_FORCEINLINE uint32_t GetDataSize() const { return m_nDataSize; }
        E_FORCEINLINE bool IsDeleteData() const { return m_bIsDeleteData; }
        E_FORCEINLINE bool IsUpdateData() const { return m_bIsUpdateData; }

    public:
        virtual void Destroy();
        virtual bool Init(uint32_t sizeDataByte, uint32_t countData) = 0;
        virtual bool Init(void* pData, 
                          uint32_t sizeDataByte,
                          uint32_t countData,
                          bool bIsDeleteData,
                          bool bIsUpdateData) = 0;
        virtual bool Init(FMeshDataPCT& data, bool bIsUpdateData) = 0;
        virtual bool Init(FMeshData& data, bool bIsUpdateData) = 0;
        virtual bool Init(FMeshDataUV2& data, bool bIsUpdateData) = 0;
        virtual bool Init(FMeshDataSkin& data, bool bIsUpdateData) = 0;

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