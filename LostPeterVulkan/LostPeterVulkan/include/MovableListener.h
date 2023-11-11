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

#ifndef _MOVABLE_LISTENER_H_
#define _MOVABLE_LISTENER_H_

#include "PreDefine.h"

namespace LostPeter
{
    class utilExport MovableListener
    {
    public:
        MovableListener();
        virtual ~MovableListener();

    public:
        virtual void ObjectDestroyed(Movable* pMovable) { }
        virtual void ObjectAttached(Movable* pMovable) { }
        virtual void ObjectDetached(Movable* pMovable) { }
        virtual void ObjectMoved(Movable* pMovable)	{ }
        virtual bool ObjectRendering(const Movable* pMovable, const ObjectCamera* pCamera) { return true; }
        virtual const ObjectLightPtrVector* ObjectQueryLights(const Movable* pMovable) { return nullptr; }
    }; 

}; //LostPeter

#endif