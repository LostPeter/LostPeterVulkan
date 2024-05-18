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

#include "../include/RHIDummyCommandEncoder.h"
#include "../include/RHIDummyCommandBuffer.h"

namespace LostPeterPluginRHIDummy
{
    //////////////////////// RHIDummyComputePassCommandEncoder ////////////////////////
    RHIDummyComputePassCommandEncoder::RHIDummyComputePassCommandEncoder(RHIDummyCommandEncoder* pDummyCommandEncoder)
        : RHIComputePassCommandEncoder(pDummyCommandEncoder)
        , m_pDummyCommandEncoder(pDummyCommandEncoder)
    {
        F_Assert(m_pDummyCommandEncoder && "RHIDummyComputePassCommandEncoder::RHIDummyComputePassCommandEncoder")
    }

    RHIDummyComputePassCommandEncoder::~RHIDummyComputePassCommandEncoder()
    {
        Destroy();
    }

    void RHIDummyComputePassCommandEncoder::Destroy()
    {

    }

    void RHIDummyComputePassCommandEncoder::SetPipeline(RHIPipelineCompute* pPipeline)
    {

    }

    void RHIDummyComputePassCommandEncoder::SetBindGroup(uint8 nLayoutIndex, RHIBindGroup* pBindGroup)
    {

    }

    void RHIDummyComputePassCommandEncoder::Dispatch(uint32 nGroupCountX, uint32 nGroupCountY, uint32 nGroupCountZ)
    {

    }

    void RHIDummyComputePassCommandEncoder::EndPass()
    {

    }


    //////////////////////// RHIDummyGraphicsPassCommandEncoder ///////////////////////
    RHIDummyGraphicsPassCommandEncoder::RHIDummyGraphicsPassCommandEncoder(RHIDummyCommandEncoder* pDummyCommandEncoder)
        : RHIGraphicsPassCommandEncoder(pDummyCommandEncoder)
        , m_pDummyCommandEncoder(pDummyCommandEncoder)
    {
        F_Assert(m_pDummyCommandEncoder && "RHIDummyGraphicsPassCommandEncoder::RHIDummyGraphicsPassCommandEncoder")
    }

    RHIDummyGraphicsPassCommandEncoder::~RHIDummyGraphicsPassCommandEncoder()
    {
        Destroy();
    }

    void RHIDummyGraphicsPassCommandEncoder::Destroy()
    {

    }

    void RHIDummyGraphicsPassCommandEncoder::SetPipeline(RHIPipelineGraphics* pPipeline)
    {

    }

    void RHIDummyGraphicsPassCommandEncoder::SetBindGroup(uint8 nLayoutIndex, RHIBindGroup* pBindGroup)
    {

    }

    void RHIDummyGraphicsPassCommandEncoder::SetIndexBuffer(RHIBufferView* pBufferView)
    {

    }

    void RHIDummyGraphicsPassCommandEncoder::SetVertexBuffer(uint32 nSlot, RHIBufferView* pBufferView)
    {

    }

    void RHIDummyGraphicsPassCommandEncoder::Draw(uint32 nVertexCount, uint32 nInstanceCount, uint32 nFirstVertex, uint32 nFirstInstance)
    {

    }

    void RHIDummyGraphicsPassCommandEncoder::DrawIndexed(uint32 nIndexCount, uint32 nInstanceCount, uint32 nFirstIndex, uint32 nBaseVertex, uint32 nFirstInstance)
    {

    }

    void RHIDummyGraphicsPassCommandEncoder::SetViewport(float fTopLeftX, float fTopLeftY, float fWidth, float fHeight, float fMinDepth, float fMaxDepth)
    {

    }

    void RHIDummyGraphicsPassCommandEncoder::SetScissor(uint32 nLeft, uint32 nTop, uint32 nRight, uint32 nBottom)
    {

    }

    void RHIDummyGraphicsPassCommandEncoder::SetPrimitiveTopology(RHIPrimitiveTopologyType ePrimitiveTopology)
    {

    }

    void RHIDummyGraphicsPassCommandEncoder::SetBlendConstant(const float* pConstants)
    {

    }

    void RHIDummyGraphicsPassCommandEncoder::SetStencilReference(uint32 nReference)
    {

    }

    void RHIDummyGraphicsPassCommandEncoder::EndPass()
    {

    }


    //////////////////////// RHIDummyCommandEncoder ///////////////////////////////////
    RHIDummyCommandEncoder::RHIDummyCommandEncoder(RHIDummyCommandBuffer* pDummyCommandBuffer)
        : RHICommandEncoder(pDummyCommandBuffer)
        , m_pDummyCommandBuffer(pDummyCommandBuffer)
    {
        F_Assert(m_pDummyCommandBuffer && "RHIDummyCommandEncoder::RHIDummyCommandEncoder")
    }   

    RHIDummyCommandEncoder::~RHIDummyCommandEncoder()
    {
        Destroy();
    }

    void RHIDummyCommandEncoder::Destroy()
    {

    }

    void RHIDummyCommandEncoder::CopyBufferToBuffer(RHIBuffer* pBufferSrc, uint32 nSrcOffset, RHIBuffer* pBufferDst, uint32 nDstOffset, uint32 nSize)
    {

    }

    void RHIDummyCommandEncoder::CopyBufferToTexture(RHIBuffer* pBufferSrc, RHITexture* pTextureDst, const RHITextureSubResourceInfo* pSubResourceInfo, const RHIExtent<3>& sExtent)
    {

    }

    void RHIDummyCommandEncoder::CopyTextureToBuffer(RHITexture* pTextureSrc, RHIBuffer* pBufferDst, const RHITextureSubResourceInfo* pSubResourceInfo, const RHIExtent<3>& sExtent)
    {

    }

    void RHIDummyCommandEncoder::CopyTextureToTexture(RHITexture* pTextureSrc, const RHITextureSubResourceInfo* pSrcSubResourceInfo, RHITexture* pTextureDst, const RHITextureSubResourceInfo* pDstSubResourceInfo, const RHIExtent<3>& sExtent)
    {

    }

    void RHIDummyCommandEncoder::ResourceBarrier(const RHIBarrier& barrier)
    {

    }

    RHIDummyComputePassCommandEncoder* RHIDummyCommandEncoder::BeginComputePass()
    {
        return new RHIDummyComputePassCommandEncoder(this);
    }

    RHIDummyGraphicsPassCommandEncoder* RHIDummyCommandEncoder::BeginGraphicsPass(const RHIGraphicsPassBeginInfo* pBeginInfo)
    {
        return new RHIDummyGraphicsPassCommandEncoder(this);
    }

    void RHIDummyCommandEncoder::SwapChainSync(RHISwapChain* pSwapChain)
    {

    }

    void RHIDummyCommandEncoder::End()
    {

    }

}; //LostPeterPluginRHIDummy