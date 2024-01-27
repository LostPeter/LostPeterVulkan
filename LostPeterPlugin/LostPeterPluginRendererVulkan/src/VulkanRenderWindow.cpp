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
#include "../include/VulkanQueue.h"
#include "../include/VulkanSemaphore.h"
#include "../include/VulkanFence.h"
#include "../include/VulkanFenceManager.h"
#include "../include/VulkanRenderFrameBufferDescriptor.h"
#include "../include/VulkanRenderPassDescriptor.h"
#include "../include/VulkanConverter.h"

namespace LostPeterPluginRendererVulkan
{
    int VulkanRenderWindow::s_maxFramesInFight = 2;
    VulkanRenderWindow::VulkanRenderWindow(const String& nameRenderWindow, VulkanDevice* pDevice)
        : RenderWindow(nameRenderWindow)
        , m_pDevice(pDevice)

        , m_eTexture(F_Texture_2D)
        , m_eMSAASampleCount(F_MSAASampleCount_1_Bit)
        , m_ePixelFormatSwapChain(F_PixelFormat_BYTE_A8R8G8B8_SRGB)
        , m_bIsUseImGUI(true)

        , m_nSwapChainImageDesiredCount(3)
        , m_pSwapChain(nullptr)

        , m_bIsCreateRenderComputeSycSemaphore(false)
        , m_pSemaphore_GraphicsWait(nullptr)
        , m_pSemaphore_ComputeWait(nullptr)
    {
        F_Assert(m_pDevice && "VulkanRenderWindow::VulkanRenderWindow")
        AddRenderTargetListener(this);
    }   

    VulkanRenderWindow::~VulkanRenderWindow()
    {
        Destroy();
    }

    void VulkanRenderWindow::Destroy()
    {
        clearVkViewports();

        DestroySwapChain();
        F_DELETE(m_pSwapChain)
        destroySyncObjects_RenderCompute();
        destroySyncObjects_PresentRender();

        RenderWindow::Destroy();
    }
        void VulkanRenderWindow::destroySyncObjects_RenderCompute()
        {
            F_DELETE(m_pSemaphore_GraphicsWait)
            F_DELETE(m_pSemaphore_ComputeWait)
        }
        void VulkanRenderWindow::destroySyncObjects_PresentRender()
        {
            m_pDevice->DestroyVkSemaphores(m_aSemaphores_PresentComplete);
            m_aSemaphores_PresentComplete.clear();
            m_pDevice->DestroyVkSemaphores(m_aSemaphores_RenderComplete);
            m_aSemaphores_RenderComplete.clear();
            m_pDevice->RecoveryFences(m_aFences_InFlight);
            m_aFences_InFlight.clear();
            m_aFences_ImagesInFlight.clear();
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
            F_LogError("*********************** VulkanRenderWindow::Init: 1> Create WindowBase failed, name window: [%s] !", nameWindow.c_str());
            return false;
        }
        F_LogInfo("VulkanRenderWindow::Init: 1> Create WindowBase success, name: [%s] !", nameWindow.c_str());

        //2> createSyncObjects_PresentRender
        if (!createSyncObjects_PresentRender())
        {
            F_LogError("*********************** VulkanRenderWindow::Init: 2> Create SyncObjects PresentRender failed, name window: [%s] !", nameWindow.c_str());
            return false;
        }
        F_LogInfo("VulkanRenderWindow::Init: 2> Create SyncObjects PresentRender success !");

        //3> createSyncObjects_RenderCompute
        if (this->m_bIsCreateRenderComputeSycSemaphore)
        {
            if (!createSyncObjects_RenderCompute())
            {
                F_LogError("*********************** VulkanRenderWindow::Init: 3> Create SyncObjects RenderCompute failed, name window: [%s] !", nameWindow.c_str());
                return false;
            }
            F_LogInfo("VulkanRenderWindow::Init: 3> Create SyncObjects RenderCompute success !");
        }

