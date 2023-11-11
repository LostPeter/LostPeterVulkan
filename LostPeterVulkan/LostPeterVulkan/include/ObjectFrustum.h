/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-11-11
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _OBJECT_FRUSTUM_H_
#define _OBJECT_FRUSTUM_H_

#include "Object.h"

namespace LostPeter
{
    class utilExport ObjectFrustum : public Object
    {
    public:
        ObjectFrustum(const String& nameObject, Scene* pScene);
        virtual ~ObjectFrustum();

    public:
    protected:
        mutable FAABB m_aabb;
        

    public:


    ////Movable
    public:
        virtual const FAABB& GetBoundingAABB() const;

    };

}; //LostPeter

#endif