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

#ifndef _CULL_BUFFER_INDIRECT_COMMAND_H_
#define _CULL_BUFFER_INDIRECT_COMMAND_H_

#include "Base.h"

namespace LostPeterVulkan
{
    class vulkanExport BufferIndirectCommand : public Base
    {
    public:
        BufferIndirectCommand(const String& nameUniformBuffer);
        BufferIndirectCommand(const String& nameUniformBuffer, int count);
        virtual ~BufferIndirectCommand();

    public:
        std::vector<VkDrawIndexedIndirectCommand> indirectCommandCBs;
        VkBuffer poBuffer_IndirectCommand;  
        VkDeviceMemory poBufferMemory_IndirectCommand;

    public:
        F_FORCEINLINE int GetBufferSize() const { return (int)this->indirectCommandCBs.size() * sizeof(VkDrawIndexedIndirectCommand); }
        
    public:
        void Destroy();
        void Init(int count);

    public:
        void UpdateBuffer();
        void UpdateBuffer(int index, const VkDrawIndexedIndirectCommand& vkCmd);

    };

}; //LostPeterVulkan

#endif