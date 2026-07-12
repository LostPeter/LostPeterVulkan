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

#ifndef _VK_BUFFER_VERTEX_H_
#define _VK_BUFFER_VERTEX_H_

#include "Base.h"

namespace LostPeterVulkan
{
    class vulkanExport VKBufferVertex : public Base
    {
    public:
        VKBufferVertex(const String& nameBuffer);
        virtual ~VKBufferVertex();

    public:
		FMeshVertexType typeVertex;
        size_t nBufferSize;
        uint8* pBuffer;
        bool bIsDelete;
		bool bIsNeedUpdate;

		VkBuffer poVertexBuffer;
        VkDeviceMemory poVertexBufferMemory;
        VkBuffer poVertexBuffer_Staging;
        VkDeviceMemory poVertexBufferMemory_Staging;

	public:
		F_FORCEINLINE const VkBuffer& GetVKBufferVertex() const { return this->poVertexBuffer; }
		F_FORCEINLINE const VkDeviceMemory& GetVKBufferVertexMemory() const { return this->poVertexBufferMemory; }

    public:
        virtual void Destroy();
		virtual bool Init(FMeshVertexType type,
                          size_t bufSize, 
                          uint8* pBuf,
                          bool bIsDelete,
						  bool bIsNeedUpdate);
		
		virtual void UpdateVertex(size_t offset,
								  size_t bufSize,
								  uint8* pBuf);

	public:
		void BindVertexBuffer(VkCommandBuffer& commandBuffer);

    protected:
        virtual void destroyBuffer();

    };

}; //LostPeterVulkan

#endif