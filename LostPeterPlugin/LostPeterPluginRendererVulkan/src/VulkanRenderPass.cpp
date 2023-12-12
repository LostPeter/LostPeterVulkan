/****************************************************************************
* LostPeterPluginRendererVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-12-12
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/VulkanRenderPass.h"
#include "../include/VulkanDevice.h"
#include "../include/VulkanFrameBufferAttachment.h"

namespace LostPeterPluginRendererVulkan
{
    VulkanRenderPass::VulkanRenderPass(const String& nameRenderPass,
                                       VulkanDevice* pDevice,
                                       bool bIsUseDefault,
                                       bool bIsMultiView2)
        : Base(nameRenderPass)
        , m_pDevice(pDevice)
        , m_bIsUseDefault(bIsUseDefault)
        , m_bIsMultiView2(bIsMultiView2)

        , m_pFramebufferAttachmentColor(nullptr)
        , m_pFramebufferAttachmentDepth(nullptr)

        , m_vkSampler(VK_NULL_HANDLE)
        , m_vkRenderPass(VK_NULL_HANDLE)
        , m_vkFrameBuffer(VK_NULL_HANDLE)
    {
        F_Assert(m_pDevice != nullptr && "VulkanRenderPass::VulkanRenderPass");
    }

    VulkanRenderPass::~VulkanRenderPass()
    {
        Destroy();
    }   

    void VulkanRenderPass::Destroy()
    {
        if (this->m_vkRenderPass != VK_NULL_HANDLE && !this->m_bIsUseDefault)
        {
            m_pDevice->DestroyVkRenderPass(this->m_vkRenderPass);
        }
        this->m_vkRenderPass = VK_NULL_HANDLE;

        F_DELETE(m_pFramebufferAttachmentColor)
        F_DELETE(m_pFramebufferAttachmentDepth)
        m_pDevice->DestroyVkSampler(this->m_vkSampler);
        this->m_vkSampler = VK_NULL_HANDLE;

        m_pDevice->DestroyVkFramebuffer(this->m_vkFrameBuffer);
        this->m_vkFrameBuffer = VK_NULL_HANDLE;

        this->m_vkDescriptorImageInfo.imageView = VK_NULL_HANDLE;
        this->m_vkDescriptorImageInfo.sampler = VK_NULL_HANDLE;
    } 
    bool VulkanRenderPass::Init(uint32_t width, 
                                uint32_t height, 
                                VkSampleCountFlagBits numSamples,
                                VkFormat formatSwapChain,
                                VkFormat formatDepth,
                                VkRenderPass vkRenderPass)
    {
        if (this->m_bIsUseDefault)
        {
            this->m_vkRenderPass = vkRenderPass;
        }
        else
        {
            //1> Attachment
            {
                //Color
                m_pFramebufferAttachmentColor = new VulkanFrameBufferAttachment(m_pDevice);
                if (!m_pFramebufferAttachmentColor->Init(width, 
                                                         height, 
                                                         false, 
                                                         this->m_bIsMultiView2,
                                                         numSamples,
                                                         formatSwapChain,
                                                         formatDepth))
                {
                    F_LogError("*********************** VulkanRenderPass::Init: Failed to init FramebufferAttachmentColor !");
                    return false;
                }

                //Depth
                m_pFramebufferAttachmentDepth = new VulkanFrameBufferAttachment(m_pDevice);
                if (!m_pFramebufferAttachmentDepth->Init(width, 
                                                         height, 
                                                         true, 
                                                         this->m_bIsMultiView2,
                                                         numSamples,
                                                         formatSwapChain,
                                                         formatDepth))
                {
                    F_LogError("*********************** VulkanRenderPass::Init: Failed to init FramebufferAttachmentDepth !");
                    return false;
                }

                //Sampler
                m_pDevice->CreateVkSampler(F_TextureFilter_Bilinear, 
                                           F_TextureAddressing_Clamp,
                                           F_TextureBorderColor_OpaqueWhite,
                                           false,
                                           1.0f,
                                           0.0f,
                                           1.0f,
                                           0.0f,
                                           this->m_vkSampler);
                
                this->m_vkDescriptorImageInfo.sampler = this->m_vkSampler;
                this->m_vkDescriptorImageInfo.imageView = this->m_pFramebufferAttachmentColor->GetVkImageView();
                this->m_vkDescriptorImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            }

            //2> RenderPass
            {
                std::vector<VkAttachmentDescription> aAttachmentDescription;
                std::vector<VkSubpassDescription> aSubpassDescription;
                std::vector<VkSubpassDependency> aSubpassDependency;
                
                //VkAttachmentDescription Color
                VkAttachmentDescription attachmentSR_Color = {};
                m_pDevice->CreateVkAttachmentDescription(0,
                                                         formatSwapChain,
                                                         VK_SAMPLE_COUNT_1_BIT,
                                                         VK_ATTACHMENT_LOAD_OP_CLEAR,
                                                         VK_ATTACHMENT_STORE_OP_STORE,
                                                         VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                                         VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                                         VK_IMAGE_LAYOUT_UNDEFINED,
                                                         VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                                                         attachmentSR_Color);
                aAttachmentDescription.push_back(attachmentSR_Color);

                //VkAttachmentDescription Depth
                VkAttachmentDescription attachmentSR_Depth = {};
                m_pDevice->CreateVkAttachmentDescription(0,
                                                         formatDepth,
                                                         VK_SAMPLE_COUNT_1_BIT,
                                                         VK_ATTACHMENT_LOAD_OP_CLEAR,
                                                         VK_ATTACHMENT_STORE_OP_STORE,
                                                         VK_ATTACHMENT_LOAD_OP_CLEAR,
                                                         VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                                         VK_IMAGE_LAYOUT_UNDEFINED,
                                                         VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
                                                         attachmentSR_Depth);
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
                if (this->m_bIsMultiView2)
                {
                    pMultiviewCI = &renderPassMultiviewCI;
                }
                if (!m_pDevice->CreateVkRenderPass(GetName(),
                                                   aAttachmentDescription,
                                                   aSubpassDescription,
                                                   aSubpassDependency,
                                                   pMultiviewCI,
                                                   this->m_vkRenderPass))
                {
                    F_LogError("*********************** VulkanRenderPass::Init: Failed to create VkRenderPass: [%s] !", GetName().c_str());
                    return false;
                }
            }

            //3> Framebuffer
            {
                VkImageViewVector aImageViews;
                aImageViews.push_back(this->m_pFramebufferAttachmentColor->GetVkImageView());
                aImageViews.push_back(this->m_pFramebufferAttachmentDepth->GetVkImageView());
                if (!m_pDevice->CreateVkFramebuffer(GetName(),
                                                    aImageViews, 
                                                    this->m_vkRenderPass,
                                                    0,
                                                    width,
                                                    height,
                                                    1,
                                                    this->m_vkFrameBuffer))
                {
                    F_LogError("*********************** VulkanRenderPass::Init: Failed to create VkFramebuffer: [%s] !", GetName().c_str());
                    return false;
                }
            }
        }

        return true;
    }

    void VulkanRenderPass::CleanupSwapChain()
    {
        Destroy();
    }

    void VulkanRenderPass::RecreateSwapChain()
    {

    }

}; //LostPeterPluginRendererVulkan