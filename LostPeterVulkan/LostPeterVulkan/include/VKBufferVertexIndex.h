/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2026-07-11
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VK_BUFFER_VERTEX_INDEX_H_
#define _VK_BUFFER_VERTEX_INDEX_H_

#include "Base.h"

namespace LostPeterVulkan
{
    class vulkanExport VKBufferVertexIndex : public Base
    {
    public:
        VKBufferVertexIndex(const String& nameBuffer);
        virtual ~VKBufferVertexIndex();

    public:
        FMeshVertexType typeVertex;
        size_t nBufferSize_Vertex;
        uint8* pBuffer_Vertex;
        bool bIsDelete_Vertex;
        size_t nBufferSize_Index;
        uint8* pBuffer_Index;
        bool bIsDelete_Index;
		bool bIsNeedUpdate;

		VkBuffer poVertexBuffer;
        VkDeviceMemory poVertexBufferMemory;
        VkBuffer poVertexBuffer_Staging;
        VkDeviceMemory poVertexBufferMemory_Staging;

		VkBuffer poIndexBuffer;
        VkDeviceMemory poIndexBufferMemory;
        VkBuffer poIndexBuffer_Staging;
        VkDeviceMemory poIndexBufferMemory_Staging;

	public:
		F_FORCEINLINE const VkBuffer& GetVKBufferVertex() const { return this->poVertexBuffer; }
		F_FORCEINLINE const VkDeviceMemory& GetVKBufferVertexMemory() const { return this->poVertexBufferMemory; }
		F_FORCEINLINE const VkBuffer& GetVKBufferIndex() const { return this->poIndexBuffer; }
		F_FORCEINLINE const VkDeviceMemory& GetVKBufferIndexMemory() const { return this->poIndexBufferMemory; }

    public:
        virtual void Destroy();
        virtual bool Init(FMeshVertexType type,
                          size_t bufSize_Vertex, 
                          uint8* pBuf_Vertex,
                          bool isDelete_Vertex,
                          size_t bufSize_Index, 
                          uint8* pBuf_Index,
                          bool isDelete_Index,
						  bool bIsNeedUpdate);

		virtual void UpdateVertex(size_t offset,
								  size_t bufSize,
								  uint8* pBuf);
		virtual void UpdateIndex(size_t offset,
								 size_t bufSize,
								 uint8* pBuf);

    public:
        void BindVertexIndexBuffer(VkCommandBuffer& commandBuffer);

    protected:
        virtual void destroyBuffer();
		
    };

}; //LostPeterVulkan

#endif