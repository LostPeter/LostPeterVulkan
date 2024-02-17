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

#ifndef _RENDER_PASS_LISTENER_H_
#define _RENDER_PASS_LISTENER_H_

#include "PreDefine.h"

namespace LostPeterEngine
{
    class engineExport RenderPassListener 
    {
    public:
        RenderPassListener()
        {

        }

        virtual ~RenderPassListener()
        {

        }

    public:
        virtual void PreRenderPassRender(Viewport* pViewport) 
        {

        }

		virtual void PostRenderPassRender(Viewport* pViewport) 
        { 
            
        }
    };

}; //LostPeterEngine

#endif