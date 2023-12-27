/****************************************************************************
* LostPeterPluginRendererVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-12-27
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VULKAN_FRAME_BUFFER_DESCRIPTOR_H_
#define _VULKAN_FRAME_BUFFER_DESCRIPTOR_H_

#include "VulkanPreDefine.h"

namespace LostPeterPluginRendererVulkan
{
    class VulkanFrameBufferDescriptor : public Base
    {
    public:
        VulkanFrameBufferDescriptor(const String& nameFrameBufferDescriptor,
                                    VulkanDevice* pDevice);
        virtual ~VulkanFrameBufferDescriptor();

    public:
    protected:
        VulkanDevice* m_pDevice;

       

    public:
        F_FORCEINLINE VulkanDevice* GetDevice() const { return m_pDevice; }
        


    public:
        void Destroy();

        virtual bool Init();
    };

}; //LostPeterPluginRendererVulkan

#endif