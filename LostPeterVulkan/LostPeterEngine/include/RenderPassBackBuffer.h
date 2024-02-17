/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-02-16
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _RENDER_PASS_BACK_BUFFER_H_
#define _RENDER_PASS_BACK_BUFFER_H_

#include "RenderPassQuad.h"

namespace LostPeterEngine
{
    class engineExport RenderPassBackBuffer : public RenderPassQuad
    {
        friend class RenderPassManager;

    private:
        RenderPassBackBuffer();
    public:
        virtual ~RenderPassBackBuffer();

    public:
        

    };

}; //LostPeterEngine

#endif