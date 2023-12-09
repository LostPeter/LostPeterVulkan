/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-22
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _RENDER_PIPELINE_H_
#define _RENDER_PIPELINE_H_

#include "Base.h"

namespace LostPeterEngine
{
    class engineExport RenderPipeline : public Base
                                      , public FNonCopyable
    {
    public:
        RenderPipeline(const String& nameRenderPipeline, RenderPipelineManager* pRenderPipelineManager, ERenderPipelineType eRenderPipelineType);
        virtual ~RenderPipeline();

    public:
    protected:
        RenderPipelineManager* m_pRenderPipelineManager;
		ERenderPipelineType m_eRenderPipelineType;

    public:
		F_FORCEINLINE ERenderPipelineType GetRenderPipePipeLineType() const { return m_eRenderPipelineType; }

    public:
        virtual bool Init() = 0;
        virtual bool RenderOneFrame(RenderFrameEvent& event);

    };

}; //LostPeterEngine

#endif