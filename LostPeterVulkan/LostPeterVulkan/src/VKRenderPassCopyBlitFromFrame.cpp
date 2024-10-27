/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-10-27
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/VKRenderPassCopyBlitFromFrame.h"
#include "../include/VulkanWindow.h"

namespace LostPeterVulkan
{
    VKRenderPassCopyBlitFromFrame::VKRenderPassCopyBlitFromFrame(const String& nameRenderPass)
        //Window
        : Base(nameRenderPass)

        //Image
        , poImage(VK_NULL_HANDLE)
        , poImageMemory(VK_NULL_HANDLE)
        , poImageView(VK_NULL_HANDLE)
        , poSampler(VK_NULL_HANDLE)

        , poRenderPass(VK_NULL_HANDLE)
        , poFrameBuffer(VK_NULL_HANDLE)

    {

    }
    VKRenderPassCopyBlitFromFrame::~VKRenderPassCopyBlitFromFrame()
    {
        Destroy();
    }   
    void VKRenderPassCopyBlitFromFrame::Destroy()
    {
        //RenderPass
        if (this->poRenderPass != VK_NULL_HANDLE)
        {
            Base::GetWindowPtr()->destroyVkRenderPass(this->poRenderPass);
        }
        this->poRenderPass = VK_NULL_HANDLE;

        //FrameBuffer
        if (this->poFrameBuffer != VK_NULL_HANDLE)
        {
            Base::GetWindowPtr()->destroyVkFramebuffer(this->poFrameBuffer);
        }
        this->poFrameBuffer = VK_NULL_HANDLE;

        //Depth
        if (this->poImage != VK_NULL_HANDLE)
        {
            Base::GetWindowPtr()->destroyVkImage(this->poImage, this->poImageMemory, this->poImageView);
        }
        this->poImage = VK_NULL_HANDLE;
        this->poImageMemory = VK_NULL_HANDLE;
        this->poImageView = VK_NULL_HANDLE;
        if (this->poSampler != VK_NULL_HANDLE)
        {
            Base::GetWindowPtr()->destroyVkImageSampler(this->poSampler);
        }
        this->poSampler = VK_NULL_HANDLE;

        this->imageInfo.imageView = VK_NULL_HANDLE;
        this->imageInfo.sampler = VK_NULL_HANDLE;
    } 
    bool VKRenderPassCopyBlitFromFrame::Init(uint32_t width,
                                             uint32_t height,
                                             VkFormat format,
                                             bool isDepth)
    {
        //0> Common
        {
            this->offset.x = 0;
            this->offset.y = 0;
            this->extent.width = width;
            this->extent.height = height;
            this->aClearValue.clear();
            VkClearValue clearValue = {};
            clearValue.depthStencil = { 1.0f, 0 };
            this->aClearValue.push_back(clearValue);

            this->viewPort.x = 0.0f;
            this->viewPort.y = 0.0f;
            this->viewPort.width = (float)width;
            this->viewPort.height = (float)height;
            this->viewPort.minDepth = 0.0f;
            this->viewPort.maxDepth = 1.0f;
            this->rtScissor.offset.x = 0;
            this->rtScissor.offset.y = 0;
            this->rtScissor.extent.width = width;
            this->rtScissor.extent.height = height;
        }

        VkImageUsageFlags usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
        VkImageLayout imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        if (isDepth)
        {
            usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
            imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
        }

        //1> Image
        {
            String nameTexture = "Texture-CopyBlitFromFrame-" + this->name;
            Base::GetWindowPtr()->createVkImage(nameTexture,
                                                width, 
                                                height, 
                                                1,
                                                1,
                                                1,
                                                VK_IMAGE_TYPE_2D, 
                                                false,
                                                VK_SAMPLE_COUNT_1_BIT, 
                                                format, 
                                                VK_IMAGE_TILING_OPTIMAL, 
                                                usage,
                                                VK_SHARING_MODE_EXCLUSIVE,
                                                false,
                                                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
                                                this->poImage, 
                                                this->poImageMemory);
            
            Base::GetWindowPtr()->createVkImageView(nameTexture,
                                                    this->poImage, 
                                                    VK_IMAGE_VIEW_TYPE_2D,
                                                    format, 
                                                    VK_IMAGE_ASPECT_DEPTH_BIT,
                                                    1, 
                                                    1,
                                                    this->poImageView);

            Base::GetWindowPtr()->createVkSampler(nameTexture,
                                                  F_TextureFilter_Bilinear, 
                                                  F_TextureAddressing_Clamp,
                                                  F_TextureBorderColor_OpaqueWhite,
                                                  false,
                                                  1.0f,
                                                  0.0f,
                                                  1.0f,
                                                  0.0f,
                                                  this->poSampler);
            
            this->imageInfo.sampler = this->poSampler;
            this->imageInfo.imageView = this->poImageView;
            this->imageInfo.imageLayout = imageLayout;
        }

        //2> RenderPass
        {
            std::vector<VkAttachmentDescription> aAttachmentDescription;
            std::vector<VkSubpassDescription> aSubpassDescription;
            std::vector<VkSubpassDependency> aSubpassDependency;
            
            //VkAttachmentDescription
            VkAttachmentDescription attachmentSR = {};
            Base::GetWindowPtr()->createAttachmentDescription(attachmentSR,
                                                              0,
                                                              format,
                                                              VK_SAMPLE_COUNT_1_BIT,
                                                              VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                                              VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                                              VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                                              VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                                              VK_IMAGE_LAYOUT_UNDEFINED,
                                                              imageLayout);
            aAttachmentDescription.push_back(attachmentSR);

            //VkSubpassDescription
            VkAttachmentReference attachRef = {};
            attachRef.attachment = 0;
            if (!isDepth)
                attachRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            else
                attachRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

            VkSubpassDescription subpass_SceneRender = {};
            subpass_SceneRender.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
            subpass_SceneRender.colorAttachmentCount = 0;
            subpass_SceneRender.pColorAttachments = !isDepth ? &attachRef : nullptr;
            subpass_SceneRender.pDepthStencilAttachment = isDepth ? &attachRef : nullptr;
            aSubpassDescription.push_back(subpass_SceneRender);
            
            //VkSubpassDependency
            VkSubpassDependency subpassDependency0 = {};
            subpassDependency0.srcSubpass = VK_SUBPASS_EXTERNAL;
            subpassDependency0.dstSubpass = 0;
            subpassDependency0.srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
            subpassDependency0.dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
            subpassDependency0.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
            subpassDependency0.dstAccessMask = isDepth ? VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT : VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            subpassDependency0.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
            aSubpassDependency.push_back(subpassDependency0);

            VkSubpassDependency subpassDependency1 = {};
            subpassDependency1.srcSubpass = 0;
            subpassDependency1.dstSubpass = VK_SUBPASS_EXTERNAL;
            subpassDependency1.srcStageMask = VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
            subpassDependency1.dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
            subpassDependency1.srcAccessMask = isDepth ? VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT : VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            subpassDependency1.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
            subpassDependency1.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
            aSubpassDependency.push_back(subpassDependency1);

            String nameRenderPass = "RenderPass-" + GetName();
            if (!Base::GetWindowPtr()->createVkRenderPass(nameRenderPass,
                                                          aAttachmentDescription,
                                                          aSubpassDescription,
                                                          aSubpassDependency,
                                                          nullptr,
                                                          this->poRenderPass))
            {
                String msg = "*********************** VKRenderPassCopyBlitFromFrame::Init: Failed to create renderpass: " + nameRenderPass;
                F_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }
        }

        //3> Framebuffer
        {
            VkImageViewVector aImageViews;
            aImageViews.push_back(this->poImageView);
            String nameFramebuffer = "Framebuffer-" + GetName();
            if (!Base::GetWindowPtr()->createVkFramebuffer(nameFramebuffer,
                                                           aImageViews, 
                                                           this->poRenderPass,
                                                           0,
                                                           width,
                                                           height,
                                                           1,
                                                           this->poFrameBuffer))
            {
                String msg = "*********************** VKRenderPassCopyBlitFromFrame::Init: Failed to create framebuffer: " + nameFramebuffer;
                F_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }
        }

        return true;
    }
    void VKRenderPassCopyBlitFromFrame::CleanupSwapChain()
    {
        Destroy();
    }
    void VKRenderPassCopyBlitFromFrame::RecreateSwapChain()
    {

    }

}; //LostPeterVulkan