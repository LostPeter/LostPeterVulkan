/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-09-13
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/VKRenderPassCull.h"
#include "../include/VulkanWindow.h"

namespace LostPeterVulkan
{
    int VKRenderPassCull::s_nHizDepthWidth = 1024;

    VKRenderPassCull::VKRenderPassCull(const String& nameRenderPass)
        //Window
        : Base(nameRenderPass)

        //HizDepth
        , poFormat(VK_FORMAT_R32_SFLOAT)
        , poHizDepthImage(VK_NULL_HANDLE)
        , poHizDepthImageMemory(VK_NULL_HANDLE)
        , poHizDepthImageView_Main(VK_NULL_HANDLE)
        , poHizDepthImageView_0(VK_NULL_HANDLE)
        , poHizDepthSampler(VK_NULL_HANDLE)

        , nHizDepthWidth(s_nHizDepthWidth)
        , nHizDepthHeight(s_nHizDepthWidth / 2)
        , nHizDepthMinmapCount(9)

        , poBuffer_HizDepthCB(VK_NULL_HANDLE)
        , poBufferMemory_HizDepthCB(VK_NULL_HANDLE)

        , poRenderPass(VK_NULL_HANDLE)
        , poFrameBuffer(VK_NULL_HANDLE)

    {

    }
    VKRenderPassCull::~VKRenderPassCull()
    {
        destroyBufferHizDepth();
        Destroy();
    }   

    void VKRenderPassCull::Destroy()
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

