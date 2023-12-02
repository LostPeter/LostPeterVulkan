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

#ifndef _OBJECT_VISIBLE_BOUNDS_INFO_H_
#define _OBJECT_VISIBLE_BOUNDS_INFO_H_

#include "PreDefine.h"

namespace LostPeterEngine
{
    class engineExport ObjectVisibleBoundsInfo
    {
    public:
        ObjectVisibleBoundsInfo();
        ~ObjectVisibleBoundsInfo();

    public:
        FAABB m_AABB;
		float m_fMinDisToCamera;
		float m_fMaxDisToCamera;

    public:
        void Reset();
		void Merge(const FAABB& aabbBounds, const FSphere& sphereBounds, const ObjectCamera* pCamera);
    };

}; //LostPeterEngine

#endif