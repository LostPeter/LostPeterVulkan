/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-11-11
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/ObjectFrustum.h"
#include "../include/SceneManager.h"
#include "../include/Scene.h"

namespace LostPeter
{
    ObjectFrustum::ObjectFrustum(const String& nameObject, SceneManager* pSceneManager)
        : Object(nameObject, pSceneManager->GetScene())
    {
        
    }

    ObjectFrustum::~ObjectFrustum()
    {

    }

    const FAABB& ObjectFrustum::GetBoundingAABB() const
    {
        return m_aabb;
    }

}; //LostPeter