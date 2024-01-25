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
#include "../include/VulkanRenderWindow.h"
#include "../include/VulkanSwapChain.h"
#include "../include/VulkanRenderPass.h"
#include "../include/VulkanRenderPassManager.h"
#include "../include/VulkanFrameBuffer.h"
#include "../include/VulkanFrameBufferManager.h"
#include "../include/VulkanTexture.h"
#include "../include/VulkanConverter.h"

namespace LostPeterPluginRendererVulkan
{
    VulkanRenderFrameBufferDescriptor::VulkanRenderFrameBufferDescriptor(const String& nameRenderFrameBufferDescriptor,
                                                                         VulkanDevice* pDevice,
                                                                         VulkanRenderWindow* pRenderWindow)
        : RenderFrameBufferDescriptor(nameRenderFrameBufferDescriptor)
        , m_pDevice(pDevice)
        , m_pRenderWindow(pRenderWindow)
        , m_pRenderPass(nullptr)
        , m_eVkFormatColor(VK_FORMAT_B8G8R8A8_SRGB)
        , m_eVkFormatDepth(VK_FORMAT_D32_SFLOAT)
        , m_eVkFormatSwapChian(VK_FORMAT_B8G8R8A8_SRGB)
        , m_eVkMSAASampleCount(VK_SAMPLE_COUNT_1_BIT)
    {
        F_Assert(m_pDevice != nullptr && m_pRenderWindow != nullptr && "VulkanRenderFrameBufferDescriptor::VulkanRenderFrameBufferDescriptor");
    }

    VulkanRenderFrameBufferDescriptor::~VulkanRenderFrameBufferDescriptor()
    {
        Destroy();
    }
    
    void VulkanRenderFrameBufferDescriptor::Destroy()
    {
        destroyRenderFrameBuffer();
        destroyRenderPass();
        m_aTextureSwapChain.clear();
        destroyTextures(m_aTextureDepth);
        destroyTextures(m_aTextureColor);
    }
        void VulkanRenderFrameBufferDescriptor::destroyRenderFrameBuffer()
        {
            int count = (int)m_aFrameBuffer.size();
            for (int i = 0; i < count; i++)
            {
                VulkanFrameBufferManager::GetSingleton().DeleteFrameBuffer(m_aFrameBuffer[i]);
            }
            m_aFrameBuffer.clear();
        }
        void VulkanRenderFrameBufferDescriptor::destroyRenderPass()
        {   
            if (m_pRenderPass != nullptr)
            {
                VulkanRenderPassManager::GetSingleton().DeleteRenderPass(m_pRenderPass);
            }
            m_pRenderPass = nullptr;
        }
        void VulkanRenderFrameBufferDescriptor::destroyTextures(TexturePtrVector& aTexture)
        {
            if (aTexture.empty())
                return;
            for (TexturePtrVector::iterator it = aTexture.begin(); 
                 it != aTexture.end(); ++it)
            {
                Texture* pTexture = (*it);
                TextureManager::GetSingleton().Delete(pTexture);
            }
            aTexture.clear();
        }

