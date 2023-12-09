/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-04
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VK_FRAME_BUFFER_ATTACHMENT_H_
#define _VK_FRAME_BUFFER_ATTACHMENT_H_

#include "Base.h"

namespace LostPeterVulkan
{
    class vulkanExport VKFrameBufferAttachment
    {
    public:
        VKFrameBufferAttachment();
        virtual ~VKFrameBufferAttachment();

    public:
        bool isDepth;
        bool isImageArray;

        VkImage image;
        VkDeviceMemory memory;
        VkImageView view;

    public:
        void Destroy();
        virtual void Init(uint32_t width, 
                          uint32_t height, 
                          bool _isDepth,
                          bool _isImageArray);
    };

}; //LostPeterVulkan

#endif