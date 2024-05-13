/****************************************************************************
* LostPeterRHI - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-05
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _RHI_DEVICE_H_
#define _RHI_DEVICE_H_

#include "RHIPreDefine.h"

namespace LostPeterRHI
{
    class rhiExport RHIDevice : public FNonCopyable
    {
    public:
        RHIDevice(const RHIDeviceCreateInfo& createInfo);
        virtual ~RHIDevice();

    public:
    protected:


    public:
        virtual void Destroy() = 0;
        
        virtual uint32 GetQueueCount(RHIQueueType eQueue) = 0;
        virtual RHIQueue* GetQueue(RHIQueueType eQueue, uint32 nIndex) = 0;
        virtual RHISurface* CreateSurface(const RHISurfaceCreateInfo& createInfo) = 0;
        virtual RHISwapChain* CreateSwapChain(const RHISwapChainCreateInfo& createInfo) = 0;
        virtual RHIBuffer* CreateBuffer(const RHIBufferCreateInfo& createInfo) = 0;
        virtual RHITexture* CreateTexture(const RHITextureCreateInfo& createInfo) = 0;
        virtual RHISampler* CreateSampler(const RHISamplerCreateInfo& createInfo) = 0;
        virtual RHIBindGroupLayout* CreateBindGroupLayout(const RHIBindGroupLayoutCreateInfo& createInfo) = 0;
        virtual RHIBindGroup* CreateBindGroup(const RHIBindGroupCreateInfo& createInfo) = 0;
        virtual RHIPipelineLayout* CreatePipelineLayout(const RHIPipelineLayoutCreateInfo& createInfo) = 0;
        virtual RHIShaderModule* CreateShaderModule(const RHIShaderModuleCreateInfo& createInfo) = 0;
        virtual RHIPipelineCompute* CreatePipelineCompute(const RHIPipelineComputeCreateInfo& createInfo) = 0;
        virtual RHIPipelineGraphics* CreatePipelineGraphics(const RHIPipelineGraphicsCreateInfo& createInfo) = 0;
        virtual RHICommandBuffer* CreateCommandBuffer() = 0;
        virtual RHIFence* CreateFence() = 0;

        virtual bool CheckSwapChainFormatSupport(RHISurface* pSurface, RHIPixelFormatType ePixelFormat) = 0;
    };

}; //LostPeterRHI

#endif