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

#ifndef _VULKAN_RENDER_WINDOW_H_
#define _VULKAN_RENDER_WINDOW_H_

#include "VulkanPreDefine.h"
#include "VulkanViewport.h"

namespace LostPeterPluginRendererVulkan
{
    class VulkanRenderWindow : public RenderWindow
                             , public RenderTargetListener
    {
    public:
        VulkanRenderWindow(const String& nameRenderWindow, VulkanDevice* pDevice);
        virtual ~VulkanRenderWindow();

    public:
        static int s_maxFramesInFight;

    public:
        typedef std::map<Viewport*, VulkanViewport> VulkanViewportMap;

    public:
    protected:
        VulkanDevice* m_pDevice;

        FTextureType m_eTexture;
        FMSAASampleCountType m_eMSAASampleCount;
        FPixelFormatType m_ePixelFormatSwapChain;
        FPixelFormatType m_ePixelFormatDepth;
        bool m_bIsUseImGUI;

        VulkanSemaphorePtrVector m_aSemaphores_PresentComplete;
        VulkanSemaphorePtrVector m_aSemaphores_RenderComplete;
        VulkanFencePtrVector m_aFences_InFlight;
        VulkanFencePtrVector m_aFences_ImagesInFlight;

        bool m_bIsCreateRenderComputeSycSemaphore;
        VulkanSemaphore* m_pSemaphore_GraphicsWait;
        VulkanSemaphore* m_pSemaphore_ComputeWait;

        uint32 m_nSwapChainImageDesiredCount;
        VkImageVector m_aSwapChainVkImages;
	    VkImageViewVector m_aSwapChainVkImageViews;
        VulkanSwapChain* m_pSwapChain;
        VkCommandBufferVector m_aCommandBuffers;


        VulkanViewportMap m_mapVulkanViewport;
        VkViewportVector m_aVkViewports;
        VkRect2DVector m_aVkScissors;

    public:
        F_FORCEINLINE VulkanDevice* GetDevice() const { return m_pDevice; }
        F_FORCEINLINE uint32 GetSwapChainImageDesiredCount() const { return m_nSwapChainImageDesiredCount; }
        F_FORCEINLINE void SetSwapChainImageDesiredCount(uint32 nSwapChainImageDesiredCount) { m_nSwapChainImageDesiredCount = nSwapChainImageDesiredCount; }
        F_FORCEINLINE const VkImageVector& GetSwapChainVkImages() const { return m_aSwapChainVkImages; }
	    F_FORCEINLINE const VkImageViewVector& GetSwapChainVkImageViews() const { return m_aSwapChainVkImageViews; }
        F_FORCEINLINE VulkanSwapChain* GetSwapChain() const { return m_pSwapChain; }
        F_FORCEINLINE const VkCommandBufferVector& GetCommandBuffers() const { return m_aCommandBuffers; }


    public:
        virtual void Destroy();
		virtual bool Init(int32 nWidth, 
                          int32 nHeight, 
                          const String2StringMap* pParams);
        
    public:
        virtual FTextureType GetTextureType() const { return m_eTexture; }
        virtual FMSAASampleCountType GetMSAASampleCountType() const { return m_eMSAASampleCount; }
        virtual FPixelFormatType GetPixelFormatSwapChain() const { return m_ePixelFormatSwapChain; }
        virtual FPixelFormatType GetPixelFormatDepth() const { return m_ePixelFormatDepth; }

		virtual bool IsClosed() const;
		virtual void EmptyGPUCommandBuffer();
        virtual bool SwapBuffers(bool bSwapBuffers = true);
        virtual bool RequiresTextureFlipping() const;

    public:
        void DestroySwapChain();
        bool RecreateSwapChain();

    protected:
        //DestroySwapChain
        void destroyCommandBuffers();
        void destroyRenderPassDescriptor();
        void destroyRenderFrameBufferDescriptor();
        void destroySwapChain();

        void destroySyncObjects_RenderCompute();
        void destroySyncObjects_PresentRender();
        
        bool createSyncObjects_PresentRender();
        bool createSyncObjects_RenderCompute();

        //RecreateSwapChain
        bool createSwapChain();
        bool createRenderFrameBufferDescriptor();
        bool createRenderPassDescriptor();
        bool createCommandBuffers();

    public: 
        virtual void ViewportAdded(const RenderTargetViewportEvent& evt);
        virtual void ViewportResized(const RenderTargetViewportEvent& evt);
		virtual void ViewportRemoved(const RenderTargetViewportEvent& evt);

    protected:  
        void clearVkViewports();
        void updateVkViewports();

    public:
        //Common/Window
        virtual void OnInit();
        virtual void OnLoad();
        virtual bool OnIsInit();
        virtual void OnResize(int w, int h, bool force);
		virtual void OnDestroy();

		//Compute/Render
        virtual bool OnBeginCompute();
            virtual void OnUpdateCompute();
            virtual void OnCompute();
        virtual void OnEndCompute();
        virtual bool OnBeginRender();
            virtual void OnUpdateRender();
            virtual void OnRender();
        virtual void OnEndRender();

    };

}; //LostPeterPluginRendererVulkan

#endif