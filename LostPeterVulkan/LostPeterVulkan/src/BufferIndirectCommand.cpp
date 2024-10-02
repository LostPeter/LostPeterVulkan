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

#include "../include/BufferIndirectCommand.h"
#include "../include/VulkanWindow.h"

namespace LostPeterVulkan
{
    BufferIndirectCommand::BufferIndirectCommand(const String& nameUniformBuffer)
        : Base(nameUniformBuffer)

        , poBuffer_IndirectCommand(VK_NULL_HANDLE)
        , poBufferMemory_IndirectCommand(VK_NULL_HANDLE)
    {

    }

    BufferIndirectCommand::BufferIndirectCommand(const String& nameUniformBuffer, int count)
        : Base(nameUniformBuffer)
        
        , poBuffer_IndirectCommand(VK_NULL_HANDLE)
        , poBufferMemory_IndirectCommand(VK_NULL_HANDLE)
    {
        Init(count);
    }
    
    BufferIndirectCommand::~BufferIndirectCommand()
    {
        Destroy();
    }

    void BufferIndirectCommand::Destroy()
    {
        this->indirectCommandCBs.clear();
        if (this->poBuffer_IndirectCommand != VK_NULL_HANDLE)
        {
            Base::GetWindowPtr()->destroyVkBuffer(this->poBuffer_IndirectCommand, this->poBufferMemory_IndirectCommand);
        }
        this->poBuffer_IndirectCommand = VK_NULL_HANDLE;
        this->poBufferMemory_IndirectCommand = VK_NULL_HANDLE;
    }

    void BufferIndirectCommand::Init(int count)
    {
        Destroy();

        if (count <= 0)
            return;

        this->indirectCommandCBs.resize(count);
        VkDeviceSize bufferSize = count * sizeof(VkDrawIndexedIndirectCommand);
        Base::GetWindowPtr()->createVkBuffer(this->name, bufferSize, VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, this->poBuffer_IndirectCommand, this->poBufferMemory_IndirectCommand);
        UpdateBuffer();
    }
    
    void BufferIndirectCommand::UpdateBuffer()
    {
        if (this->poBufferMemory_IndirectCommand == VK_NULL_HANDLE)
        {
            return;
        }
        Base::GetWindowPtr()->updateVKBuffer(0, (size_t)GetBufferSize(), &this->indirectCommandCBs[0], this->poBufferMemory_IndirectCommand);
    }
    void BufferIndirectCommand::UpdateBuffer(int index, const VkDrawIndexedIndirectCommand& vkCmd)
    {
        F_Assert(index >= 0 && index < (int)this->indirectCommandCBs.size() && "BufferIndirectCommand::UpdateBuffer")
        
        this->indirectCommandCBs[index] = vkCmd;
        UpdateBuffer();
    }

}; //LostPeterVulkan