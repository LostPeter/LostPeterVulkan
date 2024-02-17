/****************************************************************************
* LostPeterPluginRendererVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-12-24
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/VulkanRenderPassManager.h"
#include "../include/VulkanDevice.h"
#include "../include/VulkanRenderPass.h"

template<> LostPeterPluginRendererVulkan::VulkanRenderPassManager* LostPeterFoundation::FSingleton<LostPeterPluginRendererVulkan::VulkanRenderPassManager>::ms_Singleton = nullptr;

namespace LostPeterPluginRendererVulkan
{
    VulkanRenderPassManager* VulkanRenderPassManager::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	VulkanRenderPassManager& VulkanRenderPassManager::GetSingleton()
	{  
		F_Assert(ms_Singleton && "VulkanRenderPassManager::GetSingleton")
		return (*ms_Singleton);     
	}


    VulkanRenderPassManager::VulkanRenderPassManager(VulkanDevice* pDevice)
        : m_pDevice(pDevice)
    {
        F_Assert(m_pDevice && "VulkanRenderPassManager::VulkanRenderPassManager")
    }

    VulkanRenderPassManager::~VulkanRenderPassManager()
    {
        Destroy();
    }

    void VulkanRenderPassManager::Destroy()
    {
        DeleteRenderPassAll();
    }

    bool VulkanRenderPassManager::HasRenderPass(const String& strName)
    {
        return GetRenderPass(strName) != nullptr;
    }
    VulkanRenderPass* VulkanRenderPassManager::GetRenderPass(const String& strName)
    {
        VulkanRenderPassPtrMap::iterator itFind = m_mapRenderPass.find(strName);
        if (itFind == m_mapRenderPass.end())
        {
            return nullptr;
        }
        return itFind->second;
    }
    bool VulkanRenderPassManager::AddRenderPass(VulkanRenderPass* pRenderPass)
    {
        const String& strName = pRenderPass->GetName();
        VulkanRenderPassPtrMap::iterator itFind = m_mapRenderPass.find(strName);
        if (itFind != m_mapRenderPass.end())
        {
            F_LogError("*********************** VulkanRenderPassManager::AddRenderPass: RenderPass name already exist: [%s] !", strName.c_str());
            return false;
        }
        
        m_mapRenderPass.insert(VulkanRenderPassPtrMap::value_type(strName, pRenderPass));
        m_aRenderPass.push_back(pRenderPass);
        return true;
    }
    VulkanRenderPass* VulkanRenderPassManager::CreateRenderPass(const String& nameRenderPass,
                                                                const VkAttachmentDescriptionVector& aAttachmentDescription,
                                                                const VkSubpassDescriptionVector& aSubpassDescription,
                                                                const VkSubpassDependencyVector& aSubpassDependency,
                                                                VkRenderPassMultiviewCreateInfo* pMultiviewCI)
    {
        VulkanRenderPass* pRenderPass = GetRenderPass(nameRenderPass);
        if (pRenderPass != nullptr)
        {
            pRenderPass->AddRef();
            return pRenderPass;
        }

        pRenderPass = new VulkanRenderPass(nameRenderPass, this->m_pDevice);
        if (!pRenderPass->Init(aAttachmentDescription,
                               aSubpassDescription,
                               aSubpassDependency,
                               pMultiviewCI))
        {
            F_DELETE(pRenderPass)
            return nullptr;
        }
        AddRenderPass(pRenderPass);
        pRenderPass->AddRef();
        return pRenderPass;
    }
    VulkanRenderPass* VulkanRenderPassManager::CreateRenderPass_KhrDepth(const String& nameRenderPass,
                                                                         VkFormat formatSwapChain, 
                                                                         VkFormat formatDepth)
    {
        std::vector<VkAttachmentDescription> aAttachmentDescription;
        std::vector<VkSubpassDescription> aSubpassDescription;
        std::vector<VkSubpassDependency> aSubpassDependency;

        //1> Attachment SceneRender Color
        VkAttachmentDescription attachmentSR_Color = {};
        this->m_pDevice->CreateVkAttachmentDescription(0,
                                                       formatSwapChain,
                                                       VK_SAMPLE_COUNT_1_BIT,
                                                       VK_ATTACHMENT_LOAD_OP_CLEAR,
                                                       VK_ATTACHMENT_STORE_OP_STORE,
                                                       VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                                       VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                                       VK_IMAGE_LAYOUT_UNDEFINED,
                                                       VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
                                                       attachmentSR_Color);
        aAttachmentDescription.push_back(attachmentSR_Color);
        
        //2> Attachment SceneRender Depth
        VkAttachmentDescription attachmentSR_Depth = {};
        this->m_pDevice->CreateVkAttachmentDescription(0,
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
            
        //3> Subpass SceneRender
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
        
        //4> Subpass Dependency SceneRender
        VkSubpassDependency subpassDependency_SceneRender = {};
        subpassDependency_SceneRender.srcSubpass = VK_SUBPASS_EXTERNAL;
        subpassDependency_SceneRender.dstSubpass = 0;
        subpassDependency_SceneRender.srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
        subpassDependency_SceneRender.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        subpassDependency_SceneRender.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
        subpassDependency_SceneRender.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        subpassDependency_SceneRender.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
        aSubpassDependency.push_back(subpassDependency_SceneRender);

        //5> CreateRenderPass
        VulkanRenderPass* pRenderPass = CreateRenderPass(nameRenderPass,
                                                         aAttachmentDescription,
                                                         aSubpassDescription,
                                                         aSubpassDependency,
                                                         nullptr);
        if (pRenderPass == nullptr)
        {
            F_LogError("*********************** VulkanRenderPassManager::CreateRenderPass_KhrDepth: Failed to create VkRenderPass: [%s] !", nameRenderPass.c_str());
            return nullptr;
        }

        F_LogInfo("VulkanRenderPassManager::CreateRenderPass_KhrDepth: Success to create VkRenderPass: [%s] !", nameRenderPass.c_str());
        return pRenderPass;
    }
    VulkanRenderPass* VulkanRenderPassManager::CreateRenderPass_KhrDepthImgui(const String& nameRenderPass,
                                                                              VkFormat formatColor, 
                                                                              VkFormat formatDepth, 
                                                                              VkFormat formatSwapChain)
    {
        std::vector<VkAttachmentDescription> aAttachmentDescription;
        std::vector<VkSubpassDescription> aSubpassDescription;
        std::vector<VkSubpassDependency> aSubpassDependency;

        //1> Attachment SceneRender Color
        VkAttachmentDescription attachmentSR_Color = {};
        this->m_pDevice->CreateVkAttachmentDescription(0,
                                                       formatColor,
                                                       VK_SAMPLE_COUNT_1_BIT,
                                                       VK_ATTACHMENT_LOAD_OP_CLEAR,
                                                       VK_ATTACHMENT_STORE_OP_STORE,
                                                       VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                                       VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                                       VK_IMAGE_LAYOUT_UNDEFINED,
                                                       VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                                                       attachmentSR_Color);
        aAttachmentDescription.push_back(attachmentSR_Color);
        
        //2> Attachment SceneRender Depth
        VkAttachmentDescription attachmentSR_Depth = {};
        this->m_pDevice->CreateVkAttachmentDescription(0,
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
            
        //3> Attachment Imgui Color
        VkAttachmentDescription attachmentImgui_Color = {};
        this->m_pDevice->CreateVkAttachmentDescription(0,
                                                       formatSwapChain,
                                                       VK_SAMPLE_COUNT_1_BIT,
                                                       VK_ATTACHMENT_LOAD_OP_LOAD,
                                                       VK_ATTACHMENT_STORE_OP_STORE,
                                                       VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                                       VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                                       VK_IMAGE_LAYOUT_UNDEFINED,
                                                       VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
                                                       attachmentImgui_Color);
        aAttachmentDescription.push_back(attachmentImgui_Color);
        
        //4> Subpass SceneRender
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

        //5> Subpass Imgui
        VkAttachmentReference attachRef_ImguiColor = {};
        attachRef_ImguiColor.attachment = 0;
        attachRef_ImguiColor.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkAttachmentReference attachRef_ImguiDepth = {};
        attachRef_ImguiDepth.attachment = 1;
        attachRef_ImguiDepth.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass_Imgui = {};
        subpass_Imgui.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass_Imgui.colorAttachmentCount = 1;
        subpass_Imgui.pColorAttachments = &attachRef_ImguiColor;
        subpass_Imgui.pDepthStencilAttachment = &attachRef_ImguiDepth;
        aSubpassDescription.push_back(subpass_Imgui);
        
        //6> Subpass Dependency SceneRender
        VkSubpassDependency subpassDependency_SceneRender = {};
        subpassDependency_SceneRender.srcSubpass = VK_SUBPASS_EXTERNAL;
        subpassDependency_SceneRender.dstSubpass = 0;
        subpassDependency_SceneRender.srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
        subpassDependency_SceneRender.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        subpassDependency_SceneRender.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
        subpassDependency_SceneRender.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        subpassDependency_SceneRender.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
        aSubpassDependency.push_back(subpassDependency_SceneRender);

        //7> Subpass Dependency Imgui
        VkSubpassDependency subpassDependency_Imgui = {};
        subpassDependency_Imgui.srcSubpass = 0;
        subpassDependency_Imgui.dstSubpass = 1;
        subpassDependency_Imgui.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        subpassDependency_Imgui.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        subpassDependency_Imgui.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        subpassDependency_Imgui.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        subpassDependency_Imgui.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
        aSubpassDependency.push_back(subpassDependency_Imgui);

        //8> CreateRenderPass
        VulkanRenderPass* pRenderPass = CreateRenderPass(nameRenderPass,
                                                         aAttachmentDescription,
                                                         aSubpassDescription,
                                                         aSubpassDependency,
                                                         nullptr);
        if (pRenderPass == nullptr)
        {
            F_LogError("*********************** VulkanRenderPassManager::CreateRenderPass_KhrDepthImgui: Failed to create VkRenderPass: [%s] !", nameRenderPass.c_str());
            return nullptr;
        }

        F_LogInfo("VulkanRenderPassManager::CreateRenderPass_KhrDepthImgui: Success to create VkRenderPass: [%s] !", nameRenderPass.c_str());
        return pRenderPass;
    }
    VulkanRenderPass* VulkanRenderPassManager::CreateRenderPass_ColorDepthMSAA(const String& nameRenderPass,
                                                                               VkFormat formatColor, 
                                                                               VkFormat formatDepth, 
                                                                               VkFormat formatSwapChain, 
                                                                               VkSampleCountFlagBits samples)
    {
        std::vector<VkAttachmentDescription> aAttachmentDescription;
        std::vector<VkSubpassDescription> aSubpassDescription;
        std::vector<VkSubpassDependency> aSubpassDependency;

        //1> Attachment SceneRender Color
        VkAttachmentDescription attachmentSR_Color = {};
        this->m_pDevice->CreateVkAttachmentDescription(0,
                                                       formatColor,
                                                       samples,
                                                       VK_ATTACHMENT_LOAD_OP_CLEAR,
                                                       VK_ATTACHMENT_STORE_OP_STORE,
                                                       VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                                       VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                                       VK_IMAGE_LAYOUT_UNDEFINED,
                                                       VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                                                       attachmentSR_Color);
        aAttachmentDescription.push_back(attachmentSR_Color);
        
        //2> Attachment SceneRender Depth
        VkAttachmentDescription attachmentSR_Depth = {};
        this->m_pDevice->CreateVkAttachmentDescription(0,
                                                       formatDepth,
                                                       samples,
                                                       VK_ATTACHMENT_LOAD_OP_CLEAR,
                                                       VK_ATTACHMENT_STORE_OP_STORE,
                                                       VK_ATTACHMENT_LOAD_OP_CLEAR,
                                                       VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                                       VK_IMAGE_LAYOUT_UNDEFINED,
                                                       VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
                                                       attachmentSR_Depth);
        aAttachmentDescription.push_back(attachmentSR_Depth);
        
        //3> Attachment SceneRender Color Resolve
        VkAttachmentDescription attachmentSR_ColorResolve = {};
        this->m_pDevice->CreateVkAttachmentDescription(0,
                                                       formatSwapChain,
                                                       VK_SAMPLE_COUNT_1_BIT,
                                                       VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                                       VK_ATTACHMENT_STORE_OP_STORE,
                                                       VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                                       VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                                       VK_IMAGE_LAYOUT_UNDEFINED,
                                                       VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
                                                       attachmentSR_ColorResolve);
        aAttachmentDescription.push_back(attachmentSR_ColorResolve);
        
        //4> Subpass SceneRender
        VkAttachmentReference attachRef_Color = {};
        attachRef_Color.attachment = 0;
        attachRef_Color.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkAttachmentReference attachRef_Depth = {};
        attachRef_Depth.attachment = 1;
        attachRef_Depth.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkAttachmentReference attachRef_ColorResolve = {};
        attachRef_ColorResolve.attachment = 2;
        attachRef_ColorResolve.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass_SceneRender = {};
        subpass_SceneRender.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass_SceneRender.colorAttachmentCount = 1;
        subpass_SceneRender.pColorAttachments = &attachRef_Color;
        subpass_SceneRender.pDepthStencilAttachment = &attachRef_Depth;
        subpass_SceneRender.pResolveAttachments = &attachRef_ColorResolve;
        aSubpassDescription.push_back(subpass_SceneRender);
        
        //5> Subpass Dependency SceneRender
        VkSubpassDependency subpassDependency_SceneRender = {};
        subpassDependency_SceneRender.srcSubpass = VK_SUBPASS_EXTERNAL;
        subpassDependency_SceneRender.dstSubpass = 0;
        subpassDependency_SceneRender.srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
        subpassDependency_SceneRender.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        subpassDependency_SceneRender.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
        subpassDependency_SceneRender.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        subpassDependency_SceneRender.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
        aSubpassDependency.push_back(subpassDependency_SceneRender);

        //6> CreateRenderPass
        VulkanRenderPass* pRenderPass = CreateRenderPass(nameRenderPass,
                                                         aAttachmentDescription,
                                                         aSubpassDescription,
                                                         aSubpassDependency,
                                                         nullptr);
        if (pRenderPass == nullptr)
        {
            F_LogError("*********************** VulkanRenderPassManager::CreateRenderPass_ColorDepthMSAA: Failed to create VkRenderPass: [%s] !", nameRenderPass.c_str());
            return nullptr;
        }

        F_LogInfo("VulkanRenderPassManager::CreateRenderPass_ColorDepthMSAA: Success to create VkRenderPass: [%s] !", nameRenderPass.c_str());
        return pRenderPass;
    }
    VulkanRenderPass* VulkanRenderPassManager::CreateRenderPass_ColorDepthImguiMSAA(const String& nameRenderPass,
                                                                                    VkFormat formatColor,     
                                                                                    VkFormat formatDepth, 
                                                                                    VkFormat formatSwapChain, 
                                                                                    VkSampleCountFlagBits samples)
    {
        std::vector<VkAttachmentDescription> aAttachmentDescription;
        std::vector<VkSubpassDescription> aSubpassDescription;
        std::vector<VkSubpassDependency> aSubpassDependency;

        //1> Attachment SceneRender Color
        VkAttachmentDescription attachmentSR_Color = {};
        this->m_pDevice->CreateVkAttachmentDescription(0,
                                                       formatColor,
                                                       samples,
                                                       VK_ATTACHMENT_LOAD_OP_CLEAR,
                                                       VK_ATTACHMENT_STORE_OP_STORE,
                                                       VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                                       VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                                       VK_IMAGE_LAYOUT_UNDEFINED,
                                                       VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                                                       attachmentSR_Color);
        aAttachmentDescription.push_back(attachmentSR_Color);
        
        //2> Attachment SceneRender Depth
        VkAttachmentDescription attachmentSR_Depth = {};
        this->m_pDevice->CreateVkAttachmentDescription(0,
                                                       formatDepth,
                                                       samples,
                                                       VK_ATTACHMENT_LOAD_OP_CLEAR,
                                                       VK_ATTACHMENT_STORE_OP_STORE,
                                                       VK_ATTACHMENT_LOAD_OP_CLEAR,
                                                       VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                                       VK_IMAGE_LAYOUT_UNDEFINED,
                                                       VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
                                                       attachmentSR_Depth);
        aAttachmentDescription.push_back(attachmentSR_Depth);
        
        //3> Attachment SceneRender Color Resolve
        VkAttachmentDescription attachmentSR_ColorResolve = {};
        this->m_pDevice->CreateVkAttachmentDescription(0,
                                                       formatSwapChain,
                                                       VK_SAMPLE_COUNT_1_BIT,
                                                       VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                                       VK_ATTACHMENT_STORE_OP_STORE,
                                                       VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                                       VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                                       VK_IMAGE_LAYOUT_UNDEFINED,
                                                       VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                                                       attachmentSR_ColorResolve);
        aAttachmentDescription.push_back(attachmentSR_ColorResolve);
        
        //4> Attachment Imgui Color
        VkAttachmentDescription attachmentImgui_Color = {};
        this->m_pDevice->CreateVkAttachmentDescription(0,
                                                       formatSwapChain,
                                                       VK_SAMPLE_COUNT_1_BIT,
                                                       VK_ATTACHMENT_LOAD_OP_LOAD,
                                                       VK_ATTACHMENT_STORE_OP_STORE,
                                                       VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                                       VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                                       VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                                                       VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
                                                       attachmentImgui_Color);
        aAttachmentDescription.push_back(attachmentImgui_Color);
        
        //5> Subpass SceneRender
        VkAttachmentReference attachRef_Color = {};
        attachRef_Color.attachment = 0;
        attachRef_Color.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkAttachmentReference attachRef_Depth = {};
        attachRef_Depth.attachment = 1;
        attachRef_Depth.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkAttachmentReference attachRef_ColorResolve = {};
        attachRef_ColorResolve.attachment = 2;
        attachRef_ColorResolve.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass_SceneRender = {};
        subpass_SceneRender.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass_SceneRender.colorAttachmentCount = 1;
        subpass_SceneRender.pColorAttachments = &attachRef_Color;
        subpass_SceneRender.pDepthStencilAttachment = &attachRef_Depth;
        subpass_SceneRender.pResolveAttachments = &attachRef_ColorResolve;
        aSubpassDescription.push_back(subpass_SceneRender);

        //6> Subpass Imgui
        VkAttachmentReference attachRef_ImguiColor = {};
        attachRef_ImguiColor.attachment = 0;
        attachRef_ImguiColor.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkAttachmentReference attachRef_ImguiDepth = {};
        attachRef_ImguiDepth.attachment = 1;
        attachRef_ImguiDepth.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkAttachmentReference attachRef_ImguiColorResolve = {};
        attachRef_ImguiColorResolve.attachment = 2;
        attachRef_ImguiColorResolve.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass_Imgui = {};
        subpass_Imgui.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass_Imgui.colorAttachmentCount = 1;
        subpass_Imgui.pColorAttachments = &attachRef_ImguiColor;
        subpass_Imgui.pDepthStencilAttachment = &attachRef_ImguiDepth;
        subpass_Imgui.pResolveAttachments = &attachRef_ImguiColorResolve;
        aSubpassDescription.push_back(subpass_Imgui);
        
        //7> Subpass Dependency SceneRender
        VkSubpassDependency subpassDependency_SceneRender = {};
        subpassDependency_SceneRender.srcSubpass = VK_SUBPASS_EXTERNAL;
        subpassDependency_SceneRender.dstSubpass = 0;
        subpassDependency_SceneRender.srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
        subpassDependency_SceneRender.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        subpassDependency_SceneRender.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
        subpassDependency_SceneRender.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        subpassDependency_SceneRender.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
        aSubpassDependency.push_back(subpassDependency_SceneRender);

        //8> Subpass Dependency Imgui
        VkSubpassDependency subpassDependency_Imgui = {};
        subpassDependency_Imgui.srcSubpass = 0;
        subpassDependency_Imgui.dstSubpass = 1;
        subpassDependency_Imgui.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        subpassDependency_Imgui.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        subpassDependency_Imgui.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        subpassDependency_Imgui.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        aSubpassDependency.push_back(subpassDependency_Imgui);

        //9> CreateRenderPass
        VulkanRenderPass* pRenderPass = CreateRenderPass(nameRenderPass,
                                                         aAttachmentDescription,
                                                         aSubpassDescription,
                                                         aSubpassDependency,
                                                         nullptr);
        if (pRenderPass == nullptr)
        {
            F_LogError("*********************** VulkanRenderPassManager::CreateRenderPass_ColorDepthImguiMSAA: Failed to create VkRenderPass: [%s] !", nameRenderPass.c_str());
            return nullptr;
        }

        F_LogInfo("VulkanRenderPassManager::CreateRenderPass_ColorDepthImguiMSAA: Success to create VkRenderPass: [%s] !", nameRenderPass.c_str());
        return pRenderPass;
    }
    void VulkanRenderPassManager::DeleteRenderPass(const String& strName)
    {
        VulkanRenderPassPtrMap::iterator itFind = m_mapRenderPass.find(strName);
        if (itFind == m_mapRenderPass.end())
        {
            return;
        }

        itFind->second->DelRef();
        if (itFind->second->CanDel())
        {
            VulkanRenderPassPtrVector::iterator itFindA = std::find(m_aRenderPass.begin(), m_aRenderPass.end(), itFind->second);
            if (itFindA != m_aRenderPass.end())
                m_aRenderPass.erase(itFindA);
            F_DELETE(itFind->second)
            m_mapRenderPass.erase(itFind);
        }
    }
    void VulkanRenderPassManager::DeleteRenderPass(VulkanRenderPass* pRenderPass)
    {
        if (!pRenderPass)
            return;
        DeleteRenderPass(pRenderPass->GetName());
    }
    void VulkanRenderPassManager::DeleteRenderPassAll()
    {
        for (VulkanRenderPassPtrVector::iterator it = m_aRenderPass.begin();
             it != m_aRenderPass.end(); ++it)
        {
            F_DELETE(*it)
        }
        m_aRenderPass.clear();
        m_mapRenderPass.clear();
    }

}; //LostPeterPluginRendererVulkan