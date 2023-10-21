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

#ifndef _VIEWPORT_H_
#define _VIEWPORT_H_

#include "Base.h"

namespace LostPeter
{
    class utilExport Viewport : public Base
							  , public FNonCopyable
    {
    public:
        Viewport(const String& nameViewport);
        virtual ~Viewport();

    public:
        
    };

}; //LostPeter

#endif