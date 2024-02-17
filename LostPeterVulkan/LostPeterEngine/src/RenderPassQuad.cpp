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

#include "../include/RenderPassQuad.h"
#include "../include/Renderable.h"
#include "../include/Renderer.h"

namespace LostPeterEngine
{
    RenderPassQuad::RenderPassQuad(FRenderPassType eRenderPass)
        : RenderPass(eRenderPass, F_RenderPassSort_Quad)
    {

    }
    
    RenderPassQuad::~RenderPassQuad()
    {

    }

    void RenderPassQuad::AddQueue(Renderable* pRenderable)
    {
        
    }
    
    void RenderPassQuad::ClearQueue()
    {

    }

	void RenderPassQuad::Render(Renderer* pRenderer)
    {

    }

}; //LostPeterEngine