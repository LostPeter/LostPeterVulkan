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

#ifndef _RENDER_PASS_GRASS_H_
#define _RENDER_PASS_GRASS_H_

#include "RenderPassQueue.h"

namespace LostPeterEngine
{
    class engineExport RenderPassGrass : public RenderPassQueue
    {
        friend class RenderPassManager;

    private:
        RenderPassGrass();
    public:
        virtual ~RenderPassGrass();

    public:
        

    };

}; //LostPeterEngine

#endif