        //4> Create SwapChain
        if (!RecreateSwapChain())
        {   
            F_LogError("*********************** VulkanRenderWindow::Init: 4> Create SwapChain failed, name window: [%s] !", nameWindow.c_str());
            return false;
        }
        F_LogInfo("VulkanRenderWindow::Init: 4> Create SwapChain success !");

        
        F_LogInfo("VulkanRenderWindow::Init: Create RenderWindow success, name window: [%s] !", nameWindow.c_str());
        return true;
    }
        bool VulkanRenderWindow::createSyncObjects_PresentRender()
        {
            this->m_aSemaphores_PresentComplete.clear();
            this->m_aSemaphores_RenderComplete.clear();
            this->m_aFences_InFlight.clear();
            this->m_aFences_ImagesInFlight.clear();

            for (int i = 0; i < s_maxFramesInFight; i++) 
            {
                VulkanSemaphore* pSemaphore_PresentComplete = new VulkanSemaphore(m_pDevice);
                VulkanSemaphore* pSemaphore_RenderComplete = new VulkanSemaphore(m_pDevice);
                VulkanFence* pFence_InFlight = VulkanFenceManager::GetSingleton().CreateFence(true);

                if (!pSemaphore_PresentComplete->Init() || 
                    !pSemaphore_RenderComplete->Init() ||
                    !pFence_InFlight) 
                {
                    F_LogError("*********************** VulkanRenderWindow::createSyncObjects_PresentRender: Failed to create present/render synchronization objects for a frame !");
                    return false;
                }

                this->m_aSemaphores_PresentComplete.push_back(pSemaphore_PresentComplete);
                this->m_aSemaphores_RenderComplete.push_back(pSemaphore_RenderComplete);
                this->m_aFences_InFlight.push_back(pFence_InFlight);
                this->m_aFences_ImagesInFlight.push_back(nullptr);
            }
            
            F_LogInfo("VulkanRenderWindow::createSyncObjects_PresentRender: Success to create present/render synchronization objects for a frame !");
            return true;
        }
        bool VulkanRenderWindow::createSyncObjects_RenderCompute()
        {
            VkSemaphoreCreateInfo semaphoreInfo = {};
            semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

            //1> Semaphore GraphicsWait
            m_pSemaphore_GraphicsWait = new VulkanSemaphore(m_pDevice);
            if (!m_pSemaphore_GraphicsWait->Init())
            {
                F_LogError("*********************** VulkanRenderWindow::createRenderComputeSyncObjects: Failed to create Semaphore GraphicsWait !");
                return false;
            }
            VkSemaphore vkSignalSemaphores = m_pSemaphore_GraphicsWait->GetVkSemaphore();
            m_pDevice->QueueSubmitVkCommandBuffers(m_pDevice->GetQueueGraphics()->GetVkQueue(),
                                                   0,
                                                   nullptr,
                                                   1,
                                                   &vkSignalSemaphores,
                                                   nullptr);
            m_pDevice->QueueWaitIdle(m_pDevice->GetQueueGraphics()->GetVkQueue());                 

            //2> Semaphore ComputeWait
            m_pSemaphore_ComputeWait = new VulkanSemaphore(m_pDevice);
            if (!m_pSemaphore_GraphicsWait->Init())
            {
                F_LogError("*********************** VulkanRenderWindow::createRenderComputeSyncObjects: Failed to create Semaphore ComputeWait !");
                return false;
            }

            F_LogInfo("VulkanRenderWindow::createSyncObjects_RenderCompute: Success to create Semaphore GraphicsWait/ComputeWait !");
            return true;
        }
        

    bool VulkanRenderWindow::IsClosed() const
    {
        return false;
    }
    
    void VulkanRenderWindow::EmptyGPUCommandBuffer()
    {

    }

    bool VulkanRenderWindow::SwapBuffers(bool bSwapBuffers /*= true*/)
    {
        if (!bSwapBuffers)
            return true;

        VulkanSwapStatusType typeSwapStatus = m_pSwapChain->Present(nullptr);
        if (typeSwapStatus == Vulkan_SwapStatus_OutOfDate)
        {
            RecreateSwapChain();
            return false;
        }
        else if (typeSwapStatus == Vulkan_SwapStatus_Lost)
        {
            RecreateSwapChain();
            return false;
        }

        return true;
    }

    bool VulkanRenderWindow::RequiresTextureFlipping() const
    {
        return false;
    }

    void VulkanRenderWindow::DestroySwapChain()
    {
        F_LogInfo("----- VulkanRenderWindow::DestroySwapChain Start -----");
        {   
            destroyCommandBuffers();
            destroyRenderPassDescriptor();
            destroyRenderFrameBufferDescriptor();
            destroySwapChain();
        }
        F_LogInfo("----- VulkanRenderWindow::DestroySwapChain End -------");
    }
        void VulkanRenderWindow::destroyCommandBuffers()
        {
            if (m_aCommandBuffers.size() > 0)
            {
                m_pDevice->FreeVkCommandBuffers(m_pDevice->GetVkCommandPoolGraphics(),
                                                (uint32_t)m_aCommandBuffers.size(),
                                                m_aCommandBuffers.data());
            }
            m_aCommandBuffers.clear();
        }
        void VulkanRenderWindow::destroyRenderPassDescriptor()
        {
            F_DELETE(m_pRenderPassDescriptor)
        }
        void VulkanRenderWindow::destroyRenderFrameBufferDescriptor()
        {
            F_DELETE(m_pRenderFrameBufferDescriptor)
        }
        void VulkanRenderWindow::destroySwapChain()
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
        F_LogInfo("++++++++++ VulkanRenderWindow::RecreateSwapChain Start ++++++++++");
        {
            m_pDevice->WaitVkDeviceIdle();

            DestroySwapChain();

            //[1] createSwapChain
            if (!createSwapChain())
            {
                F_LogError("*********************** VulkanRenderWindow::RecreateSwapChain: [1] Failed to create SwapChain, name window: [%s] !", GetName().c_str());
                return false;
            }
            F_LogInfo("VulkanRenderWindow::RecreateSwapChain: [1] Success to create SwapChain !");

            //[2] createRenderFrameBufferDescriptor
            if (!createRenderFrameBufferDescriptor())
            {
                F_LogError("*********************** VulkanRenderWindow::RecreateSwapChain: [2] Failed to create RenderFrameBufferDescriptor, name window: [%s] !", GetName().c_str());
                return false;
            }
            F_LogInfo("VulkanRenderWindow::RecreateSwapChain: [2] Success to create RenderFrameBufferDescriptor !");

            //[3] createRenderPassDescriptor
            if (!createRenderPassDescriptor())
            {
                F_LogError("*********************** VulkanRenderWindow::RecreateSwapChain: [3] Failed to create RenderPassDescriptor, name window: [%s] !", GetName().c_str());
                return false;
            }
            F_LogInfo("VulkanRenderWindow::RecreateSwapChain: [3] Success to create RenderPassDescriptor !");

            //[4] createCommandBuffers
            if (!createCommandBuffers())
            {
                F_LogError("*********************** VulkanRenderWindow::RecreateSwapChain: [4] Failed to create CommandBuffers, name window: [%s] !", GetName().c_str());
                return false;
            }
            F_LogInfo("VulkanRenderWindow::RecreateSwapChain: [4] Success to create CommandBuffers !");


        }       
        F_LogInfo("++++++++++ VulkanRenderWindow::RecreateSwapChain End ++++++++++++");
        return true;
    }
        bool VulkanRenderWindow::createSwapChain()
        {
            const String& nameWindow = GetName();

            //1> Window Width/Height
            int nWindowWidth = 0;
            int nWindowHeight = 0;
            RefreshWindowSize(nWindowWidth, nWindowHeight);
            if (nWindowWidth <= 0 || nWindowHeight <= 0)
            {
                F_LogError("*********************** VulkanRenderWindow::createSwapChain: Wrong window size: Width: [%d], Height: [%d], name: [%s] !", nWindowWidth, nWindowHeight, nameWindow.c_str());
                return false;
            }

            //2> SwapChain
            if (m_pSwapChain == nullptr)
            {
                m_pSwapChain = new VulkanSwapChain(this->m_pDevice);
            }
            if (!m_pSwapChain->Init(m_pWindow,
                                    m_ePixelFormatSwapChain,
                                    nWindowWidth,
                                    nWindowHeight,
                                    &this->m_nSwapChainImageDesiredCount,
                                    this->m_aSwapChainVkImages,
                                    1))
            {
                F_LogError("*********************** VulkanInstance::createSwapChain: VulkanSwapChain.Init failed, name: [%s] !", nameWindow.c_str());
                return false;
            }

            //3> VkImageView
            m_aSwapChainVkImageViews.clear();
            size_t count = m_aSwapChainVkImages.size();
            for (size_t i = 0; i < count; ++i)
            {
                VkImageView vkImageView = VK_NULL_HANDLE;
                if (!m_pDevice->CreateVkImageView(m_aSwapChainVkImages[i],
                                                  VK_IMAGE_VIEW_TYPE_2D,
                                                  VulkanConverter::Transform2VkFormat(m_ePixelFormatSwapChain),
                                                  VulkanConverter::Transform2VkComponentMapping(m_ePixelFormatSwapChain),
                                                  VK_IMAGE_ASPECT_COLOR_BIT,
                                                  1,
                                                  1,
                                                  vkImageView))
                {
                    F_LogError("*********************** VulkanInstance::createSwapChain: CreateVkImageView failed, name: [%s] !", nameWindow.c_str());
                    return false;
                }   
                m_aSwapChainVkImageViews.push_back(vkImageView);
            }
            return true;
        }
        bool VulkanRenderWindow::createRenderFrameBufferDescriptor()
        {
            this->m_ePixelFormatDepth = m_pDevice->FindDepthPixelFormatType();
            String nameRenderFrameBufferDescriptor = "RenderFrameBufferDescriptor-" + GetName();
            m_pRenderFrameBufferDescriptor = new VulkanRenderFrameBufferDescriptor(nameRenderFrameBufferDescriptor, m_pDevice, this);
            if (!m_pRenderFrameBufferDescriptor->Init(this->m_eTexture,
                                                      this->m_ePixelFormatSwapChain,
                                                      this->m_ePixelFormatDepth,
                                                      this->m_ePixelFormatSwapChain,
                                                      this->m_eMSAASampleCount,
                                                      this->m_bIsUseImGUI))
            {
                F_LogError("*********************** VulkanRenderWindow::createRenderFrameBufferDescriptor: Failed to create VulkanRenderPassDescriptor !");
                return false;
            }
            return true;
        }
        bool VulkanRenderWindow::createRenderPassDescriptor()
        {
            

            return true;
        }
        bool VulkanRenderWindow::createCommandBuffers()
        {   
            m_aCommandBuffers.resize(m_aSwapChainVkImageViews.size());
            if (!m_pDevice->AllocateVkCommandBuffers(m_pDevice->GetVkCommandPoolGraphics(),
                                                     VK_COMMAND_BUFFER_LEVEL_PRIMARY,
                                                     (uint32_t)m_aCommandBuffers.size(),
                                                     m_aCommandBuffers.data()))
            {
                F_LogError("*********************** VulkanRenderWindow::createCommandBuffers: Failed to allocate VkCommandBuffers !");
                return false;
            }

            return true;
        }

    void VulkanRenderWindow::ViewportAdded(const RenderTargetViewportEvent& evt)
    {
        if (evt.pViewportSrc == nullptr)
            return;

        VulkanViewportMap::iterator itFind = m_mapVulkanViewport.find(evt.pViewportSrc);
        if (itFind == m_mapVulkanViewport.end())
        {
            VulkanViewport vp;
            vp.Init(evt.pViewportSrc);
            m_mapVulkanViewport[evt.pViewportSrc] = vp;
        }
        updateVkViewports();
    }
    void VulkanRenderWindow::ViewportResized(const RenderTargetViewportEvent& evt)
    {
        if (evt.pViewportSrc == nullptr)
            return;

        VulkanViewportMap::iterator itFind = m_mapVulkanViewport.find(evt.pViewportSrc);
        if (itFind != m_mapVulkanViewport.end())
        {
            m_mapVulkanViewport.erase(itFind);
        }
        updateVkViewports();
    }
	void VulkanRenderWindow::ViewportRemoved(const RenderTargetViewportEvent& evt)
    {
        if (evt.pViewportSrc == nullptr)
            return;

        for (ViewportPtrOrderMap::iterator it = m_mapViewport.begin();
			 it != m_mapViewport.end(); ++it)
		{
            VulkanViewportMap::iterator itFind = m_mapVulkanViewport.find(it->second);
            VulkanViewport& vp = itFind->second;
            vp.Init(it->second);
		}
        updateVkViewports();
    }
    void VulkanRenderWindow::clearVkViewports()
    {
        m_mapVulkanViewport.clear();
        m_aVkViewports.clear();
        m_aVkScissors.clear();
    }
    void VulkanRenderWindow::updateVkViewports()
    {
        m_aVkViewports.clear();
        m_aVkScissors.clear();
        for (ViewportPtrOrderMap::iterator it = m_mapViewport.begin();
			 it != m_mapViewport.end(); ++it)
		{
            VulkanViewportMap::iterator itFind = m_mapVulkanViewport.find(it->second);
            VulkanViewport& vp = itFind->second;

            m_aVkViewports.push_back(vp.GetVkViewport());
            m_aVkScissors.push_back(vp.GetScissor());
		}
    }   

    //Common/Window
    void VulkanRenderWindow::OnInit()
    {
        RenderWindow::OnInit();
    }

    void VulkanRenderWindow::OnLoad()
    {
        RenderWindow::OnLoad();
    }

    bool VulkanRenderWindow::OnIsInit()
    {
        RenderWindow::OnIsInit();

        return true;
    }

    void VulkanRenderWindow::OnResize(int w, int h, bool force)
    {
        UpdateViewportAll();

        RenderWindow::OnResize(w, h, force);
    }   

    void VulkanRenderWindow::OnDestroy()
    {
        RenderWindow::OnDestroy();
    }

    //Compute/Render
    bool VulkanRenderWindow::OnBeginCompute()
    {
        return true;
    }
        void VulkanRenderWindow::OnUpdateCompute()
        {

        }
        void VulkanRenderWindow::OnCompute()
        {

        }
    void VulkanRenderWindow::OnEndCompute()
    {

    }

    bool VulkanRenderWindow::OnBeginRender()
    {
        return true;
    }
        void VulkanRenderWindow::OnUpdateRender()
        {

        }
        void VulkanRenderWindow::OnRender()
        {

        }
    void VulkanRenderWindow::OnEndRender()
    {
        
    }

}; //LostPeterPluginRendererVulkan