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

#include "../include/RenderQueue.h"
#include "../include/Renderable.h"
#include "../include/RenderPass.h"
#include "../include/RenderPassManager.h"

namespace LostPeterEngine
{
    RenderQueue::RenderQueue(const String& nameRenderQueue)
        : Base(nameRenderQueue)
        , m_pSceneManager(nullptr)
    {

    }

    RenderQueue::~RenderQueue()
    {

    }

    void RenderQueue::Destroy()
    {

    }

    bool RenderQueue::Init()
    {
        
        return true;
    }

    bool RenderQueue::AddRenderable(Renderable* pRenderable)
    {
        F_Assert(pRenderable && "RenderQueue::AddRenderable")

        FRenderPassType eRenderPass = pRenderable->GetRenderPassType();
		RenderPass* pRenderPass = RenderPassManager::GetSingleton().GetRenderPassByType(eRenderPass);
		if (pRenderPass)
			pRenderPass->AddQueue(pRenderable);
		else
			return false;
		return true;
    }

}; //LostPeterEngine