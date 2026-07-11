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
        , pBuffer(nullptr)
		, bIsDelete(true)

        , poBuffer_Uniform(VK_NULL_HANDLE)
        , poBufferMemory_Uniform(VK_NULL_HANDLE)
    {

    }
    
    VKBufferUniform::~VKBufferUniform()
    {
        Destroy();
    }

    void VKBufferUniform::Destroy()
    {
		if (this->bIsDelete)
		{
			F_DELETE_T(this->pBuffer)
		}
        this->pBuffer = nullptr;

        if (this->poBuffer_Uniform != VK_NULL_HANDLE)
        {
            Base::GetWindowPtr()->destroyVkBuffer(this->poBuffer_Uniform, this->poBufferMemory_Uniform);
        }
        this->poBuffer_Uniform = VK_NULL_HANDLE;
        this->poBufferMemory_Uniform = VK_NULL_HANDLE;
    }

    bool VKBufferUniform::Init(size_t bufSize, 
							   uint8* pBuf,
							   bool isDelete)
    {
		this->nBufferSize = bufSize;
        this->pBuffer = pBuf;
        this->bIsDelete = isDelete;

		const String& nameBuffer = GetName();
		if (!Base::GetWindowPtr()->createUniformBuffer(nameBuffer,
													   bufSize, 
													   pBuf, 
													   this->poBuffer_Uniform,
													   this->poBufferMemory_Uniform))
        {
            F_LogError("*********************** VKBufferUniform::Init: Failed to create buffer uniform: [%s] !", nameBuffer.c_str());
            return false;
        }
		return true;
    }
    
    void VKBufferUniform::UpdateBuffer()
    {
        if (this->pBuffer == nullptr ||
            this->poBufferMemory_Uniform == VK_NULL_HANDLE)
        {
            return;
        }
        Base::GetWindowPtr()->updateVKBuffer(0, 
											 this->nBufferSize, 
											 this->pBuffer, 
											 this->poBufferMemory_Uniform);
    }
    void VKBufferUniform::UpdateBuffer(size_t offset, 
									   size_t bufSize, 
									   uint8* pBuf)
    {
        F_Assert(offset >= 0 && offset < this->nBufferSize && bufSize <= this->nBufferSize && "VKBufferUniform::UpdateBuffer")

        uint8* pData = this->pBuffer + offset;
        memcpy(pData, pBuf, bufSize);
        UpdateBuffer();
    }

}; //LostPeterVulkan