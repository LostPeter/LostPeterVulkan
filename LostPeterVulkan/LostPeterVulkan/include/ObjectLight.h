/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-04
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _OBJECT_LIGHT_H_
#define _OBJECT_LIGHT_H_

#include "Object.h"

namespace LostPeter
{
    class utilExport ObjectLight : public Object
    {
    public:
        ObjectLight(const String& nameLight, Scene* pScene);
        virtual ~ObjectLight();

    public:

    public:

    };

}; //LostPeter

#endif