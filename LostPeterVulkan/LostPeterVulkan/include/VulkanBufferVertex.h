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

#ifndef _VULKAN_BUFFER_VERTEX_H_
#define _VULKAN_BUFFER_VERTEX_H_

#include "VulkanBuffer.h"

namespace LostPeter
{
    class utilExport VulkanBufferVertex : public VulkanBuffer
    {
    public:
        VulkanBufferVertex(const String& strName);
        virtual ~VulkanBufferVertex();

    public:
        static uint32_t GetNextID() { return ++ms_nMaxID; }
    protected:
        static uint32_t ms_nMaxID;

    protected:
        uint32_t m_nVertexCount;

    public:
        UTIL_FORCEINLINE uint32_t GetVertexCount() const { return m_nVertexCount; }

    public:
        virtual void Destroy();
        virtual bool Init(uint32_t sizeDataByte, uint32_t countData);
        virtual bool Init(void* pData, 
                          uint32_t sizeDataByte, 
                          uint32_t countData,
                          bool bIsDeleteData,
                          bool bIsUpdateData);
        virtual bool Init(MeshPCTData& data, bool bIsUpdateData);
        virtual bool Init(MeshData& data, bool bIsUpdateData);
        virtual bool Init(MeshUV2Data& data, bool bIsUpdateData);
        virtual bool Init(SkinMeshData& data, bool bIsUpdateData);

        virtual bool UpdateData(void* pData, uint32_t sizeDataByte, uint32_t countData, bool bIsDeleteData, bool bIsUpdateData);

        virtual VulkanBufferVertex* Clone();
    };

}; //LostPeter

#endif