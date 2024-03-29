/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-04
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _OBJECT_TERRAIN_H_
#define _OBJECT_TERRAIN_H_

#include "Object.h"

namespace LostPeterEngine
{
    class engineExport ObjectTerrain : public Object
    {
    public:
        ObjectTerrain(const String& nameObjectTerrain, Scene* pScene);
        virtual ~ObjectTerrain();

    public:
    protected:
        static const String	ms_strMovableType;	

    public:
    protected:
        mutable FAABB m_aabb;

    public:


    ////Movable
    public:
        virtual const String& GetMovableType() const;
        virtual const FAABB& GetBoundingAABB() const;
		virtual float GetBoundingRadius() const;
    };

}; //LostPeterEngine

#endif