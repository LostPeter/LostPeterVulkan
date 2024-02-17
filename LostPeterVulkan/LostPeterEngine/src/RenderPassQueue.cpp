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

    }

    void RenderPassQueue::AddQueue(Renderable* pRenderable)
    {

    }

    void RenderPassQueue::ClearQueue()
    {

    }

	void RenderPassQueue::Render(Renderer* pRenderer)
    {

    }

}; //LostPeterEngine