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

#ifndef _SCENE_H_
#define _SCENE_H_

#include "Base.h"

namespace LostPeter
{
    class utilExport Scene : public Base
    {
    public:
        Scene(const String& nameScene);
        virtual ~Scene();

    public:
        ObjectPtrVector m_aObjects;
        ObjectPtrMap m_mapObjects;


    public:
        

    };

}; //LostPeter

#endif