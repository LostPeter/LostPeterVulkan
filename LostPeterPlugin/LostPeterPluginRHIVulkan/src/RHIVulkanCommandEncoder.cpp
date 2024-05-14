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

#include "../include/RHIVulkanCommandEncoder.h"

namespace LostPeterPluginRHIVulkan
{
    //////////////////////// RHIVulkanComputePassCommandEncoder ////////////////////////
    RHIVulkanComputePassCommandEncoder::RHIVulkanComputePassCommandEncoder(const RHIVulkanCommandBuffer& commandBuffer)
    {

    }

    RHIVulkanComputePassCommandEncoder::~RHIVulkanComputePassCommandEncoder()
    {
        Destroy();
    }

    void RHIVulkanComputePassCommandEncoder::Destroy()
    {

    }

    void RHIVulkanComputePassCommandEncoder::SetPipeline(RHIPipelineCompute* pPipeline)
    {

    }

    void RHIVulkanComputePassCommandEncoder::SetBindGroup(uint8 nLayoutIndex, RHIBindGroup* pBindGroup)
    {

    }

    void RHIVulkanComputePassCommandEncoder::Dispatch(uint32 nGroupCountX, uint32 nGroupCountY, uint32 nGroupCountZ)
    {

    }

    void RHIVulkanComputePassCommandEncoder::EndPass()
    {

    }


    //////////////////////// RHIVulkanGraphicsPassCommandEncoder ///////////////////////
    RHIVulkanGraphicsPassCommandEncoder::RHIVulkanGraphicsPassCommandEncoder(const RHIVulkanCommandBuffer& commandBuffer)
    {

    }

    RHIVulkanGraphicsPassCommandEncoder::~RHIVulkanGraphicsPassCommandEncoder()
    {
        Destroy();
    }

    void RHIVulkanGraphicsPassCommandEncoder::Destroy()
    {

    }

    void RHIVulkanGraphicsPassCommandEncoder::SetPipeline(RHIPipelineGraphics* pPipeline)
    {

    }

    void RHIVulkanGraphicsPassCommandEncoder::SetBindGroup(uint8 nLayoutIndex, RHIBindGroup* pBindGroup)
    {

    }

    void RHIVulkanGraphicsPassCommandEncoder::SetIndexBuffer(RHIBufferView* pBufferView)
    {

    }

    void RHIVulkanGraphicsPassCommandEncoder::SetVertexBuffer(uint32 nSlot, RHIBufferView* pBufferView)
    {

    }

    void RHIVulkanGraphicsPassCommandEncoder::Draw(uint32 nVertexCount, uint32 nInstanceCount, uint32 nFirstVertex, uint32 nFirstInstance)
    {

    }

    void RHIVulkanGraphicsPassCommandEncoder::DrawIndexed(uint32 nIndexCount, uint32 nInstanceCount, uint32 nFirstIndex, uint32 nBaseVertex, uint32 nFirstInstance)
    {

    }

    void RHIVulkanGraphicsPassCommandEncoder::SetViewport(float fTopLeftX, float fTopLeftY, float fWidth, float fHeight, float fMinDepth, float fMaxDepth)
    {

    }

    void RHIVulkanGraphicsPassCommandEncoder::SetScissor(uint32 nLeft, uint32 nTop, uint32 nRight, uint32 nBottom)
    {

    }

    void RHIVulkanGraphicsPassCommandEncoder::SetPrimitiveTopology(RHIPrimitiveTopologyType ePrimitiveTopology)
    {

    }

    void RHIVulkanGraphicsPassCommandEncoder::SetBlendConstant(const float* pConstants)
    {

    }

    void RHIVulkanGraphicsPassCommandEncoder::SetStencilReference(uint32 nReference)
    {

    }

    void RHIVulkanGraphicsPassCommandEncoder::EndPass()
    {

    }


    //////////////////////// RHIVulkanCommandEncoder ///////////////////////////////////
    RHIVulkanCommandEncoder::RHIVulkanCommandEncoder(const RHIVulkanCommandBuffer& commandBuffer)
        : m_commandBuffer(commandBuffer)
    {

    }

    RHIVulkanCommandEncoder::~RHIVulkanCommandEncoder()
    {
        Destroy();
    }

    void RHIVulkanCommandEncoder::Destroy()
    {

    }

    void RHIVulkanCommandEncoder::CopyBufferToBuffer(RHIBuffer* pBufferSrc, uint32 nSrcOffset, RHIBuffer* pBufferDst, uint32 nDstOffset, uint32 nSize)
    {

    }

    void RHIVulkanCommandEncoder::CopyBufferToTexture(RHIBuffer* pBufferSrc, RHITexture* pTextureDst, const RHITextureSubResourceInfo* pSubResourceInfo, const RHIExtent<3>& sExtent)
    {

    }

    void RHIVulkanCommandEncoder::CopyTextureToBuffer(RHITexture* pTextureSrc, RHIBuffer* pBufferDst, const RHITextureSubResourceInfo* pSubResourceInfo, const RHIExtent<3>& sExtent)
    {

    }

    void RHIVulkanCommandEncoder::CopyTextureToTexture(RHITexture* pTextureSrc, const RHITextureSubResourceInfo* pSrcSubResourceInfo, RHITexture* pTextureDst, const RHITextureSubResourceInfo* pDstSubResourceInfo, const RHIExtent<3>& sExtent)
    {

    }

    void RHIVulkanCommandEncoder::ResourceBarrier(const RHIBarrier& barrier)
    {

    }

    RHIVulkanComputePassCommandEncoder* RHIVulkanCommandEncoder::BeginComputePass()
    {
        return new RHIVulkanComputePassCommandEncoder(m_commandBuffer);
    }

    RHIVulkanGraphicsPassCommandEncoder* RHIVulkanCommandEncoder::BeginGraphicsPass(const RHIGraphicsPassBeginInfo* pBeginInfo)
    {
        return new RHIVulkanGraphicsPassCommandEncoder(m_commandBuffer);
    }

    void RHIVulkanCommandEncoder::SwapChainSync(RHISwapChain* pSwapChain)
    {

    }

    void RHIVulkanCommandEncoder::End()
    {

    }

}; //LostPeterPluginRHIVulkan