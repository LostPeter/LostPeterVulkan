/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-21
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _RENDER_PASS_H_
#define _RENDER_PASS_H_

#include "Base.h"

namespace LostPeterEngine
{
    class engineExport RenderPass : public Base
                                  , public FNonCopyable
    {
    public:
        RenderPass(const String& nameRenderPass);
        virtual ~RenderPass();

    public:
        void Destroy();
        virtual bool Init();

    

    };

}; //LostPeterEngine

#endif