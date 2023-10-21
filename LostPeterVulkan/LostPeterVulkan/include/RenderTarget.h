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

#ifndef _RENDER_TARGET_H_
#define _RENDER_TARGET_H_

#include "Base.h"

namespace LostPeter
{
    class utilExport RenderTarget : public Base
                                  , public FNonCopyable
    {
    public:
        RenderTarget(const String& nameRenderTarget);
        virtual ~RenderTarget();

    public:
        

    };

}; //LostPeter

#endif