/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-06-22
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/VKShadowMapRenderPass.h"
#include "../include/VulkanWindow.h"

namespace LostPeterVulkan
{
    VKShadowMapRenderPass::VKShadowMapRenderPass(const String& _nameRenderPass)
        //Window
        : Base(_nameRenderPass)

        //Depth
        , poDepthImage(VK_NULL_HANDLE)
        , poDepthImageMemory(VK_NULL_HANDLE)
        , poDepthImageView(VK_NULL_HANDLE)
        , sampler(VK_NULL_HANDLE)

        //RenderPass
        , poRenderPass(VK_NULL_HANDLE)

        //FrameBuffer
        , poFrameBuffer(VK_NULL_HANDLE)
    {

    }
    VKShadowMapRenderPass::~VKShadowMapRenderPass()
    {
        Destroy();
    }   
    void VKShadowMapRenderPass::Destroy()
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
        if (this->poDepthImage != VK_NULL_HANDLE)
        {
            Base::GetWindowPtr()->destroyVkImage(this->poDepthImage, this->poDepthImageMemory, this->poDepthImageView);
        }
        this->poDepthImage = VK_NULL_HANDLE;
        this->poDepthImageMemory = VK_NULL_HANDLE;
        this->poDepthImageView = VK_NULL_HANDLE;
        if (this->sampler != VK_NULL_HANDLE)
        {
            Base::GetWindowPtr()->destroyVkImageSampler(this->sampler);
        }
        this->sampler = VK_NULL_HANDLE;

        this->imageInfo.imageView = VK_NULL_HANDLE;
        this->imageInfo.sampler = VK_NULL_HANDLE;
    } 
    void VKShadowMapRenderPass::Init(uint32_t width, 
                                     uint32_t height,
                                     VkFormat format)
    {
        //1> Depth
        {
            Base::GetWindowPtr()->createVkImage(width, 
                                                height, 
                                                1,
                                                1,
                                                1,
                                                VK_IMAGE_TYPE_2D, 
                                                false,
                                                VK_SAMPLE_COUNT_1_BIT, 
                                                format, 
                                                VK_IMAGE_TILING_OPTIMAL, 
                                                VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
                                                VK_SHARING_MODE_EXCLUSIVE,
                                                false,
                                                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
                                                this->poDepthImage, 
                                                this->poDepthImageMemory);
            
            Base::GetWindowPtr()->createVkImageView(this->poDepthImage, 
                                                    VK_IMAGE_VIEW_TYPE_2D,
                                                    format, 
                                                    VK_IMAGE_ASPECT_COLOR_BIT, 
                                                    1, 
                                                    1,
                                                    this->poDepthImageView);

            Base::GetWindowPtr()->createVkSampler(F_TextureFilter_Bilinear, 
                                                  F_TextureAddressing_Clamp,
                                                  F_TextureBorderColor_OpaqueWhite,
                                                  false,
                                                  1.0f,
                                                  0.0f,
                                                  1.0f,
                                                  0.0f,
                                                  this->sampler);
            
            this->imageInfo.sampler = this->sampler;
            this->imageInfo.imageView = this->poDepthImageView;
            this->imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        }

        //2> RenderPass
        {
            std::vector<VkAttachmentDescription> aAttachmentDescription;
            std::vector<VkSubpassDescription> aSubpassDescription;
            std::vector<VkSubpassDependency> aSubpassDependency;
            
            //VkAttachmentDescription Depth
            VkAttachmentDescription attachmentSR_Depth = {};
            Base::GetWindowPtr()->createAttachmentDescription(attachmentSR_Depth,
                                                              0,
                                                              format,
                                                              VK_SAMPLE_COUNT_1_BIT,
                                                              VK_ATTACHMENT_LOAD_OP_CLEAR,
                                                              VK_ATTACHMENT_STORE_OP_STORE,
                                                              VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                                              VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                                              VK_IMAGE_LAYOUT_UNDEFINED,
                                                              VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL);
            aAttachmentDescription.push_back(attachmentSR_Depth);

            //VkSubpassDescription
            VkAttachmentReference attachRef_Depth = {};
            attachRef_Depth.attachment = 0;
            attachRef_Depth.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

            VkSubpassDescription subpass_SceneRender = {};
            subpass_SceneRender.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
            subpass_SceneRender.colorAttachmentCount = 0;
            subpass_SceneRender.pColorAttachments = nullptr;
            subpass_SceneRender.pDepthStencilAttachment = &attachRef_Depth;
            aSubpassDescription.push_back(subpass_SceneRender);
            
            //VkSubpassDependency
            VkSubpassDependency subpassDependency0 = {};
            subpassDependency0.srcSubpass = VK_SUBPASS_EXTERNAL;
            subpassDependency0.dstSubpass = 0;
            subpassDependency0.srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
            subpassDependency0.dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
            subpassDependency0.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
            subpassDependency0.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
            subpassDependency0.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
            aSubpassDependency.push_back(subpassDependency0);

            VkSubpassDependency subpassDependency1 = {};
            subpassDependency1.srcSubpass = 0;
            subpassDependency1.dstSubpass = VK_SUBPASS_EXTERNAL;
            subpassDependency1.srcStageMask = VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
            subpassDependency1.dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
            subpassDependency1.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
            subpassDependency1.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
            subpassDependency1.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
            aSubpassDependency.push_back(subpassDependency1);


            if (!Base::GetWindowPtr()->createVkRenderPass(GetName(),
                                                          aAttachmentDescription,
                                                          aSubpassDescription,
                                                          aSubpassDependency,
                                                          nullptr,
                                                          this->poRenderPass))
            {
                String msg = "*********************** VKShadowMapRenderPass::Init: Failed to create renderpass: " + GetName();
                F_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }
        }

        //3> Framebuffer
        {
            VkImageViewVector aImageViews;
            aImageViews.push_back(this->poDepthImageView);
            if (!Base::GetWindowPtr()->createVkFramebuffer(GetName(),
                                                           aImageViews, 
                                                           this->poRenderPass,
                                                           0,
                                                           width,
                                                           height,
                                                           1,
                                                           this->poFrameBuffer))
            {
                String msg = "*********************** VKShadowMapRenderPass::Init: Failed to create framebuffer: " + GetName();
                F_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }
        }
    }
    void VKShadowMapRenderPass::CleanupSwapChain()
    {
        Destroy();
    }
    void VKShadowMapRenderPass::RecreateSwapChain()
    {

    }

}; //LostPeterVulkan