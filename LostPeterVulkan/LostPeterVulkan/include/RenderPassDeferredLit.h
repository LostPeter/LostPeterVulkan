/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-22
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _RENDER_PASS_DEFERRED_LIT_H_
#define _RENDER_PASS_DEFERRED_LIT_H_

#include "RenderPass.h"

namespace LostPeter
{
    class utilExport RenderPassDeferredLit : public RenderPass
    {
    public:
        RenderPassDeferredLit(const String& nameRenderPass);
        virtual ~RenderPassDeferredLit();

    public:
        

    };

}; //LostPeter

#endif