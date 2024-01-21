/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-11-13
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _RENDER_TARGET_LISTENER_H_
#define _RENDER_TARGET_LISTENER_H_

#include "PreDefine.h"

namespace LostPeterEngine
{
    class engineExport RenderTargetListener : public FNonCopyable
    {
    public:
        RenderTargetListener();
        virtual ~RenderTargetListener();

    public:
        virtual void PreRenderTargetUpdate(const RenderTargetEvent& evt) { }
		virtual void PostRenderTargetUpdate(const RenderTargetEvent& evt) { }

		virtual void PreViewportUpdate(const RenderTargetViewportEvent& evt) { }
		virtual void PostViewportUpdate(const RenderTargetViewportEvent& evt) { }

		virtual void ViewportAdded(const RenderTargetViewportEvent& evt) { }
        virtual void ViewportResized(const RenderTargetViewportEvent& evt) { }
		virtual void ViewportRemoved(const RenderTargetViewportEvent& evt) { }
    };

}; //LostPeterEngine

#endif