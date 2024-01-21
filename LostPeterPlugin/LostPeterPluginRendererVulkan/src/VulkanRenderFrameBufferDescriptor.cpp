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

#include "../include/VulkanRenderFrameBufferDescriptor.h"
#include "../include/VulkanDevice.h"
#include "../include/VulkanSwapChain.h"
#include "../include/VulkanConverter.h"

namespace LostPeterPluginRendererVulkan
{
    VulkanRenderFrameBufferDescriptor::VulkanRenderFrameBufferDescriptor(const String& nameRenderFrameBufferDescriptor,
                                                                         VulkanDevice* pDevice,
                                                                         VulkanSwapChain* pSwapChain)
        : RenderFrameBufferDescriptor(nameRenderFrameBufferDescriptor)
        , m_pDevice(pDevice)
        , m_pSwapChain(pSwapChain)
        , m_pRenderPass(nullptr)
        , m_pFrameBuffer(nullptr)
    {
        F_Assert(m_pDevice != nullptr && m_pSwapChain != nullptr && "VulkanRenderFrameBufferDescriptor::VulkanRenderFrameBufferDescriptor");
    }

    VulkanRenderFrameBufferDescriptor::~VulkanRenderFrameBufferDescriptor()
    {
        Destroy();
    }
    
    void VulkanRenderFrameBufferDescriptor::Destroy()
    {
        
    }

    bool VulkanRenderFrameBufferDescriptor::Init(VkFormat typeFormatColorImage,
                                                 VkFormat typeFormatDepthImage,
                                                 VkSampleCountFlagBits vkSampleCountFlagBits,
                                                 bool bHasImgui)
    {   
        //1> Texture Color/Depth/SwapChain


        
        //2> RenderPass



        //3> FrameBuffer



        return true;
    }
    bool VulkanRenderFrameBufferDescriptor::createTextureColor()
    {

        return true;
    }
    bool VulkanRenderFrameBufferDescriptor::createTextureDepth()    
    {

        return true;
    }

    bool VulkanRenderFrameBufferDescriptor::createRenderPass()
    {

        return true;
    }
    bool VulkanRenderFrameBufferDescriptor::createRenderFrameBuffer()
    {
        
        return true;
    }


}; //LostPeterPluginRendererVulkan