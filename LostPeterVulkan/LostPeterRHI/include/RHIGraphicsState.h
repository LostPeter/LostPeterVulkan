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

#ifndef _RHI_GRAPHICS_STATE_H_
#define _RHI_GRAPHICS_STATE_H_

#include "RHIObject.h"

namespace LostPeterRHI
{
    class rhiExport RHIGraphicsState : public RHIObject
    {
    public:
        RHIGraphicsState(RHIDevice* pDevice);
        virtual ~RHIGraphicsState();

    public:
    protected:
        RHIViewportState m_stateViewport;
        RHIVertexState m_stateVertex;
        RHIPrimitiveState m_statePrimitive;
        RHITessellationState m_stateTessellation;
        RHIDepthStencilState m_stateDepthStencil;
        RHIMultiSampleState m_stateMultiSample;
        RHIFragmentState m_stateFragment;

        uint32 m_nSubpassIndex;
        RHIPipelineGraphics* m_pPipelineGraphics;
        RHIFrameBuffer* m_pFrameBuffer;

        bool m_bIsDirty;

    public:
        F_FORCEINLINE const RHIViewportState& GetStateViewport() const { return m_stateViewport; }
        F_FORCEINLINE const RHIVertexState& GetStateVertex() const { return m_stateVertex; }
        F_FORCEINLINE const RHIPrimitiveState& GetStatePrimitive() const { return m_statePrimitive; }
        F_FORCEINLINE const RHITessellationState& GetStateTessellation() const { return m_stateTessellation; }
        F_FORCEINLINE const RHIDepthStencilState& GetStateDepthStencil() const { return m_stateDepthStencil; }
        F_FORCEINLINE const RHIMultiSampleState& GetStateMultiSample() const { return m_stateMultiSample; }
        F_FORCEINLINE const RHIFragmentState& GetStateFragment() const { return m_stateFragment; }

        F_FORCEINLINE uint32 GetSubpassIndex() const { return m_nSubpassIndex; }
        F_FORCEINLINE RHIPipelineGraphics* GetPipelineGraphics() const { return m_pPipelineGraphics; }
        F_FORCEINLINE RHIFrameBuffer* GetFrameBuffer() const { return m_pFrameBuffer; }
        F_FORCEINLINE bool IsDirty() const { return m_bIsDirty; }

        F_FORCEINLINE void EnableIsDirty(bool bIsDirty) { m_bIsDirty = bIsDirty; }

    public:
        virtual RHIGraphicsStateHash GetHash() const = 0;

    public:
        virtual void Reset();

        virtual void SetStateViewport(const RHIViewportState& stateViewport); 
        virtual void SetStateVertex(const RHIVertexState& stateVertex); 
        virtual void SetStatePrimitive(const RHIPrimitiveState& statePrimitive); 
        virtual void SetStateTessellation(const RHITessellationState& stateTessellation); 
        virtual void SetStateDepthStencil(const RHIDepthStencilState& stateDepthStencil); 
        virtual void SetStateMultiSample(const RHIMultiSampleState& stateMultiSample); 
        virtual void SetStateFragment(const RHIFragmentState& stateFragment); 

        virtual void SetSubpassIndex(uint32 nIndex);
        virtual void SetPipelineGraphics(RHIPipelineGraphics* pPipelineGraphics);
        virtual void SetFramebuffer(RHIFrameBuffer* pFramebuffer);
        
    };
    
}; //LostPeterRHI

#endif