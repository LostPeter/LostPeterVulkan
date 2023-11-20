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

#ifndef _RENDER_TEXTURE_H_
#define _RENDER_TEXTURE_H_

#include "RenderTarget.h"

namespace LostPeterEngine
{
    class utilExport RenderTexture : public RenderTarget
    {
    public:
        RenderTexture(const String& nameRenderTexture);
        virtual ~RenderTexture();

    public:
        

    };

}; //LostPeterEngine

#endif