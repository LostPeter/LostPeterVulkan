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

#include "../include/ObjectVisibleBoundsInfo.h"
#include "../include/VulkanWindow.h"
#include "../include/ObjectCamera.h"

namespace LostPeter
{
    ObjectVisibleBoundsInfo::ObjectVisibleBoundsInfo()
    {
        Reset();
    }

    ObjectVisibleBoundsInfo::~ObjectVisibleBoundsInfo()
    {

    }

    void ObjectVisibleBoundsInfo::Reset()
    {
        m_AABB.SetNull();
		m_fMinDisToCamera = F_C_MAX_FLOAT;
		m_fMaxDisToCamera = 0;
    }

	void ObjectVisibleBoundsInfo::Merge(const FAABB& aabbBounds, const FSphere& sphereBounds, const ObjectCamera* pCamera)
    {
        m_AABB.Merge(aabbBounds);
		float fCamDistToCenter = FMath::Length(pCamera->GetDerivedPosition() - sphereBounds.GetCenter());
		m_fMinDisToCamera = FMath::Min(m_fMinDisToCamera, FMath::Max((float)0, fCamDistToCenter - sphereBounds.GetRadius()));
		m_fMaxDisToCamera = FMath::Max(m_fMaxDisToCamera, fCamDistToCenter + sphereBounds.GetRadius());
    }

}; //LostPeter