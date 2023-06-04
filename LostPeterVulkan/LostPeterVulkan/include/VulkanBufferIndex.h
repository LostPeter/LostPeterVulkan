/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-05-31
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VULKAN_BUFFER_INDEX_H_
#define _VULKAN_BUFFER_INDEX_H_

#include "VulkanBuffer.h"

namespace LostPeter
{
    class utilExport VulkanBufferIndex : public VulkanBuffer
    {
    public:
        VulkanBufferIndex(const String& strName);
        virtual ~VulkanBufferIndex();

    public:
        public:
        static uint32_t GetNextID() { return ++ms_nMaxID; }
    protected:
        static uint32_t ms_nMaxID;

    protected:
        VkIndexType m_vkIndexType;
        uint32_t m_nIndexCount;
        uint32_t m_nTriangleCount;

    public:   
        UTIL_FORCEINLINE VkIndexType GetVkIndexType() const { return m_vkIndexType; }
        UTIL_FORCEINLINE uint32_t GetIndexCount() const { return m_nIndexCount; }
        UTIL_FORCEINLINE uint32_t GetTriangleCount() const { return m_nTriangleCount; }

    public:
        virtual void Destroy();
        virtual bool Init(uint32_t sizeDataByte, uint32_t countData);
        virtual bool Init(void* pData, 
                          uint32_t sizeDataByte, 
                          uint32_t countData,
                          bool bIsDeleteData,
                          bool bIsUpdateData);
        virtual bool Init(FMeshDataPCT& data, bool bIsUpdateData);
        virtual bool Init(FMeshData& data, bool bIsUpdateData);
        virtual bool Init(FMeshDataUV2& data, bool bIsUpdateData);
        virtual bool Init(FMeshDataSkin& data, bool bIsUpdateData);

        virtual bool UpdateData(void* pData, uint32_t sizeDataByte, uint32_t countData, bool bIsDeleteData, bool bIsUpdateData);

        virtual VulkanBufferIndex* Clone();
    };

}; //LostPeter

#endif