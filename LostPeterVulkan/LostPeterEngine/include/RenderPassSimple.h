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

#ifndef _RENDER_PASS_SIMPLE_H_
#define _RENDER_PASS_SIMPLE_H_

#include "RenderPass.h"

namespace LostPeterEngine
{
    class engineExport RenderPassSimple : public RenderPass
    {
    public:
        RenderPassSimple(FRenderPassType eRenderPass);
        virtual ~RenderPassSimple();

    public:
        

    public:
        virtual void AddQueue(Renderable* pRenderable);
		virtual void ClearQueue();

		virtual void Render(Renderer* pRenderer);
    };

}; //LostPeterEngine

#endif