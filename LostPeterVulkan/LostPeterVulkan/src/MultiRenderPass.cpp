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

#include "../include/MultiRenderPass.h"
#include "../include/VulkanWindow.h"

namespace LostPeter
{
    FrameBufferAttachment::FrameBufferAttachment()
        : isDepth(false)
        , isImageArray(false)
        , image(VK_NULL_HANDLE)
        , memory(VK_NULL_HANDLE)
        , view(VK_NULL_HANDLE)
    {

    }
    FrameBufferAttachment::~FrameBufferAttachment()
    {

    }
    void FrameBufferAttachment::Destroy()
    {
        if (this->image != VK_NULL_HANDLE)
        {
            Base::GetWindowPtr()->destroyVkImage(this->image, this->memory, this->view);
        }
        this->image = VK_NULL_HANDLE;
        this->memory = VK_NULL_HANDLE;
        this->view = VK_NULL_HANDLE;
    }
    void FrameBufferAttachment::Init(uint32_t width, 
                                     uint32_t height, 
                                     bool _isDepth,
                                     bool _isImageArray)
    {
        this->isDepth = _isDepth;
        this->isImageArray = _isImageArray;

        uint32_t depth = 1;
        uint32_t numArray = 1;
        uint32_t mipMapCount = 1;
        VkImageType imageType = VK_IMAGE_TYPE_2D;
        VkSampleCountFlagBits numSamples = Base::GetWindowPtr()->poMSAASamples;
        VkFormat format = Base::GetWindowPtr()->poSwapChainImageFormat;
        VkImageTiling tiling = VK_IMAGE_TILING_OPTIMAL;
        VkImageUsageFlags usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
        VkSharingMode sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        VkMemoryPropertyFlags properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

        VkImageViewType imageViewType = VK_IMAGE_VIEW_TYPE_2D;
        VkImageAspectFlags aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;

        if (_isDepth)
        {
            format = Base::GetWindowPtr()->poDepthImageFormat;
            usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
            aspectFlags = VK_IMAGE_ASPECT_DEPTH_BIT;
        }

        if (_isImageArray)
        {
            numArray = 2;
            imageViewType = VK_IMAGE_VIEW_TYPE_2D_ARRAY;
        }

        Base::GetWindowPtr()->createVkImage(width, 
                                            height, 
                                            depth,
                                            numArray,
                                            mipMapCount,
                                            imageType, 
                                            false,
                                            numSamples, 
                                            format, 
                                            tiling, 
                                            usage,
                                            sharingMode,
                                            false,
                                            properties, 
                                            this->image, 
                                            this->memory);
        
        Base::GetWindowPtr()->createVkImageView(this->image, 
                                                imageViewType,
                                                format, 
                                                aspectFlags, 
                                                mipMapCount,
                                                numArray,
                                                this->view);
    }


    MultiRenderPass::MultiRenderPass(const String& _nameRenderPass,
                                     bool _isUseDefault,
                                     bool _isMultiView2)
        //Window
        : Base(_nameRenderPass)
        , isUseDefault(_isUseDefault)
        , isMultiView2(_isMultiView2)

        //RenderPass
        , poRenderPass(VK_NULL_HANDLE)

