/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-10-02
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/VKBufferUniform.h"
#include "../include/VulkanWindow.h"

namespace LostPeterVulkan
{
    VKBufferUniform::VKBufferUniform(const String& nameUniformBuffer)
        : VKBuffer(nameUniformBuffer)
    
        , nBufferSize(0)
        , pBuffer_Uniform(nullptr)
		, bIsDelete(true)
    {

    }
    
    VKBufferUniform::~VKBufferUniform()
    {
        Destroy();
    }

    void VKBufferUniform::Destroy()
    {
		VKBuffer::Destroy();

		if (this->bIsDelete)
		{
			F_DELETE_T(this->pBuffer_Uniform)
		}
        this->pBuffer_Uniform = nullptr;
    }

    bool VKBufferUniform::Init(size_t bufSize, 
							   uint8* pBuf,
							   bool isDelete)
    {
		this->nBufferSize = bufSize;
        this->pBuffer_Uniform = pBuf;
        this->bIsDelete = isDelete;

		if (!createVkBuffer(this->name, 
							bufSize, 
							VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, 
							VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT))
		{
			F_LogError("*********************** VKBufferUniform::Init: Failed to create buffer: [%s] !", this->name.c_str());
			return false;
		}
		UpdateBuffer();

		return true;
    }
    
    void VKBufferUniform::UpdateBuffer()
    {
		if (this->pBuffer_Uniform == nullptr)
			return;

		updateVkBuffer(0, 
					   GetBufferSize(), 
					   this->pBuffer_Uniform);
    }
    void VKBufferUniform::UpdateBuffer(size_t offset, 
									   size_t bufSize, 
									   uint8* pBuf)
    {
		size_t size = GetBufferSize();
        F_Assert(offset >= 0 && offset < size && bufSize <= size && "VKBufferUniform::UpdateBuffer")

        uint8* pData = this->pBuffer_Uniform + offset;
        memcpy(pData, pBuf, bufSize);
        UpdateBuffer();
    }

}; //LostPeterVulkan