    bool VulkanRenderFrameBufferDescriptor::Init(FTextureType eTexture,
                                                 FPixelFormatType ePixelFormatColor,
                                                 FPixelFormatType ePixelFormatDepth,
                                                 FPixelFormatType ePixelFormatSwapChian,
                                                 FMSAASampleCountType eMSAASampleCount,
                                                 bool bIsUseImGUI)
    {   
        m_eTexture = eTexture;
        m_ePixelFormatColor = ePixelFormatColor;
        m_eVkFormatColor = VulkanConverter::Transform2VkFormat(ePixelFormatColor);
        m_ePixelFormatDepth = ePixelFormatDepth;
        m_eVkFormatDepth = VulkanConverter::Transform2VkFormat(ePixelFormatDepth);
        m_ePixelFormatSwapChian = ePixelFormatSwapChian;
        m_eVkFormatSwapChian = VulkanConverter::Transform2VkFormat(ePixelFormatSwapChian);
        m_eMSAASampleCount = eMSAASampleCount;
        m_eVkMSAASampleCount = VulkanConverter::Transform2VkSampleCountFlagBits(eMSAASampleCount);
        m_bIsUseImGUI = bIsUseImGUI;

        //1> Texture Color/Depth/SwapChain
        if (!createTextureColor(eTexture,
                                ePixelFormatColor,
                                eMSAASampleCount))
        {
            F_LogError("*********************** VulkanRenderFrameBufferDescriptor::Init: 1> Failed to create TextureColor !");
            return false;
        }
        if (!createTextureDepth(eTexture,
                                ePixelFormatDepth,
                                eMSAASampleCount))
        {
            F_LogError("*********************** VulkanRenderFrameBufferDescriptor::Init: 1> Failed to create TextureDepth !");
            return false;
        }
        F_LogInfo("VulkanRenderFrameBufferDescriptor::Init: 1> Success to create TextureColor/Depth/SwapChain !");

        //2> createRenderPass
        if (!createRenderPass())
        {
            F_LogError("*********************** VulkanRenderFrameBufferDescriptor::Init: 2> Failed to create RenderPass !");
            return false;
        }
        F_LogInfo("VulkanRenderFrameBufferDescriptor::Init: 2> Success to create RenderPass !");

        //3> createRenderFrameBuffer
        if (!createRenderFrameBuffer())
        {
            F_LogError("*********************** VulkanRenderFrameBufferDescriptor::Init: 2> Failed to create RenderFrameBuffer !");
            return false;
        }
        F_LogInfo("VulkanRenderFrameBufferDescriptor::Init: 3> Success to create RenderFrameBuffer !");

        return true;
    }
    bool VulkanRenderFrameBufferDescriptor::createTextureColor(FTextureType eTexture,
                                                               FPixelFormatType ePixelFormatColor, 
                                                               FMSAASampleCountType eMSAASampleCount)
    {
        if (ePixelFormatColor == F_PixelFormat_Unknown)
            return true;

        String nameTexture = "Texture-Color-" + GetName();
        Texture* pTextureColor = TextureManager::GetSingleton().CreateTextureManual(FPathManager::PathGroup_Texture,
                                                                                    nameTexture,
                                                                                    ResourceGroupManager::ms_strNameResourceGroup_Internal,
                                                                                    nullptr,
                                                                                    E_TextureUsage_FrameBufferColor,
                                                                                    eTexture,
                                                                                    F_TextureFilter_Bilinear,
                                                                                    F_TextureAddressing_Clamp,
                                                                                    F_TextureBorderColor_OpaqueBlack,
                                                                                    eMSAASampleCount,
                                                                                    ePixelFormatColor,
                                                                                    (uint32)m_pRenderWindow->GetSwapChain()->GetVkSwapChainImageWidth(),
                                                                                    (uint32)m_pRenderWindow->GetSwapChain()->GetVkSwapChainImageHeight(),
                                                                                    1,
                                                                                    TextureManager::ms_nBitDepthInteger_Default,
                                                                                    TextureManager::ms_nBitDepthFloat_Default,
                                                                                    false,
                                                                                    0,
                                                                                    false);
        if (!pTextureColor)
        {
            F_LogError("*********************** VulkanRenderFrameBufferDescriptor::createTextureColor: Failed to create TextureColor, name: [%s] !", nameTexture.c_str());
            return false;
        }
        m_aTextureColor.push_back(pTextureColor);

        F_LogInfo("VulkanRenderFrameBufferDescriptor::createTextureColor: Success to create TextureColor, name: [%s], type: [%s], PixelFormat: [%s], SampleCount: [%s] !",
                  nameTexture.c_str(), 
                  F_GetTextureTypeName(eTexture).c_str(),
                  FPixelFormat::GetPixelFormatName(ePixelFormatColor).c_str(),
                  F_GetMSAASampleCountTypeName(eMSAASampleCount).c_str());
        return true;
    }
    bool VulkanRenderFrameBufferDescriptor::createTextureDepth(FTextureType eTexture,
                                                               FPixelFormatType ePixelFormatDepth, 
                                                               FMSAASampleCountType eMSAASampleCount)    
    {
        String nameTexture = "Texture-Depth-" + GetName();
        Texture* pTextureDepth = TextureManager::GetSingleton().CreateTextureManual(FPathManager::PathGroup_Texture,
                                                                                    nameTexture,
                                                                                    ResourceGroupManager::ms_strNameResourceGroup_Internal,
                                                                                    nullptr,
                                                                                    E_TextureUsage_FrameBufferDepth,
                                                                                    eTexture,
                                                                                    F_TextureFilter_Bilinear,
                                                                                    F_TextureAddressing_Clamp,
                                                                                    F_TextureBorderColor_OpaqueBlack,
                                                                                    eMSAASampleCount,
                                                                                    ePixelFormatDepth,
                                                                                    (uint32)m_pRenderWindow->GetSwapChain()->GetVkSwapChainImageWidth(),
                                                                                    (uint32)m_pRenderWindow->GetSwapChain()->GetVkSwapChainImageHeight(),
                                                                                    1,
                                                                                    TextureManager::ms_nBitDepthInteger_Default,
                                                                                    TextureManager::ms_nBitDepthFloat_Default,
                                                                                    false,
                                                                                    0,
                                                                                    false);
        if (!pTextureDepth)
        {
            F_LogError("*********************** VulkanRenderFrameBufferDescriptor::createTextureDepth: Failed to create TextureDepth, name: [%s] !", nameTexture.c_str());
            return false;
        }
        m_aTextureDepth.push_back(pTextureDepth);

        F_LogInfo("VulkanRenderFrameBufferDescriptor::createTextureDepth: Success to create TextureDepth, name: [%s], type: [%s], PixelFormat: [%s], SampleCount: [%s] !",
                  nameTexture.c_str(), 
                  F_GetTextureTypeName(eTexture).c_str(),
                  FPixelFormat::GetPixelFormatName(ePixelFormatDepth).c_str(),
                  F_GetMSAASampleCountTypeName(eMSAASampleCount).c_str());
        return true;
    }
    bool VulkanRenderFrameBufferDescriptor::createTextureSwapChain()
    {
        
        return true;
    }

