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

#ifndef _VK_BUFFER_INDIRECT_COMMAND_H_
#define _VK_BUFFER_INDIRECT_COMMAND_H_

#include "VKBuffer.h"

namespace LostPeterVulkan
{
    class vulkanExport VKBufferIndirectCommand : public VKBuffer
    {
    public:
        VKBufferIndirectCommand(const String& nameUniformBuffer);
        VKBufferIndirectCommand(const String& nameUniformBuffer, int count);
        virtual ~VKBufferIndirectCommand();

    public:
        std::vector<VkDrawIndexedIndirectCommand> indirectCommandCBs;

    public:
        F_FORCEINLINE int GetBufferSize() const { return (int)this->indirectCommandCBs.size() * sizeof(VkDrawIndexedIndirectCommand); }
        F_FORCEINLINE void* GetBuffer() const { return (void*)&this->indirectCommandCBs[0]; }

    public:
        virtual void Destroy();
        bool Init(int count);

    public:
        virtual void UpdateBuffer();
		virtual void UpdateBuffer(size_t offset, size_t bufSize, uint8* pBuf);

        void UpdateBuffer(int index, const VkDrawIndexedIndirectCommand& vkCmd);
		void UpdateBuffer(const std::vector<VkDrawIndexedIndirectCommand>& vkCmds);
    };

}; //LostPeterVulkan

#endif