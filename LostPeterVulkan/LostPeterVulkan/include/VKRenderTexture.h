/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-11-15
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VK_RENDER_TEXTURE_H_
#define _VK_RENDER_TEXTURE_H_

#include "RenderTexture.h"

namespace LostPeter
{
    class utilExport VKRenderTexture : public RenderTexture
    {
    public:
        VKRenderTexture(const String& nameRenderTexture);
        virtual ~VKRenderTexture();

    public:

    };

}; //LostPeter

#endif