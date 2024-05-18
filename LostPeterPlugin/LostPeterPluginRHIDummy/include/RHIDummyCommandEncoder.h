/****************************************************************************
* LostPeterPluginRHIDummy - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-05
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _RHI_DUMMY_COMMAND_ENCODER_H_
#define _RHI_DUMMY_COMMAND_ENCODER_H_

#include "RHIDummyPreDefine.h"

namespace LostPeterPluginRHIDummy
{
    //////////////////////// RHIDummyComputePassCommandEncoder ////////////////////////
    class rhiDummyExport RHIDummyComputePassCommandEncoder : public RHIComputePassCommandEncoder
    {
    public:
        RHIDummyComputePassCommandEncoder(RHIDummyCommandEncoder* pDummyCommandEncoder);
        virtual ~RHIDummyComputePassCommandEncoder();

    public:
    protected:
        RHIDummyCommandEncoder* m_pDummyCommandEncoder;

    public:
        F_FORCEINLINE RHIDummyCommandEncoder* GetDummyCommandEncoder() { return m_pDummyCommandEncoder; }

        
    public:
        virtual void Destroy();

        virtual void SetPipeline(RHIPipelineCompute* pPipeline);
        virtual void SetBindGroup(uint8 nLayoutIndex, RHIBindGroup* pBindGroup);
        virtual void Dispatch(uint32 nGroupCountX, uint32 nGroupCountY, uint32 nGroupCountZ);
        virtual void EndPass();
        
    };

    //////////////////////// RHIDummyGraphicsPassCommandEncoder ///////////////////////
    class rhiDummyExport RHIDummyGraphicsPassCommandEncoder : public RHIGraphicsPassCommandEncoder
    {
    public:
        RHIDummyGraphicsPassCommandEncoder(RHIDummyCommandEncoder* pDummyCommandEncoder);
        virtual ~RHIDummyGraphicsPassCommandEncoder();

    public:
    protected:
        RHIDummyCommandEncoder* m_pDummyCommandEncoder;

    public:
        F_FORCEINLINE RHIDummyCommandEncoder* GetDummyCommandEncoder() { return m_pDummyCommandEncoder; }


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

    //////////////////////// RHIDummyCommandEncoder ///////////////////////////////////
    class rhiDummyExport RHIDummyCommandEncoder : public RHICommandEncoder
    {
    public:
        RHIDummyCommandEncoder(RHIDummyCommandBuffer* pDummyCommandBuffer);
        virtual ~RHIDummyCommandEncoder();

    public:
    protected:
        RHIDummyCommandBuffer* m_pDummyCommandBuffer;

    public:
        F_FORCEINLINE RHIDummyCommandBuffer* GetDummyCommandBuffer() { return m_pDummyCommandBuffer; }

    public:
        virtual void Destroy();

        virtual void CopyBufferToBuffer(RHIBuffer* pBufferSrc, uint32 nSrcOffset, RHIBuffer* pBufferDst, uint32 nDstOffset, uint32 nSize);
        virtual void CopyBufferToTexture(RHIBuffer* pBufferSrc, RHITexture* pTextureDst, const RHITextureSubResourceInfo* pSubResourceInfo, const RHIExtent<3>& sExtent);
        virtual void CopyTextureToBuffer(RHITexture* pTextureSrc, RHIBuffer* pBufferDst, const RHITextureSubResourceInfo* pSubResourceInfo, const RHIExtent<3>& sExtent);
        virtual void CopyTextureToTexture(RHITexture* pTextureSrc, const RHITextureSubResourceInfo* pSrcSubResourceInfo, RHITexture* pTextureDst, const RHITextureSubResourceInfo* pDstSubResourceInfo, const RHIExtent<3>& sExtent);
        virtual void ResourceBarrier(const RHIBarrier& barrier);

        virtual RHIDummyComputePassCommandEncoder* BeginComputePass();
        virtual RHIDummyGraphicsPassCommandEncoder* BeginGraphicsPass(const RHIGraphicsPassBeginInfo* pBeginInfo);
        virtual void SwapChainSync(RHISwapChain* pSwapChain);
        virtual void End();
        
    };
    
}; //LostPeterPluginRHIDummy

#endif