    bool VulkanRenderFrameBufferDescriptor::createRenderPass()
    {
        String nameRenderPass;
        bool bHasMSAA = HasMSAASampleCount();
        if (this->m_bIsUseImGUI)
        {
            if (bHasMSAA)
            {
                nameRenderPass = "RenderPass-ImGUI-MSAA-" + GetName();
                m_pRenderPass = VulkanRenderPassManager::GetSingleton().CreateRenderPass_ColorDepthImguiMSAA(nameRenderPass, 
                                                                                                             this->m_eVkFormatColor,
                                                                                                             this->m_eVkFormatDepth,
                                                                                                             this->m_eVkFormatSwapChian,
                                                                                                             this->m_eVkMSAASampleCount);
            }
            else
            {
                nameRenderPass = "RenderPass-ImGUI-" + GetName();
                m_pRenderPass = VulkanRenderPassManager::GetSingleton().CreateRenderPass_KhrDepthImgui(nameRenderPass, 
                                                                                                       this->m_eVkFormatColor,
                                                                                                       this->m_eVkFormatDepth,
                                                                                                       this->m_eVkFormatSwapChian);
            }
        }
        else
        {
            if (bHasMSAA)
            {
                nameRenderPass = "RenderPass-MSAA-" + GetName();
                m_pRenderPass = VulkanRenderPassManager::GetSingleton().CreateRenderPass_ColorDepthMSAA(nameRenderPass, 
                                                                                                        this->m_eVkFormatColor,
                                                                                                        this->m_eVkFormatDepth,
                                                                                                        this->m_eVkFormatSwapChian,
                                                                                                        this->m_eVkMSAASampleCount);
            }
            else
            {
                nameRenderPass = "RenderPass-" + GetName();
                m_pRenderPass = VulkanRenderPassManager::GetSingleton().CreateRenderPass_KhrDepth(nameRenderPass, 
                                                                                                  this->m_eVkFormatDepth,
                                                                                                  this->m_eVkFormatSwapChian);
            }
        }

         if (!m_pRenderPass)
        {
            F_LogError("*********************** VulkanRenderFrameBufferDescriptor::createRenderPass: Failed to create RenderPass, name: [%s], IsUseImGUI: [%s], HasMSAA: [%s] !", 
                       nameRenderPass.c_str(),
                       this->m_bIsUseImGUI ? "true" : "false",
                       bHasMSAA ? "true" : "false");
            return false;
        }

        F_LogInfo("VulkanRenderFrameBufferDescriptor::createRenderPass: Success to create RenderPass, IsUseImGUI: [%s], HasMSAA: [%s] !",
                  this->m_bIsUseImGUI ? "true" : "false",
                  bHasMSAA ? "true" : "false");
        return true;
    }
    bool VulkanRenderFrameBufferDescriptor::createRenderFrameBuffer()
    {
        bool bHasMSAA = HasMSAASampleCount();
        VulkanTexture* pTextureColor = nullptr;
        if (m_aTextureColor.size() > 0)
            pTextureColor = (VulkanTexture*)m_aTextureColor[0];
        VulkanTexture* pTextureDepth = nullptr;
        if (m_aTextureDepth.size() > 0)
            pTextureDepth = (VulkanTexture*)m_aTextureDepth[0];
        F_Assert(pTextureDepth && "VulkanRenderFrameBufferDescriptor::createRenderFrameBuffer: pTextureDepth is nullptr !")

        const VkImageViewVector& aSwapChainVkImageViews = m_pRenderWindow->GetSwapChainVkImageViews();
        int count = (int)aSwapChainVkImageViews.size();
        for (int i = 0; i < count; i++)
        {
            VkImageViewVector aImageViews;
            if (!this->m_bIsUseImGUI)
            {
                if (!bHasMSAA)
                {
                    aImageViews.push_back(aSwapChainVkImageViews[i]);
                    aImageViews.push_back(pTextureDepth->GetVkImageView());
                }
                else
                {
                    F_Assert(pTextureColor && "VulkanRenderFrameBufferDescriptor::createRenderFrameBuffer: pTextureColor is nullptr !")
                    aImageViews.push_back(pTextureColor->GetVkImageView());
                    aImageViews.push_back(pTextureDepth->GetVkImageView());
                    aImageViews.push_back(aSwapChainVkImageViews[i]);
                }
            }
            else
            {
                if (!bHasMSAA)
                {
                    aImageViews.push_back(aSwapChainVkImageViews[i]);
                    aImageViews.push_back(pTextureDepth->GetVkImageView());
                    aImageViews.push_back(aSwapChainVkImageViews[i]);
                }
                else
                {
                    F_Assert(pTextureColor && "VulkanRenderFrameBufferDescriptor::createRenderFrameBuffer: pTextureColor is nullptr !")
                    aImageViews.push_back(pTextureColor->GetVkImageView());
                    aImageViews.push_back(pTextureDepth->GetVkImageView());
                    aImageViews.push_back(aSwapChainVkImageViews[i]);
                    aImageViews.push_back(aSwapChainVkImageViews[i]);
                }
            }

            String nameFramebuffer = "Framebuffer-" + FUtilString::SaveSizeT(i) + "-" + GetName();
            VulkanFrameBuffer* pFrameBuffer = VulkanFrameBufferManager::GetSingleton().CreateFrameBuffer(nameFramebuffer,
                                                                                                         aImageViews,
                                                                                                         m_pRenderPass->GetVkRenderPass(),
                                                                                                         0,
                                                                                                         (uint32)m_pRenderWindow->GetSwapChain()->GetVkSwapChainImageWidth(),
                                                                                                         (uint32)m_pRenderWindow->GetSwapChain()->GetVkSwapChainImageHeight(),
                                                                                                         1);
            if (!pFrameBuffer)
            {
                F_LogError("*********************** VulkanRenderFrameBufferDescriptor::createRenderFrameBuffer: Failed to create FrameBuffer: name: [%s], index: [%d] !", nameFramebuffer.c_str(), i);
                return false;
            }
            m_aFrameBuffer.push_back(pFrameBuffer);
        }

        F_LogInfo("VulkanRenderFrameBufferDescriptor::createRenderFrameBuffer: Success to create FrameBuffer, Count: [%d] !", count);
        return true;
    }

}; //LostPeterPluginRendererVulkan