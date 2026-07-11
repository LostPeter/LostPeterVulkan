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

#include "../include/VKBufferVertex.h"
#include "../include/VulkanWindow.h"

namespace LostPeterVulkan
{
    VKBufferVertex::VKBufferVertex(const String& nameBuffer)
        : Base(nameBuffer)

		, typeVertex()
		, nBufferSize(0)
		, pBuffer(nullptr)
		, bIsDelete(true)
		, bIsNeedUpdate(false)

		, poVertexBuffer(VK_NULL_HANDLE)
        , poVertexBufferMemory(VK_NULL_HANDLE)
        , poVertexBuffer_Staging(VK_NULL_HANDLE)
        , poVertexBufferMemory_Staging(VK_NULL_HANDLE)
    {

    }
    VKBufferVertex::~VKBufferVertex()
    {
		Destroy();
    }

	void VKBufferVertex::Destroy()
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
	}
		void VKBufferVertex::destroyBuffer()
		{
			if (this->bIsDelete && this->pBuffer != nullptr)
			{
				F_DELETE_T(this->pBuffer)
			}
			this->pBuffer = nullptr;
		}

	bool VKBufferVertex::Init(FMeshVertexType type,
							  size_t bufSize, 
							  uint8* pBuf,
							  bool bIsDelete,
							  bool bIsNeedUpdate)
	{
		this->typeVertex = type;
        this->nBufferSize = bufSize;
        this->pBuffer = pBuf;
        this->bIsDelete = bIsDelete;
		this->bIsNeedUpdate = bIsNeedUpdate;

		VulkanWindow* pWindow = Base::GetWindowPtr();
		const String& nameBuffer = GetName();
		if (bIsNeedUpdate) {
			if (!pWindow->createVertexBuffer(nameBuffer,
											 bufSize,
											 pBuf,
											 this->poVertexBuffer,
											 this->poVertexBufferMemory,
											 this->poVertexBuffer_Staging,
											 this->poVertexBufferMemory_Staging))
			{
				F_LogError("*********************** VKBufferVertex::Init: Failed to create buffer vertex: [%s] !", nameBuffer.c_str());
				return false;
			}
		} else {
			if (!pWindow->createVertexBuffer(nameBuffer,
											 bufSize,
											 pBuf,
											 this->poVertexBuffer,
											 this->poVertexBufferMemory))
			{
				F_LogError("*********************** VKBufferVertex::Init: Failed to create buffer vertex: [%s] !", nameBuffer.c_str());
				return false;
			}
		}
		return true;
	}
		
	void VKBufferVertex::UpdateVertex(size_t offset,
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

	void VKBufferVertex::BindVertexBuffer()
	{

	}

}; //LostPeterVulkan