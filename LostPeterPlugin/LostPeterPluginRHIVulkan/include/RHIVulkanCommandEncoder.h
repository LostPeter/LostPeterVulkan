/****************************************************************************
* LostPeterPluginRHIVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-05
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _RHI_VULKAN_COMMAND_ENCODER_H_
#define _RHI_VULKAN_COMMAND_ENCODER_H_

#include "RHIVulkanPreDefine.h"

namespace LostPeterPluginRHIVulkan
{
    //////////////////////// RHIVulkanComputePassCommandEncoder ////////////////////////
    class rhiVulkanExport RHIVulkanComputePassCommandEncoder : public RHIComputePassCommandEncoder
    {
    public:
        RHIVulkanComputePassCommandEncoder(const RHIVulkanCommandBuffer& commandBuffer);
        virtual ~RHIVulkanComputePassCommandEncoder();

    public:
    protected:


    public:
        virtual void Destroy();

        virtual void SetPipeline(RHIPipelineCompute* pPipeline);
        virtual void SetBindGroup(uint8 nLayoutIndex, RHIBindGroup* pBindGroup);
        virtual void Dispatch(uint32 nGroupCountX, uint32 nGroupCountY, uint32 nGroupCountZ);
        virtual void EndPass();

    };

    //////////////////////// RHIVulkanGraphicsPassCommandEncoder ///////////////////////
    class rhiVulkanExport RHIVulkanGraphicsPassCommandEncoder : public RHIGraphicsPassCommandEncoder
    {
    public:
        RHIVulkanGraphicsPassCommandEncoder(const RHIVulkanCommandBuffer& commandBuffer);
        virtual ~RHIVulkanGraphicsPassCommandEncoder();

    public:
    protected:


    public:
        virtual void Destroy();

        virtual void SetPipeline(RHIPipelineGraphics* pPipeline);
        virtual void SetBindGroup(uint8 nLayoutIndex, RHIBindGroup* pBindGroup);
        virtual void SetIndexBuffer(RHIBufferView* pBufferView);
        virtual void SetVertexBuffer(uint32 nSlot, RHIBufferView* pBufferView);
        virtual void Draw(uint32 nVertexCount, uint32 nInstanceCount, uint32 nFirstVertex, uint32 nFirstInstance);
        virtual void DrawIndexed(uint32 nIndexCount, uint32 nInstanceCount, uint32 nFirstIndex, uint32 nBaseVertex, uint32 nFirstInstance);
        virtual void SetViewport(float fTopLeftX, float fTopLeftY, float fWidth, float fHeight, float fMinDepth, float fMaxDepth);
        virtual void SetScissor(uint32 nLeft, uint32 nTop, uint32 nRight, uint32 nBottom);
        virtual void SetPrimitiveTopology(RHIPrimitiveTopologyType ePrimitiveTopology);
        virtual void SetBlendConstant(const float* pConstants);
        virtual void SetStencilReference(uint32 nReference);
        virtual void EndPass();

    };

    //////////////////////// RHIVulkanCommandEncoder ///////////////////////////////////
    class rhiVulkanExport RHIVulkanCommandEncoder : public RHICommandEncoder
    {
    public:
        RHIVulkanCommandEncoder(const RHIVulkanCommandBuffer& commandBuffer);
        virtual ~RHIVulkanCommandEncoder();

    public:
    protected:
        const RHIVulkanCommandBuffer& m_commandBuffer;

    public:
        virtual void Destroy();

        virtual void CopyBufferToBuffer(RHIBuffer* pBufferSrc, uint32 nSrcOffset, RHIBuffer* pBufferDst, uint32 nDstOffset, uint32 nSize);
        virtual void CopyBufferToTexture(RHIBuffer* pBufferSrc, RHITexture* pTextureDst, const RHITextureSubResourceInfo* pSubResourceInfo, const RHIExtent<3>& sExtent);
        virtual void CopyTextureToBuffer(RHITexture* pTextureSrc, RHIBuffer* pBufferDst, const RHITextureSubResourceInfo* pSubResourceInfo, const RHIExtent<3>& sExtent);
        virtual void CopyTextureToTexture(RHITexture* pTextureSrc, const RHITextureSubResourceInfo* pSrcSubResourceInfo, RHITexture* pTextureDst, const RHITextureSubResourceInfo* pDstSubResourceInfo, const RHIExtent<3>& sExtent);
        virtual void ResourceBarrier(const RHIBarrier& barrier);

        virtual RHIVulkanComputePassCommandEncoder* BeginComputePass();
        virtual RHIVulkanGraphicsPassCommandEncoder* BeginGraphicsPass(const RHIGraphicsPassBeginInfo* pBeginInfo);
        virtual void SwapChainSync(RHISwapChain* pSwapChain);
        virtual void End();

    };
    
}; //LostPeterPluginRHIVulkan

#endif