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

#ifndef _RHI_COMMAND_ENCODER_H_
#define _RHI_COMMAND_ENCODER_H_

#include "RHIPreDefine.h"

namespace LostPeterRHI
{
    //////////////////////// RHIComputePassCommandEncoder ////////////////////////
    class rhiExport RHIComputePassCommandEncoder : public FNonCopyable
    {
    public:
        RHIComputePassCommandEncoder(RHICommandEncoder* pCommandEncoder);
        virtual ~RHIComputePassCommandEncoder();

    public:
    protected:
        RHICommandEncoder* m_pCommandEncoder;

    public:
        F_FORCEINLINE RHICommandEncoder* GetCommandEncoder() { return m_pCommandEncoder; }

        
    public:
        virtual void Destroy() = 0;

        virtual void SetPipeline(RHIPipelineCompute* pPipeline) = 0;
        virtual void SetBindGroup(uint8 nLayoutIndex, RHIBindGroup* pBindGroup) = 0;
        virtual void Dispatch(uint32 nGroupCountX, uint32 nGroupCountY, uint32 nGroupCountZ) = 0;
        virtual void EndPass() = 0;
        
    };

    //////////////////////// RHIGraphicsPassCommandEncoder ///////////////////////
    class rhiExport RHIGraphicsPassCommandEncoder : public FNonCopyable
    {
    public:
        RHIGraphicsPassCommandEncoder(RHICommandEncoder* pCommandEncoder);
        virtual ~RHIGraphicsPassCommandEncoder();

    public:
    protected:
        RHICommandEncoder* m_pCommandEncoder;

    public:
        F_FORCEINLINE RHICommandEncoder* GetCommandEncoder() { return m_pCommandEncoder; }
        

    public:
        virtual void Destroy() = 0;

        virtual void SetPipeline(RHIPipelineGraphics* pPipeline) = 0;
        virtual void SetBindGroup(uint8 nLayoutIndex, RHIBindGroup* pBindGroup) = 0;
        virtual void SetIndexBuffer(RHIBufferView* pBufferView) = 0;
        virtual void SetVertexBuffer(uint32 nSlot, RHIBufferView* pBufferView) = 0;
        virtual void Draw(uint32 nVertexCount, uint32 nInstanceCount, uint32 nFirstVertex, uint32 nFirstInstance) = 0;
        virtual void DrawIndexed(uint32 nIndexCount, uint32 nInstanceCount, uint32 nFirstIndex, uint32 nBaseVertex, uint32 nFirstInstance) = 0;
        virtual void SetViewport(float fTopLeftX, float fTopLeftY, float fWidth, float fHeight, float fMinDepth, float fMaxDepth) = 0;
        virtual void SetScissor(uint32 nLeft, uint32 nTop, uint32 nRight, uint32 nBottom) = 0;
        virtual void SetPrimitiveTopology(RHIPrimitiveTopologyType ePrimitiveTopology) = 0;
        virtual void SetBlendConstant(const float* pConstants) = 0;
        virtual void SetStencilReference(uint32 nReference) = 0;
        virtual void EndPass() = 0;

    };

    //////////////////////// RHICommandEncoder ///////////////////////////////////
    class rhiExport RHICommandEncoder : public FNonCopyable
    {
    public:
        RHICommandEncoder(RHICommandBuffer* pCommandBuffer);
        virtual ~RHICommandEncoder();

    public:
    protected:
        RHICommandBuffer* m_pCommandBuffer;

    public:
        F_FORCEINLINE RHICommandBuffer* GetCommandBuffer() { return m_pCommandBuffer; }


    public:
        virtual void Destroy() = 0;

        virtual void CopyBufferToBuffer(RHIBuffer* pBufferSrc, uint32 nSrcOffset, RHIBuffer* pBufferDst, uint32 nDstOffset, uint32 nSize) = 0;
        virtual void CopyBufferToTexture(RHIBuffer* pBufferSrc, RHITexture* pTextureDst, const RHITextureSubResourceInfo* pSubResourceInfo, const RHIExtent<3>& sExtent) = 0;
        virtual void CopyTextureToBuffer(RHITexture* pTextureSrc, RHIBuffer* pBufferDst, const RHITextureSubResourceInfo* pSubResourceInfo, const RHIExtent<3>& sExtent) = 0;
        virtual void CopyTextureToTexture(RHITexture* pTextureSrc, const RHITextureSubResourceInfo* pSrcSubResourceInfo, RHITexture* pTextureDst, const RHITextureSubResourceInfo* pDstSubResourceInfo, const RHIExtent<3>& sExtent) = 0;
        virtual void ResourceBarrier(const RHIBarrier& barrier) = 0;

        virtual RHIComputePassCommandEncoder* BeginComputePass() = 0;
        virtual RHIGraphicsPassCommandEncoder* BeginGraphicsPass(const RHIGraphicsPassBeginInfo* pBeginInfo) = 0;
        virtual void SwapChainSync(RHISwapChain* pSwapChain) = 0;
        virtual void End() = 0;
        
    };

}; //LostPeterRHI

#endif