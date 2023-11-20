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

#ifndef _RENDER_QUEUE_MANAGER_H_
#define _RENDER_QUEUE_MANAGER_H_

#include "Base.h"

namespace LostPeterEngine
{
    class utilExport RenderQueueManager : public FSingleton<RenderQueueManager>
                                        , public Base
    {
    public:
        RenderQueueManager();
        virtual ~RenderQueueManager();

    public:
    protected:
        RenderQueuePtrVector m_aRenderQueue;
        RenderQueuePtrMap m_mapRenderQueue;

    public:
        LP_FORCEINLINE const RenderQueuePtrVector& GetRenderQueuePtrVector() const { return m_aRenderQueue; }
        LP_FORCEINLINE RenderQueuePtrVector& GetRenderQueuePtrVector() { return m_aRenderQueue; }
        LP_FORCEINLINE const RenderQueuePtrMap& GetRenderQueuePtrMap() const { return m_mapRenderQueue; }
        LP_FORCEINLINE RenderQueuePtrMap& GetRenderQueuePtrMap() { return m_mapRenderQueue; }

    public:
        static RenderQueueManager& GetSingleton();
		static RenderQueueManager* GetSingletonPtr();


    };

}; //LostPeterEngine

#endif