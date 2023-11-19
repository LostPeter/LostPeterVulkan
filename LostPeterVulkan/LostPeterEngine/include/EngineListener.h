/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-11-18
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _ENGINE_LISTENER_H_
#define _ENGINE_LISTENER_H_

#include "PreDefine.h"

namespace LostPeter
{
    class utilExport EngineListener
    {
    public:
        EngineListener();
        virtual ~EngineListener();

    public:
        struct LessCompare
		{
			bool operator() (const EngineListener* a, const EngineListener* b) const
			{
				return (a->GetOrder() < b->GetOrder());
			}
		};

    protected:
        int32 m_nOrder;

    public:
        LP_FORCEINLINE int32 GetOrder() const { return m_nOrder; }
		LP_FORCEINLINE void SetOrder(int32 nOrder) { m_nOrder = nOrder; }

    public:
        virtual bool RenderFrameStarted(const RenderFrameEvent& event) { return true; }
		virtual bool RenderFrameRenderingQueued(const RenderFrameEvent& event) { return true; }
		virtual bool RenderFrameEnded(const RenderFrameEvent& event) { return true; }
    };
    typedef std::vector<EngineListener*> EngineListenerPtrVector;
	typedef std::multiset<EngineListener*, EngineListener::LessCompare> EngineListenerPtrMultiSet;

}; //LostPeter

#endif