        //HizDepth
        if (this->poHizDepthImageView_0 != VK_NULL_HANDLE)
        {
            Base::GetWindowPtr()->destroyVkImageView(this->poHizDepthImageView_0);
        }
        for (size_t i = 0; i < this->aHizDepthImageView_Mipmap.size(); i++)
        {
            Base::GetWindowPtr()->destroyVkImageView(this->aHizDepthImageView_Mipmap[i]);
        }
        if (this->poHizDepthImage != VK_NULL_HANDLE)
        {
            Base::GetWindowPtr()->destroyVkImage(this->poHizDepthImage, this->poHizDepthImageMemory, this->poHizDepthImageView_Main);
        }
        this->poHizDepthImage = VK_NULL_HANDLE;
        this->poHizDepthImageMemory = VK_NULL_HANDLE;
        this->poHizDepthImageView_Main = VK_NULL_HANDLE;
        this->poHizDepthImageView_0 = VK_NULL_HANDLE;
        this->aHizDepthImageView_Mipmap.clear();
        if (this->poHizDepthSampler != VK_NULL_HANDLE)
        {
            Base::GetWindowPtr()->destroyVkImageSampler(this->poHizDepthSampler);
        }
        this->poHizDepthSampler = VK_NULL_HANDLE;
    } 
        void VKRenderPassCull::destroyBufferHizDepth()
        {
            if (this->poBuffer_HizDepthCB != VK_NULL_HANDLE)
            {
                Base::GetWindowPtr()->destroyVkBuffer(this->poBuffer_HizDepthCB, this->poBufferMemory_HizDepthCB);
            }
            this->poBuffer_HizDepthCB = VK_NULL_HANDLE;
            this->poBufferMemory_HizDepthCB = VK_NULL_HANDLE;
        }

    bool VKRenderPassCull::Init()
    {
        if (this->poBuffer_HizDepthCB == VK_NULL_HANDLE)
        {
            if (!createBufferHizDepth())
            {
                F_LogError("*********************** VKRenderPassCull::Init: createBufferHizDepth failed !");
                return false;
            }
        }
        if (!createCullTexture())
        {
            F_LogError("VKRenderPassCull::Init: Failed to setup cull texture !");
            return false;
        }
        if (!createRenderPassFrameBuffer())
        {
            F_LogError("VKRenderPassCull::Init: Failed to setup cull renderpass, framebuffer !");
            return false;
        }

        return true;
    }
        bool VKRenderPassCull::createBufferHizDepth()
        {
            destroyBufferHizDepth();

            VkDeviceSize bufferSize = sizeof(HizDepthConstants);
            Base::GetWindowPtr()->createVkBuffer("HizDepthConstants-" + this->name, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, this->poBuffer_HizDepthCB, this->poBufferMemory_HizDepthCB);
            return true;
        } 
        bool VKRenderPassCull::createCullTexture()
        {
            F_LogInfo("VKRenderPassCull::createCullTexture: Start setup cull texture !");
            {
                updateHizDepthRTSize();

                String nameTexture = "Texture-HizDepth";
                Base::GetWindowPtr()->createTextureRenderTarget2D(nameTexture,
                                                                  nullptr,
                                                                  (uint32_t)this->nHizDepthWidth,
                                                                  (uint32_t)this->nHizDepthHeight,
                                                                  true,
                                                                  (uint32_t)this->nHizDepthMinmapCount,
                                                                  VK_SAMPLE_COUNT_1_BIT,
                                                                  this->poFormat,
                                                                  VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT,
                                                                  VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                                                                  true,
                                                                  this->poHizDepthImage,
                                                                  this->poHizDepthImageMemory);
                F_LogInfo("VKRenderPassCull::createCullTexture: createTextureRenderTarget2D !");
                VkComponentMapping componentMapping = { VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY };
                Base::GetWindowPtr()->createVkImageView(nameTexture + "-ViewMain",
                                                        0,
                                                        this->poHizDepthImage, 
                                                        VK_IMAGE_VIEW_TYPE_2D, 
                                                        this->poFormat, 
                                                        componentMapping,
                                                        VK_IMAGE_ASPECT_COLOR_BIT,
                                                        0,
                                                        (uint32_t)this->nHizDepthMinmapCount,
                                                        0,
                                                        1, 
                                                        this->poHizDepthImageView_Main);
                F_LogInfo("VKRenderPassCull::createCullTexture: createVkImageView Main !");
                Base::GetWindowPtr()->createVkImageView(nameTexture + "-View0",
                                                        0,
                                                        this->poHizDepthImage, 
                                                        VK_IMAGE_VIEW_TYPE_2D, 
                                                        this->poFormat, 
                                                        componentMapping,
                                                        VK_IMAGE_ASPECT_COLOR_BIT,
                                                        0,
                                                        1,
                                                        0,
                                                        1, 
                                                        this->poHizDepthImageView_0);
                F_LogInfo("VKRenderPassCull::createCullTexture: createVkImageView 0 !");
                for (int i = 0; i < this->nHizDepthMinmapCount; i++)
                {
                    VkImageView viewMip = VK_NULL_HANDLE;
                    String nameView = nameTexture + "-Storage-View" + FUtilString::SaveInt(i);
                    Base::GetWindowPtr()->createVkImageView(nameView,
                                                            0,
                                                            this->poHizDepthImage, 
                                                            VK_IMAGE_VIEW_TYPE_2D, 
                                                            this->poFormat, 
                                                            componentMapping,
                                                            VK_IMAGE_ASPECT_COLOR_BIT,
                                                            i,
                                                            1,
                                                            0,
                                                            1, 
                                                            viewMip);
                    if (viewMip == VK_NULL_HANDLE)
                    {
                        F_LogError("*********************** VKRenderPassCull::createCullTexture: createVkImageView Storage: [%s] failed !", nameView.c_str());
                        continue;
                    }

                    this->aHizDepthImageView_Mipmap.push_back(viewMip);
                    F_LogInfo("VKRenderPassCull::createCullTexture: createVkImageView Storage: [%s] success !", nameView.c_str());
                }
                Base::GetWindowPtr()->createVkSampler(nameTexture,
                                                      F_TextureFilter_None, 
                                                      F_TextureAddressing_Clamp,
                                                      F_TextureBorderColor_OpaqueBlack,
                                                      true,
                                                      Base::GetWindowPtr()->poPhysicalDeviceProperties.limits.maxSamplerAnisotropy,
                                                      0.0f,
                                                      1.0f,
                                                      0.0f,
                                                      this->poHizDepthSampler);
                F_LogInfo("VKRenderPassCull::createCullTexture: createVkSampler !");

                this->poHizDepthImageInfo_Sampler = {};
                this->poHizDepthImageInfo_Sampler.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                this->poHizDepthImageInfo_Sampler.imageView = this->poHizDepthImageView_Main;
                this->poHizDepthImageInfo_Sampler.sampler = this->poHizDepthSampler;

                F_LogInfo("VKRenderPassCull::createCullTexture: Compute: Create render texture: [%s] - (w,h): [%d, %d], mip: [%d] success !", 
                          nameTexture.c_str(), this->nHizDepthWidth, this->nHizDepthHeight, this->nHizDepthMinmapCount);
            }
            F_LogInfo("VKRenderPassCull::createCullTexture: End setup cull texture !");

            return true;
        }
        bool VKRenderPassCull::createRenderPassFrameBuffer()
        {
            F_LogInfo("VKRenderPassCull::createRenderPassFrameBuffer: Start setup cull renderpass, framebuffer !");
            {
                uint32_t width = (uint32_t)this->nHizDepthWidth;
                uint32_t height = (uint32_t)this->nHizDepthHeight;

                //1> Common
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

                //2> RenderPass
                {
                    std::vector<VkAttachmentDescription> aAttachmentDescription;
                    std::vector<VkSubpassDescription> aSubpassDescription;
                    std::vector<VkSubpassDependency> aSubpassDependency;
                    
                    //VkAttachmentDescription
                    VkAttachmentDescription attachmentSR = {};
                    Base::GetWindowPtr()->createAttachmentDescription(attachmentSR,
                                                                      0,
                                                                      this->poFormat,
                                                                      VK_SAMPLE_COUNT_1_BIT,
                                                                      VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                                                      VK_ATTACHMENT_STORE_OP_DONT_CARE, 
                                                                      VK_ATTACHMENT_LOAD_OP_DONT_CARE, 
                                                                      VK_ATTACHMENT_STORE_OP_DONT_CARE, 
                                                                      VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                                                                      VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL); 
                    aAttachmentDescription.push_back(attachmentSR);

                    //VkSubpassDescription
                    VkAttachmentReference attachRef = {};
                    attachRef.attachment = 0;
                    attachRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

                    VkSubpassDescription subpass_SceneRender = {};
                    subpass_SceneRender.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
                    subpass_SceneRender.colorAttachmentCount = 1;
                    subpass_SceneRender.pColorAttachments = &attachRef;
                    subpass_SceneRender.pDepthStencilAttachment = nullptr;
                    aSubpassDescription.push_back(subpass_SceneRender);
                    
                    //VkSubpassDependency


                    String nameRenderPass = "RenderPass-" + GetName();
                    if (!Base::GetWindowPtr()->createVkRenderPass(nameRenderPass,
                                                                  aAttachmentDescription,
                                                                  aSubpassDescription,
                                                                  aSubpassDependency,
                                                                  nullptr,
                                                                  this->poRenderPass))
                    {
                        String msg = "*********************** VKRenderPassCull::Init: createRenderPassFrameBuffer to create renderpass: " + nameRenderPass;
                        F_LogError(msg.c_str());
                        throw std::runtime_error(msg);
                    }
                }

                //3> Framebuffer
                {
                    VkImageViewVector aImageViews;
                    aImageViews.push_back(this->poHizDepthImageView_0);
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
                        String msg = "*********************** VKRenderPassCull::createRenderPassFrameBuffer: Failed to create framebuffer: " + nameFramebuffer;
                        F_LogError(msg.c_str());
                        throw std::runtime_error(msg);
                    }
                }
            }
            F_LogInfo("VKRenderPassCull::createRenderPassFrameBuffer: End setup cull renderpass, framebuffer !");

            return true;
        }

    void VKRenderPassCull::CleanupSwapChain()
    {   
        Destroy();
    }

    void VKRenderPassCull::RecreateSwapChain()
    {

    }

    void VKRenderPassCull::GetDescriptorImageInfo(int mipmap, VkDescriptorImageInfo& imageInfo)
    {
        F_Assert(mipmap >= 0 && mipmap < this->nHizDepthMinmapCount && "VKRenderPassCull::GetDescriptorImageInfo")

        imageInfo.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
        imageInfo.imageView = this->aHizDepthImageView_Mipmap[mipmap];
        imageInfo.sampler = nullptr;
    }

    void VKRenderPassCull::UpdateHizDepthBuffer_ImageLayoutFromColorAttachmentToShaderReadOnly(VkCommandBuffer& commandBuffer)
    {
        uint32_t count = (uint32_t)this->aHizDepthImageView_Mipmap.size();
        for (uint32_t i = 0; i < count; i++)
        {
            Base::GetWindowPtr()->transitionImageLayout(commandBuffer,
                                                        this->poHizDepthImage,
                                                        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                                                        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                                                        i,
                                                        1,
                                                        0,
                                                        1,
                                                        VK_IMAGE_ASPECT_COLOR_BIT);
        }
    }
    void VKRenderPassCull::UpdateHizDepthBuffer_ImageLayoutFromShaderReadOnlyToColorAttachment(VkCommandBuffer& commandBuffer)
    {
        uint32_t count = (uint32_t)this->aHizDepthImageView_Mipmap.size();
        for (uint32_t i = 0; i < count; i++)
        {
            Base::GetWindowPtr()->transitionImageLayout(commandBuffer,
                                                        this->poHizDepthImage,
                                                        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                                                        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                                                        i,
                                                        1,
                                                        0,
                                                        1,
                                                        VK_IMAGE_ASPECT_COLOR_BIT);
        }
    }
    void VKRenderPassCull::UpdateHizDepthBuffer_ImageLayoutFromColorAttachmentToGeneral(VkCommandBuffer& commandBuffer)
    {
        uint32_t count = (uint32_t)this->aHizDepthImageView_Mipmap.size();
        for (uint32_t i = 0; i < count; i++)
        {
            Base::GetWindowPtr()->transitionImageLayout(commandBuffer,
                                                        this->poHizDepthImage,
                                                        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                                                        VK_IMAGE_LAYOUT_GENERAL,
                                                        i,
                                                        1,
                                                        0,
                                                        1,
                                                        VK_IMAGE_ASPECT_COLOR_BIT);
        }
    }
    void VKRenderPassCull::UpdateHizDepthBuffer_ImageLayoutFromGeneralToColorAttachment(VkCommandBuffer& commandBuffer)
    {
        uint32_t count = (uint32_t)this->aHizDepthImageView_Mipmap.size();
        for (uint32_t i = 0; i < count; i++)
        {
            Base::GetWindowPtr()->transitionImageLayout(commandBuffer,
                                                        this->poHizDepthImage,
                                                        VK_IMAGE_LAYOUT_GENERAL,
                                                        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                                                        i,
                                                        1,
                                                        0,
                                                        1,
                                                        VK_IMAGE_ASPECT_COLOR_BIT);
        }
    }

    void VKRenderPassCull::UpdateHizDepthBuffer_Render()
    {
        this->hizDepthCB.vRtDepthSize.x = (float)this->nHizDepthWidth;
        this->hizDepthCB.vRtDepthSize.y = (float)this->nHizDepthHeight;
        this->hizDepthCB.vRtDepthSize.z = 1.0f / this->nHizDepthWidth;
        this->hizDepthCB.vRtDepthSize.w = 1.0f / this->nHizDepthHeight;
        updateHizDepthBuffer();
    }
    void VKRenderPassCull::UpdateHizDepthBuffer_Compute(float w, float h)
    {
        this->hizDepthCB.vRtDepthSize.x = w;
        this->hizDepthCB.vRtDepthSize.y = h;
        this->hizDepthCB.vRtDepthSize.z = 0.0f;
        this->hizDepthCB.vRtDepthSize.w = 0.0f;
        updateHizDepthBuffer();
    }

    void VKRenderPassCull::updateHizDepthRTSize()
    {
        int nScreenWith = (int)Base::GetWindowPtr()->poSwapChainExtent.width;

        this->nHizDepthWidth = s_nHizDepthWidth;
        this->nHizDepthMinmapCount = 9;
        getHizDepthRTSizeFromScreen(nScreenWith, this->nHizDepthWidth, this->nHizDepthMinmapCount);
        this->nHizDepthHeight = this->nHizDepthWidth / 2;
    }
    void VKRenderPassCull::updateHizDepthBuffer()
    {
        Base::GetWindowPtr()->updateVKBuffer(0, sizeof(HizDepthConstants), &this->hizDepthCB, this->poBufferMemory_HizDepthCB);
    }
    void VKRenderPassCull::getHizDepthRTSizeFromScreen(int screenWidth, int& w, int& mip)
    {
        if (screenWidth >= 2048)
        {
            w = 2048;
            mip = 10;
            return;
        }
        else if (screenWidth >= 1024)
        {
            w = 1024;
            mip = 9;
            return;
        }

        w = 512;
        mip = 8;
    }

}; //LostPeterVulkan