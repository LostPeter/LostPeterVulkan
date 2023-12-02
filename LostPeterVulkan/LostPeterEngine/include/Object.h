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

#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "Movable.h"

namespace LostPeterEngine
{
    class engineExport Object : public Movable
    {
    public:
        Object(const String& nameObject, Scene* pScene);
        virtual ~Object();

    public:
    protected:
        EObjectType m_typeObject;



    public:
        E_FORCEINLINE EObjectType GetObjectType() const { return this->m_typeObject; }

    public:


    };

}; //LostPeterEngine

#endif