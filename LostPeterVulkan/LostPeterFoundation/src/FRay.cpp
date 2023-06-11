/****************************************************************************
* LostPeterFoundation - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-06-01
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/FRay.h"
#include "../include/FMath.h"
#include "../include/FAABB.h"
#include "../include/FPlane.h"
#include "../include/FPlaneBoundedVolume.h"
#include "../include/FSphere.h"

namespace LostPeterFoundation
{
    std::pair<bool, float> FRay::Intersects(const FPlane& plane) const
    {
        return FMath::Intersects(*this, plane);
    }
    std::pair<bool, float> FRay::Intersects(const FPlaneBoundedVolume& planeBoundedVolume) const
    {
        return FMath::Intersects(*this, planeBoundedVolume.m_aPlanes, planeBoundedVolume.m_ePlaneSide == F_PlaneSide_Positive);
    }
    std::pair<bool, float> FRay::Intersects(const FSphere& sphere) const
    {
        return FMath::Intersects(*this, sphere);
    }
    std::pair<bool, float> FRay::Intersects(const FAABB& aabb) const
    {
        return FMath::Intersects(*this, aabb);
    }

    bool FRay::operator ==(const FRay& ray) const
    {
        return FMath::IsEqual(m_vOrigin, ray.m_vOrigin) &&
            FMath::IsEqual(m_vDirection, ray.m_vDirection);
    }

    bool FRay::operator !=(const FRay& ray) const
    {
        return !(*this == ray);
    }

    FVector3 FRay::operator *(float t) const
    {
        return GetPoint(t);
    }
    
}; //LostPeterFoundation