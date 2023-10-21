/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
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

namespace LostPeter
{
    class utilExport RenderPipelineManager : public FSingleton<RenderPipelineManager>
                                           , public Base
    {
    public:
        RenderPipelineManager();
        virtual ~RenderPipelineManager();

    public:
    protected:
        RenderPipelinePtrVector m_aRenderPipeline;
        RenderPipelinePtrMap m_mapRenderPipeline;

    public:
        LP_FORCEINLINE const RenderPipelinePtrVector& GetRenderPipelinePtrVector() const { return m_aRenderPipeline; }
        LP_FORCEINLINE RenderPipelinePtrVector& GetRenderPipelinePtrVector() { return m_aRenderPipeline; }
        LP_FORCEINLINE const RenderPipelinePtrMap& GetRenderPipelinePtrMap() const { return m_mapRenderPipeline; }
        LP_FORCEINLINE RenderPipelinePtrMap& GetRenderPipelinePtrMap() { return m_mapRenderPipeline; }

    public:
        static RenderPipelineManager& GetSingleton();
		static RenderPipelineManager* GetSingletonPtr();


    };

}; //LostPeter

#endif