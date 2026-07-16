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

#include "../include/VKBufferIndirectCommand.h"
#include "../include/VulkanWindow.h"

namespace LostPeterVulkan
{
    VKBufferIndirectCommand::VKBufferIndirectCommand(const String& nameUniformBuffer)
        : VKBuffer(nameUniformBuffer)

    {

    }

    VKBufferIndirectCommand::VKBufferIndirectCommand(const String& nameUniformBuffer, int count)
        : VKBuffer(nameUniformBuffer)
        
    {
        Init(count);
    }
    
    VKBufferIndirectCommand::~VKBufferIndirectCommand()
    {
        Destroy();
    }

    void VKBufferIndirectCommand::Destroy()
    {
		VKBuffer::Destroy();

        this->indirectCommandCBs.clear();
    }

    bool VKBufferIndirectCommand::Init(int count)
    {
        Destroy();
        if (count <= 0)
        {
			F_LogError("*********************** VKBufferIndirectCommand::Init: Empty buffer: [%s] !", this->name.c_str());
			return false;
		}

        this->indirectCommandCBs.resize(count);
        VkDeviceSize bufferSize = count * sizeof(VkDrawIndexedIndirectCommand);

		if (!createVkBuffer(this->name, 
							bufferSize, 
							VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT, 
							VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT))
		{
			F_LogError("*********************** VKBufferIndirectCommand::Init: Failed to create buffer: [%s] !", this->name.c_str());
			return false;
		}
        UpdateBuffer();

		return true;
    }
    
    void VKBufferIndirectCommand::UpdateBuffer()
    {
        if (this->indirectCommandCBs.size() <= 0)
            return;

		updateVkBuffer(0, 
					   (size_t)GetBufferSize(), 
					   &this->indirectCommandCBs[0]);
    }
	void VKBufferIndirectCommand::UpdateBuffer(size_t offset, size_t bufSize, uint8* pBuf)
	{
		F_Assert(false && "VKBufferIndirectCommand::UpdateBuffer")
	}

    void VKBufferIndirectCommand::UpdateBuffer(int index, const VkDrawIndexedIndirectCommand& vkCmd)
    {
        F_Assert(index >= 0 && index < (int)this->indirectCommandCBs.size() && "VKBufferIndirectCommand::UpdateBuffer")
        
        this->indirectCommandCBs[index] = vkCmd;
        UpdateBuffer();
    }
	void VKBufferIndirectCommand::UpdateBuffer(const std::vector<VkDrawIndexedIndirectCommand>& vkCmds)
	{
		F_Assert(vkCmds.size() <= this->indirectCommandCBs.size() && "VKBufferIndirectCommand::UpdateBuffer")
		
		for (size_t i = 0; i < vkCmds.size(); i++)
		{
			this->indirectCommandCBs[i] = vkCmds[i];
		}
		UpdateBuffer();
	}

}; //LostPeterVulkan