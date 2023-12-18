/****************************************************************************
* LostPeterPluginRendererVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-12-07
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/VulkanRenderWindow.h"
#include "../include/VulkanDevice.h"
#include "../include/VulkanSwapChain.h"
#include "../include/VulkanConverter.h"

namespace LostPeterPluginRendererVulkan
{
    VulkanRenderWindow::VulkanRenderWindow(const String& nameRenderWindow, VulkanDevice* pDevice)
        : RenderWindow(nameRenderWindow)
        , m_pDevice(pDevice)
        , m_eSwapChainImagePixelFormat(F_PixelFormat_BYTE_A8R8G8B8_SRGB)
        , m_nDesiredNumSwapChainImages(3)
        , m_pSwapChain(nullptr)
    {
        F_Assert(m_pDevice && "VulkanRenderWindow::VulkanRenderWindow")
    }   

    VulkanRenderWindow::~VulkanRenderWindow()
    {
        Destroy();
    }

    void VulkanRenderWindow::Destroy()
    {
        DestroySwapChain();
        F_DELETE(m_pSwapChain)

        RenderWindow::Destroy();
    }

	bool VulkanRenderWindow::Init(int32 nWidth, 
                                  int32 nHeight, 
                                  const String2StringMap* pParams)
    {
        const String& nameWindow = GetName();
        //0> Params
        {
            //PixelFormat

            //SwapChainImage Count

        }

        //1> Create GLFWwindow
        if (!WindowBase::Init(nameWindow, nWidth, nHeight))
        {
            F_LogError("*********************** VulkanRenderWindow::Init: Create GLFWwindow failed, name: [%s] !", nameWindow.c_str());
            return false;
        }

        //2> Create SwapChain
        if (!RecreateSwapChain())
        {   
            F_LogError("*********************** VulkanRenderWindow::Init: Create SwapChain failed, name: [%s] !", nameWindow.c_str());
            return false;
        }

        F_LogInfo("VulkanRenderWindow::Init: Create render window success, name: [%s] !", nameWindow.c_str());
        return true;
    }

    void VulkanRenderWindow::Resize(int32 nWidth, int32 nHeight)
    {

    }

    void VulkanRenderWindow::Reposition(int32 nLeft, int32 nTop)
    {

    }

    bool VulkanRenderWindow::IsClosed() const
    {
        return false;
    }

    void VulkanRenderWindow::WindowMovedOrResized()
    {

    }

    bool VulkanRenderWindow::CanChangeToWindowMode(int32 srcWidth, int32 srcHeight, int32& destWidth, int32& destHeight)
    {
        return true;
    }
    
    void VulkanRenderWindow::EmptyGPUCommandBuffer()
    {

    }

    bool VulkanRenderWindow::RequiresTextureFlipping() const
    {
        return false;
    }

    void VulkanRenderWindow::DestroySwapChain()
    {
        if (m_pSwapChain != nullptr)
        {
            m_pSwapChain->Destroy();
        }

        size_t count = m_aSwapChainVkImageViews.size();
        for (size_t i = 0; i < count; ++i) 
        {
            m_pDevice->DestroyVkImageView(m_aSwapChainVkImageViews[i]);
        }
        m_aSwapChainVkImageViews.clear();
        m_aSwapChainVkImages.clear();

    }
    bool VulkanRenderWindow::RecreateSwapChain()
    {
        const String& nameWindow = GetName();

        //1> Window Width/Height
        int nWindowWidth = 0;
        int nWindowHeight = 0;
        RefreshWindowSize(nWindowWidth, nWindowHeight);
        if (nWindowWidth <= 0 || nWindowHeight <= 0)
        {
            F_LogError("*********************** VulkanRenderWindow::RecreateSwapChain: Wrong window size: width: [%d], height: [%d], name: [%s] !", nWindowWidth, nWindowHeight, nameWindow.c_str());
            return false;
        }

        //2> SwapChain
        if (m_pSwapChain == nullptr)
        {
            m_pSwapChain = new VulkanSwapChain(this->m_pDevice);
        }
        if (!m_pSwapChain->Init(m_pWindow,
                                m_eSwapChainImagePixelFormat,
                                nWindowWidth,
                                nWindowHeight,
                                &this->m_nDesiredNumSwapChainImages,
                                this->m_aSwapChainVkImages,
                                1))
        {
            F_LogError("*********************** VulkanInstance::RecreateSwapChain: VulkanSwapChain.Init failed, name: [%s] !", nameWindow.c_str());
            return false;
        }

        //3> VkImageView
        size_t count = m_aSwapChainVkImages.size();
        for (size_t i = 0; i < count; ++i)
        {
            VkImageView vkImageView = VK_NULL_HANDLE;
            if (!m_pDevice->CreateVkImageView(m_aSwapChainVkImages[i],
                                              VK_IMAGE_VIEW_TYPE_2D,
                                              VulkanConverter::Transform2VkFormat(m_eSwapChainImagePixelFormat),
                                              VulkanConverter::Transform2VkComponentMapping(m_eSwapChainImagePixelFormat),
                                              VK_IMAGE_ASPECT_COLOR_BIT,
                                              1,
                                              1,
                                              vkImageView))
            {
                F_LogError("*********************** VulkanInstance::RecreateSwapChain: CreateVkImageView failed, name: [%s] !", nameWindow.c_str());
                return false;
            }   
            
            m_aSwapChainVkImageViews.push_back(vkImageView);
        }

        return true;
    }

}; //LostPeterPluginRendererVulkan