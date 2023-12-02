/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-11-16
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _RENDER_ENGINE_LISTENER_H_
#define _RENDER_ENGINE_LISTENER_H_

#include "Base.h"

namespace LostPeterEngine
{
    class engineExport RenderEngineListener
    {
    public:
        RenderEngineListener();
        virtual ~RenderEngineListener();

    public:
		struct LessCompare
		{
			bool operator() (const RenderEngineListener* a, const RenderEngineListener* b) const
			{
				return (a->GetOrder() < b->GetOrder());
			}
		};

    public:
    protected:
		int32 m_nOrder;

    public:
		E_FORCEINLINE int32 GetOrder() const { return m_nOrder; }
		E_FORCEINLINE void SetOrder(int32 nOrder) { m_nOrder = nOrder; }

	public:
		virtual	bool PreRenderOneFrame() { return true; }
		virtual	bool PostRenderOneFrame() { return true; }
    };
    typedef std::vector<RenderEngineListener*> RenderEngineListenerPtrVector;
	typedef std::set<RenderEngineListener*, RenderEngineListener::LessCompare> RenderEngineListenerPtrSet;

}; //LostPeterEngine

#endif