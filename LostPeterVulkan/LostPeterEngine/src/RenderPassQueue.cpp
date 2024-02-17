/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-02-17
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/RenderPassQueue.h"
#include "../include/Renderable.h"
#include "../include/Renderer.h"

namespace LostPeterEngine
{
    RenderPassQueue::RenderPassQueue(FRenderPassType eRenderPass)
        : RenderPass(eRenderPass, F_RenderPassSort_Queue)
    {

    }
    
    RenderPassQueue::~RenderPassQueue()
    {
        ClearQueue();
    }

    void RenderPassQueue::AddQueue(Renderable* pRenderable)
    {
        if(m_listRenderable.empty())
			m_listRenderable.push_back(pRenderable);
		else
		{
			RenderablePtrList::iterator it,itEnd;
			itEnd = m_listRenderable.end();
			for (it = m_listRenderable.begin(); it != itEnd; ++it)
			{
				if((*it)->GetRendOrderID() > pRenderable->GetRendOrderID())
					break;
			}
			m_listRenderable.insert(it, pRenderable);
		}
    }

    void RenderPassQueue::ClearQueue()
    {
        m_listRenderable.clear();
    }

	void RenderPassQueue::Render(Renderer* pRenderer)
    {

    }

}; //LostPeterEngine