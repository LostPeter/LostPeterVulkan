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

#include "../include/VKBufferVertexIndex.h"
#include "../include/VulkanWindow.h"

namespace LostPeterVulkan
{
    VKBufferVertexIndex::VKBufferVertexIndex(const String& nameBuffer)
        : Base(nameBuffer)

		, typeVertex(F_MeshVertex_Pos3Color4Normal3Tangent3Tex2)
        , nBufferSize_Vertex(0)
        , pBuffer_Vertex(nullptr)
        , bIsDelete_Vertex(true)
        , nBufferSize_Index(0)
        , pBuffer_Index(nullptr)
        , bIsDelete_Index(true)
		, bIsNeedUpdate(false)

		, poVertexBuffer(VK_NULL_HANDLE)
        , poVertexBufferMemory(VK_NULL_HANDLE)
        , poVertexBuffer_Staging(VK_NULL_HANDLE)
        , poVertexBufferMemory_Staging(VK_NULL_HANDLE)

		, poIndexBuffer(VK_NULL_HANDLE)
        , poIndexBufferMemory(VK_NULL_HANDLE)
        , poIndexBuffer_Staging(VK_NULL_HANDLE)
        , poIndexBufferMemory_Staging(VK_NULL_HANDLE)
    {

    }
    VKBufferVertexIndex::~VKBufferVertexIndex()
    {
		Destroy();
    }

	void VKBufferVertexIndex::Destroy()
	{
		destroyBuffer();
		//Vertex
        if (this->poVertexBuffer != VK_NULL_HANDLE)
        {
            Base::GetWindowPtr()->destroyVkBuffer(this->poVertexBuffer, this->poVertexBufferMemory);
        }
        this->poVertexBuffer = VK_NULL_HANDLE;
        this->poVertexBufferMemory = VK_NULL_HANDLE;

        if (this->poVertexBuffer_Staging != VK_NULL_HANDLE)
        {
            Base::GetWindowPtr()->destroyVkBuffer(this->poVertexBuffer_Staging, this->poVertexBufferMemory_Staging);
        }
        this->poVertexBuffer_Staging = VK_NULL_HANDLE;
        this->poVertexBufferMemory_Staging = VK_NULL_HANDLE;

        //Index
        if (this->poIndexBuffer != VK_NULL_HANDLE)
        {
            Base::GetWindowPtr()->destroyVkBuffer(this->poIndexBuffer, this->poIndexBufferMemory);
        }
        this->poIndexBuffer = VK_NULL_HANDLE;
        this->poIndexBufferMemory = VK_NULL_HANDLE;

        if (this->poIndexBuffer_Staging != VK_NULL_HANDLE)
        {
            Base::GetWindowPtr()->destroyVkBuffer(this->poIndexBuffer_Staging, this->poIndexBufferMemory_Staging);
        }
        this->poIndexBuffer_Staging = VK_NULL_HANDLE;
        this->poIndexBufferMemory_Staging = VK_NULL_HANDLE;
	}
		void VKBufferVertexIndex::destroyBuffer()
		{
			if (this->bIsDelete_Vertex && this->pBuffer_Vertex != nullptr)
            {
                F_DELETE_T(this->pBuffer_Vertex)
            }
            this->pBuffer_Vertex = nullptr;
            if (this->bIsDelete_Index && this->pBuffer_Index != nullptr)
            {
                F_DELETE_T(this->pBuffer_Index)
            }
            this->pBuffer_Index = nullptr;
		}

