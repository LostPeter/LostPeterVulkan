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

#include "../include/RHIGraphicsState.h"
#include "../include/RHIDevice.h"
#include "../include/RHIPipelineGraphics.h"
#include "../include/RHIFrameBuffer.h"

namespace LostPeterRHI
{
    RHIGraphicsState::RHIGraphicsState(RHIDevice* pDevice)
        : RHIObject(pDevice)

    {
        Reset();
    }

    RHIGraphicsState::~RHIGraphicsState()
    {
        
    }
    
    void RHIGraphicsState::Reset()
    {
        m_nSubpassIndex = 0;
        m_pPipelineGraphics = nullptr;
        m_pFrameBuffer = nullptr;
        m_bIsDirty = false;
    }

    void RHIGraphicsState::SetStateViewport(const RHIViewportState& stateViewport)
    {
        m_stateViewport = stateViewport;
    }

    void RHIGraphicsState::SetStateVertex(const RHIVertexState& stateVertex)
    {
        m_stateVertex = stateVertex;
    }

    void RHIGraphicsState::SetStatePrimitive(const RHIPrimitiveState& statePrimitive)
    {
        m_statePrimitive = statePrimitive;
    }

    void RHIGraphicsState::SetStateTessellation(const RHITessellationState& stateTessellation)
    {
        m_stateTessellation = stateTessellation;
    }

    void RHIGraphicsState::SetStateDepthStencil(const RHIDepthStencilState& stateDepthStencil)
    {
        m_stateDepthStencil = stateDepthStencil;
    }   

    void RHIGraphicsState::SetStateMultiSample(const RHIMultiSampleState& stateMultiSample)
    {
        m_stateMultiSample = stateMultiSample;
    }   

    void RHIGraphicsState::SetStateFragment(const RHIFragmentState& stateFragment)
    {
        m_stateFragment = stateFragment;
    }

    void RHIGraphicsState::SetSubpassIndex(uint32 nIndex)
    {
        if (m_nSubpassIndex != nIndex)
        {
            m_nSubpassIndex = nIndex;
            EnableIsDirty(true);
        }
    }

    void RHIGraphicsState::SetPipelineGraphics(RHIPipelineGraphics* pPipelineGraphics)
    {
        if (m_pPipelineGraphics != pPipelineGraphics)
        {
            m_pPipelineGraphics = pPipelineGraphics;
            EnableIsDirty(true);
        }
    }

    void RHIGraphicsState::SetFramebuffer(RHIFrameBuffer* pFramebuffer)
    {
        if (m_pFrameBuffer != pFramebuffer)
        {
            m_pFrameBuffer = pFramebuffer;
            EnableIsDirty(true);
        }
    }

}; //LostPeterRHI