        //FrameBuffer
        , sampler(VK_NULL_HANDLE)
        , poFrameBuffer(VK_NULL_HANDLE)
    {

    }
    MultiRenderPass::~MultiRenderPass()
    {
        Destroy();
    }   
    void MultiRenderPass::Destroy()
    {
        //RenderPass
        if (this->poRenderPass != VK_NULL_HANDLE &&
            !this->isUseDefault)
        {
            Base::GetWindowPtr()->destroyVkRenderPass(this->poRenderPass);
        }
        this->poRenderPass = VK_NULL_HANDLE;

        //FrameBuffer
        this->framebufferColor.Destroy();
        this->framebufferDepth.Destroy();
        Base::GetWindowPtr()->destroyVkImageSampler(this->sampler);
        this->sampler = VK_NULL_HANDLE;

        Base::GetWindowPtr()->destroyVkFramebuffer(this->poFrameBuffer);
        this->poFrameBuffer = VK_NULL_HANDLE;

        this->imageInfo.imageView = VK_NULL_HANDLE;
        this->imageInfo.sampler = VK_NULL_HANDLE;
    } 
    void MultiRenderPass::Init(uint32_t width, uint32_t height)
    {
        if (this->isUseDefault)
        {
            this->poRenderPass = Base::GetWindowPtr()->poRenderPass;
        }
        else
        {
            //1> Attachment
            {
                this->framebufferColor.Init(width, height, false, this->isMultiView2);
                this->framebufferDepth.Init(width, height, true, this->isMultiView2);
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
                this->imageInfo.imageView = this->framebufferColor.view;
                this->imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            }

            //2> RenderPass
            {
                std::vector<VkAttachmentDescription> aAttachmentDescription;
                std::vector<VkSubpassDescription> aSubpassDescription;
                std::vector<VkSubpassDependency> aSubpassDependency;
                
                //VkAttachmentDescription Color
                VkAttachmentDescription attachmentSR_Color = {};
                Base::GetWindowPtr()->createAttachmentDescription(attachmentSR_Color,
                                                                  0,
                                                                  Base::GetWindowPtr()->poSwapChainImageFormat,
                                                                  VK_SAMPLE_COUNT_1_BIT,
                                                                  VK_ATTACHMENT_LOAD_OP_CLEAR,
                                                                  VK_ATTACHMENT_STORE_OP_STORE,
                                                                  VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                                                  VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                                                  VK_IMAGE_LAYOUT_UNDEFINED,
                                                                  VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
                aAttachmentDescription.push_back(attachmentSR_Color);

                //VkAttachmentDescription Depth
                VkAttachmentDescription attachmentSR_Depth = {};
                Base::GetWindowPtr()->createAttachmentDescription(attachmentSR_Depth,
                                                                  0,
                                                                  Base::GetWindowPtr()->poDepthImageFormat,
                                                                  VK_SAMPLE_COUNT_1_BIT,
                                                                  VK_ATTACHMENT_LOAD_OP_CLEAR,
                                                                  VK_ATTACHMENT_STORE_OP_STORE,
                                                                  VK_ATTACHMENT_LOAD_OP_CLEAR,
                                                                  VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                                                  VK_IMAGE_LAYOUT_UNDEFINED,
                                                                  VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
                aAttachmentDescription.push_back(attachmentSR_Depth);

                //VkSubpassDescription 
                VkAttachmentReference attachRef_Color = {};
                attachRef_Color.attachment = 0;
                attachRef_Color.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

                VkAttachmentReference attachRef_Depth = {};
                attachRef_Depth.attachment = 1;
                attachRef_Depth.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

                VkSubpassDescription subpass_SceneRender = {};
                subpass_SceneRender.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
                subpass_SceneRender.colorAttachmentCount = 1;
                subpass_SceneRender.pColorAttachments = &attachRef_Color;
                subpass_SceneRender.pDepthStencilAttachment = &attachRef_Depth;
                aSubpassDescription.push_back(subpass_SceneRender);
                
                //VkSubpassDependency
                VkSubpassDependency subpassDependency_SceneRender = {};
                subpassDependency_SceneRender.srcSubpass = VK_SUBPASS_EXTERNAL;
                subpassDependency_SceneRender.dstSubpass = 0;
                subpassDependency_SceneRender.srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
                subpassDependency_SceneRender.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
                subpassDependency_SceneRender.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
                subpassDependency_SceneRender.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
                subpassDependency_SceneRender.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
                aSubpassDependency.push_back(subpassDependency_SceneRender);


                const uint32_t viewMask = 0b00000011; 
                const uint32_t correlationMask = 0b00000011;
                VkRenderPassMultiviewCreateInfo renderPassMultiviewCI = {};
                renderPassMultiviewCI.sType = VK_STRUCTURE_TYPE_RENDER_PASS_MULTIVIEW_CREATE_INFO;
                renderPassMultiviewCI.subpassCount = 1;
                renderPassMultiviewCI.pViewMasks = &viewMask;
                renderPassMultiviewCI.correlationMaskCount = 1;
                renderPassMultiviewCI.pCorrelationMasks = &correlationMask;

                VkRenderPassMultiviewCreateInfo* pMultiviewCI = nullptr;
                if (this->isMultiView2)
                {
                    pMultiviewCI = &renderPassMultiviewCI;
                }
                if (!Base::GetWindowPtr()->createVkRenderPass(GetName(),
                                                              aAttachmentDescription,
                                                              aSubpassDescription,
                                                              aSubpassDependency,
                                                              pMultiviewCI,
                                                              this->poRenderPass))
                {
                    String msg = "*********************** MultiRenderPass::Init: Failed to create renderpass: " + GetName();
                    F_LogError(msg.c_str());
                    throw std::runtime_error(msg);
                }
            }

            //3> Framebuffer
            {
                VkImageViewVector aImageViews;
                aImageViews.push_back(this->framebufferColor.view);
                aImageViews.push_back(this->framebufferDepth.view);
                if (!Base::GetWindowPtr()->createVkFramebuffer(GetName(),
                                                               aImageViews, 
                                                               this->poRenderPass,
                                                               0,
                                                               width,
                                                               height,
                                                               1,
                                                               this->poFrameBuffer))
                {
                    String msg = "*********************** MultiRenderPass::Init: Failed to create framebuffer: " + GetName();
                    F_LogError(msg.c_str());
                    throw std::runtime_error(msg);
                }
            }
        }
    }
    void MultiRenderPass::CleanupSwapChain()
    {
         Destroy();
    }
    void MultiRenderPass::RecreateSwapChain()
    {

    }

}; //LostPeter