	bool VKBufferVertexIndex::Init(FMeshVertexType type,
								   size_t bufSize_Vertex, 
								   uint8* pBuf_Vertex,
								   bool isDelete_Vertex,
								   size_t bufSize_Index, 
								   uint8* pBuf_Index,
								   bool isDelete_Index,
								   bool bIsNeedUpdate)
	{
		this->typeVertex = type;
        this->nBufferSize_Vertex = bufSize_Vertex;
        this->pBuffer_Vertex = pBuf_Vertex;
        this->bIsDelete_Vertex = isDelete_Vertex;
        this->nBufferSize_Index = bufSize_Index;
        this->pBuffer_Index = pBuf_Index;
        this->bIsDelete_Index = isDelete_Index;
		this->bIsNeedUpdate = bIsNeedUpdate;

		VulkanWindow* pWindow = Base::GetWindowPtr();
		const String& nameBuffer = GetName();
		if (bIsNeedUpdate) {
			//Vertex
			if (!pWindow->createVertexBuffer(nameBuffer,
											 bufSize_Vertex,
											 pBuf_Vertex,
											 this->poVertexBuffer,
											 this->poVertexBufferMemory,
											 this->poVertexBuffer_Staging,
											 this->poVertexBufferMemory_Staging))
			{
				F_LogError("*********************** VKBufferVertexIndex::Init: Failed to create buffer vertex: [%s] !", nameBuffer.c_str());
				return false;
			}

			//Index
			if (!pWindow->createIndexBuffer(nameBuffer,
											bufSize_Index, 
											pBuf_Index, 
											this->poIndexBuffer, 
											this->poIndexBufferMemory,
											this->poIndexBuffer_Staging,
										    this->poIndexBufferMemory_Staging))
			{
				F_LogError("*********************** VKBufferVertexIndex::Init: Failed to create buffer index: [%s] !", nameBuffer.c_str());
				return false;
			}

		} else {
			//Vertex
			if (!pWindow->createVertexBuffer(nameBuffer,
											 bufSize_Vertex,
											 pBuf_Vertex,
											 this->poVertexBuffer,
											 this->poVertexBufferMemory))
			{
				F_LogError("*********************** VKBufferVertexIndex::Init: Failed to create buffer vertex: [%s] !", nameBuffer.c_str());
				return false;
			}

			//Index
			if (!pWindow->createIndexBuffer(nameBuffer,
											bufSize_Index, 
											pBuf_Index, 
											this->poIndexBuffer, 
											this->poIndexBufferMemory))
			{
				F_LogError("*********************** VKBufferVertexIndex::Init: Failed to create buffer index: [%s] !", nameBuffer.c_str());
				return false;
			}
		}
        return true;
	}

	void VKBufferVertexIndex::UpdateVertex(size_t offset,
										   size_t bufSize,
										   uint8* pBuf)
	{
		if (!this->bIsNeedUpdate ||
            this->poVertexBuffer_Staging == VK_NULL_HANDLE ||
            this->poVertexBufferMemory_Staging == VK_NULL_HANDLE)
            return;
        
        Base::GetWindowPtr()->updateVKBuffer(offset, bufSize, pBuf, this->poVertexBufferMemory_Staging);
        Base::GetWindowPtr()->copyVkBuffer(this->poVertexBuffer_Staging, this->poVertexBuffer, bufSize);

	}
	void VKBufferVertexIndex::UpdateIndex(size_t offset,
										  size_t bufSize,
										  uint8* pBuf)
	{
		if (!this->bIsNeedUpdate ||
            this->poIndexBuffer_Staging == VK_NULL_HANDLE ||
            this->poIndexBufferMemory_Staging == VK_NULL_HANDLE)
            return;

        Base::GetWindowPtr()->updateVKBuffer(offset, bufSize, pBuf, this->poIndexBufferMemory_Staging);
        Base::GetWindowPtr()->copyVkBuffer(this->poIndexBuffer_Staging, this->poIndexBuffer, bufSize);

	}

	void VKBufferVertexIndex::BindVertexIndexBuffer(VkCommandBuffer& commandBuffer)
	{
		VkBuffer vertexBuffers[] = { this->poVertexBuffer };
		VkDeviceSize offsets[] = { 0 };
		Base::GetWindowPtr()->bindVertexBuffer(commandBuffer, 0, 1, vertexBuffers, offsets);
		Base::GetWindowPtr()->bindIndexBuffer(commandBuffer, this->poIndexBuffer, 0, VK_INDEX_TYPE_UINT32);
	}

}; //LostPeterVulkan