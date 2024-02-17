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
    class engineExport RenderQueueManager : public FSingleton<RenderQueueManager>
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
        F_FORCEINLINE const RenderQueuePtrVector& GetRenderQueuePtrVector() const { return m_aRenderQueue; }
        F_FORCEINLINE RenderQueuePtrVector& GetRenderQueuePtrVector() { return m_aRenderQueue; }
        F_FORCEINLINE const RenderQueuePtrMap& GetRenderQueuePtrMap() const { return m_mapRenderQueue; }
        F_FORCEINLINE RenderQueuePtrMap& GetRenderQueuePtrMap() { return m_mapRenderQueue; }

    public:
        static RenderQueueManager& GetSingleton();
		static RenderQueueManager* GetSingletonPtr();

    public:
        void Destroy();

    public:
        bool HasRenderQueue(const String& strName);
        RenderQueue* GetRenderQueue(const String& strName);
        bool AddRenderQueue(RenderQueue* pRenderQueue);
        RenderQueue* CreateRenderQueue(const String& strName);
        void DeleteRenderQueue(const String& strName);
		void DeleteRenderQueue(RenderQueue* pRenderQueue);
        void DeleteRenderQueueAll();

    };

}; //LostPeterEngine

#endif