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

#ifndef _RENDER_PIPELINE_MANAGER_H_
#define _RENDER_PIPELINE_MANAGER_H_

#include "Base.h"

namespace LostPeterEngine
{
    class engineExport RenderPipelineManager : public FSingleton<RenderPipelineManager>
                                           , public Base
    {
    public:
        RenderPipelineManager();
        virtual ~RenderPipelineManager();

    public:
    protected:
        RenderPassManager* m_pRenderPassManager;
		RenderPipeline* m_pPipeLineCurrent;

		RenderPipelinePtrMap m_mapRenderPipeLine;

    public:
        static RenderPipelineManager& GetSingleton();
		static RenderPipelineManager* GetSingletonPtr();

    public:
		E_FORCEINLINE RenderPipeline* GetPipeLineCurrent() const { return m_pPipeLineCurrent; }

	public:
        void Destroy();
		bool Init(const String& strPipeLineName);
		
		
		bool RenderOneFrame(RenderFrameEvent& event);

		bool ChangePipeLineTo(ERenderPipelineType eRenderPipeline);
    };

}; //LostPeterEngine

#endif