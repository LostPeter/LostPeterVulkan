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

#ifndef _RENDER_PASS_FORWARD_LIT_H_
#define _RENDER_PASS_FORWARD_LIT_H_

#include "RenderPassQueue.h"

namespace LostPeterEngine
{
    class engineExport RenderPassForwardLit : public RenderPassQueue
    {
        friend class RenderPassManager;

    private:
        RenderPassForwardLit();
    public:
        virtual ~RenderPassForwardLit();

    public:
        

    };

}; //LostPeterEngine

#endif