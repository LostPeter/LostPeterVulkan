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
#include "../include/VulkanRenderPass.h"
#include "../include/VulkanFrameBuffer.h"
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
        m_aTextureSwapChain.clear();
        destroyTextures(m_aTextureDepth);
        destroyTextures(m_aTextureColor);
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
                                                 FMSAASampleCountType eMSAASampleCount,
                                                 bool bHasImGUI)
    {   
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
                                                                                    (uint32)m_pSwapChain->GetVkSwapChainImageWidth(),
                                                                                    (uint32)m_pSwapChain->GetVkSwapChainImageHeight(),
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
                                                                                    (uint32)m_pSwapChain->GetVkSwapChainImageWidth(),
                                                                                    (uint32)m_pSwapChain->GetVkSwapChainImageHeight(),
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

        return true;
    }
    bool VulkanRenderFrameBufferDescriptor::createRenderFrameBuffer()
    {

        return true;
    }


}; //LostPeterPluginRendererVulkan