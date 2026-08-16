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

#include "../include/VKRenderPassShadowMap.h"
#include "../include/VulkanWindow.h"
#include "../include/VKTexture.h"

namespace LostPeterVulkan
{
    VKRenderPassShadowMap::VKRenderPassShadowMap(const String& nameRenderPass)
        //Window
        : Base(nameRenderPass)

        //Depth
		, pTexture_Depth(nullptr)

        //RenderPass
        , poRenderPass(VK_NULL_HANDLE)

        //FrameBuffer
        , poFrameBuffer(VK_NULL_HANDLE)
    {

    }
    VKRenderPassShadowMap::~VKRenderPassShadowMap()
    {
        Destroy();
    }   
    void VKRenderPassShadowMap::Destroy()
    {
		VulkanWindow* pWindow = Base::GetWindowPtr();

        //RenderPass
        if (this->poRenderPass != VK_NULL_HANDLE)
        {
            pWindow->destroyVkRenderPass(this->poRenderPass);
        }
        this->poRenderPass = VK_NULL_HANDLE;

        //FrameBuffer
        if (this->poFrameBuffer != VK_NULL_HANDLE)
        {
            pWindow->destroyVkFramebuffer(this->poFrameBuffer);
        }
        this->poFrameBuffer = VK_NULL_HANDLE;

        //Depth
		F_DELETE(this->pTexture_Depth)
    } 
    void VKRenderPassShadowMap::Init(uint32_t width, 
                                     uint32_t height,
                                     FTexturePixelFormatType format)
    {
		VulkanWindow* pWindow = Base::GetWindowPtr();

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

        //1> Depth
        {
            String nameTexture = "Texture-ShadowMapDepth-" + GetName();
			StringVector aPathTextureShadowMapDepth;
			this->pTexture_Depth = new VKTexture(0,
												 nameTexture,
												 aPathTextureShadowMapDepth,
												 F_Texture_2D,
												 format,
												 F_TextureFilter_Bilinear,
												 F_TextureAddressing_Clamp,
												 F_TextureBorderColor_OpaqueBlack,
												 true,
												 true);
			this->pTexture_Depth->rtImageUsage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
			this->pTexture_Depth->aspectFlags = VK_IMAGE_ASPECT_DEPTH_BIT;
			this->pTexture_Depth->poTextureImageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
			this->pTexture_Depth->LoadTexture(width, 
											  height,
											  1,
											  1,
											  nullptr);
        }

        //2> RenderPass
        {
            std::vector<VkAttachmentDescription> aAttachmentDescription;
            std::vector<VkSubpassDescription> aSubpassDescription;
            std::vector<VkSubpassDependency> aSubpassDependency;
            
            //VkAttachmentDescription Depth
            VkAttachmentDescription attachmentSR_Depth = {};
            pWindow->createAttachmentDescription(attachmentSR_Depth,
												 0,
												 this->pTexture_Depth->typeFormat,
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

			String nameRenderPass = "RenderPass-ShadowMapDepth-" + GetName();
            if (!pWindow->createVkRenderPass(nameRenderPass,
											 aAttachmentDescription,
											 aSubpassDescription,
											 aSubpassDependency,
											 nullptr,
											 this->poRenderPass))
            {
                String msg = "*********************** VKRenderPassShadowMap::Init: Failed to create renderpass: " + nameRenderPass;
                F_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }
        }

        //3> Framebuffer
        {
            VkImageViewVector aImageViews;
            aImageViews.push_back(this->pTexture_Depth->GetVkImageView());
			String nameFrameBuffer = "FrameBuffer-ShadowMapDepth-" + GetName();
            if (!pWindow->createVkFramebuffer(nameFrameBuffer,
											  aImageViews, 
											  this->poRenderPass,
											  0,
											  width,
											  height,
											  1,
											  this->poFrameBuffer))
            {
                String msg = "*********************** VKRenderPassShadowMap::Init: Failed to create framebuffer: " + nameFrameBuffer;
                F_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }
        }
    }
    void VKRenderPassShadowMap::CleanupSwapChain()
    {
        Destroy();
    }
    void VKRenderPassShadowMap::RecreateSwapChain()
    {

    }

}; //LostPeterVulkan