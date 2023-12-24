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

namespace LostPeterPluginRendererVulkan
{
    class VulkanRenderWindow : public RenderWindow
    {
    public:
        VulkanRenderWindow(const String& nameRenderWindow, VulkanDevice* pDevice);
        virtual ~VulkanRenderWindow();

    public:
        static int s_maxFramesInFight;

    public:
    protected:
        VulkanDevice* m_pDevice;

        FPixelFormatType m_eSwapChainImagePixelFormat;
        uint32 m_nDesiredNumSwapChainImages;
        VkImageVector m_aSwapChainVkImages;
	    VkImageViewVector m_aSwapChainVkImageViews;
        VulkanSwapChain* m_pSwapChain;
        

        VulkanSemaphorePtrVector m_aSemaphores_PresentComplete;
        VulkanSemaphorePtrVector m_aSemaphores_RenderComplete;
        VulkanFencePtrVector m_aFences_InFlight;
        VulkanFencePtrVector m_aFences_ImagesInFlight;

        bool m_bIsCreateRenderComputeSycSemaphore;
        VulkanSemaphore* m_pSemaphore_GraphicsWait;
        VulkanSemaphore* m_pSemaphore_ComputeWait;
        

    public:
        F_FORCEINLINE VulkanDevice* GetDevice() const { return m_pDevice; }
        F_FORCEINLINE FPixelFormatType GetSwapChainImagePixelFormat() const { return m_eSwapChainImagePixelFormat; }
        F_FORCEINLINE uint32 GetDesiredNumSwapChainImages() const { return m_nDesiredNumSwapChainImages; }
        F_FORCEINLINE void SetDesiredNumSwapChainImages(uint32 nDesiredNumSwapChainImages) { m_nDesiredNumSwapChainImages = nDesiredNumSwapChainImages; }
        F_FORCEINLINE const VkImageVector& GetSwapChainVkImages() const { return m_aSwapChainVkImages; }
	    F_FORCEINLINE const VkImageViewVector& GetSwapChainVkImageViews() const { return m_aSwapChainVkImageViews; }
        F_FORCEINLINE VulkanSwapChain* GetSwapChain() const { return m_pSwapChain; }

    public:
        virtual void Destroy();
		virtual bool Init(int32 nWidth, 
                          int32 nHeight, 
                          const String2StringMap* pParams);


    public:
        virtual void Resize(int32 nWidth, int32 nHeight);
		virtual void Reposition(int32 nLeft, int32 nTop);
		virtual bool IsClosed() const;
		virtual void WindowMovedOrResized();
		virtual bool CanChangeToWindowMode(int32 srcWidth, int32 srcHeight, int32& destWidth, int32& destHeight);
		
		virtual void EmptyGPUCommandBuffer();

        virtual bool SwapBuffers(bool bSwapBuffers = true);

        virtual bool RequiresTextureFlipping() const;

    public:
        void DestroySwapChain();
        bool RecreateSwapChain();

    protected:
        void destroySyncObjects_RenderCompute();
        void destroySyncObjects_PresentRender();

        bool createSyncObjects_PresentRender();
        bool createSyncObjects_RenderCompute();
    };

}; //LostPeterPluginRendererVulkan

#endif