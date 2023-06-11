/****************************************************************************
* LostPeterFoundation - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-06-11
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/FPlaneBoundedVolume.h"
#include "../include/FMath.h"
#include "../include/FRay.h"
#include "../include/FAABB.h"
#include "../include/FSphere.h"

namespace LostPeterFoundation
{
    int FPlaneBoundedVolume::GetPlaneCount() const
    {
        return (int)m_aPlanes.size();
    }
    const FPlane& FPlaneBoundedVolume::GetPlaneRef(int index) const
    {
        return m_aPlanes[index];
    }
    FPlane* FPlaneBoundedVolume::GetPlanePtr(int index)
    {
        return &m_aPlanes[index];
    }
    FPlane* FPlaneBoundedVolume::GetPlanePtr()
    {
        return &m_aPlanes[0];
    }

    bool FPlaneBoundedVolume::Intersects(const FAABB& aabb) const
    {
        if (!aabb.IsValid()) 
            return false;

        FVector3 center; aabb.GetCenter(center);
        FVector3 halfSize = aabb.GetHalfSize();
        for (FPlaneVector::const_iterator it = m_aPlanes.begin(); 
             it != m_aPlanes.end(); ++it)
        {
            const FPlane& plane = *it;

            FPlaneSideType ePlaneSideOutside = plane.GetSide(center, halfSize);
            if (ePlaneSideOutside == m_ePlaneSide)
            {
                return false;
            }
        }
        return true;
    }

    bool FPlaneBoundedVolume::Intersects(const FSphere& sphere) const
    {
        for (FPlaneVector::const_iterator it = m_aPlanes.begin(); 
             it != m_aPlanes.end(); ++it)
        {
            const FPlane& plane = *it;
            
            float d = plane.GetDistance(sphere.GetCenter());
            if (m_ePlaneSide == F_PlaneSide_Negative)
                d = -d;

            if ((d - sphere.GetRadius()) > 0.0f)
                return false;
        }
        return true;
    }

    std::pair<bool, float> FPlaneBoundedVolume::Intersects(const FRay& ray)
    {
        return FMath::Intersects(ray, m_aPlanes, m_ePlaneSide == F_PlaneSide_Negative);
    }

    bool FPlaneBoundedVolume::operator ==(const FPlaneBoundedVolume& rhs) const
    {
        if (m_ePlaneSide != rhs.m_ePlaneSide)
            return false;
        if (m_aPlanes.size() != rhs.m_aPlanes.size())
            return false;

        int count = GetPlaneCount();
        for (int i = 0; i < count; i++)
        {
            const FPlane& planeSrc = GetPlaneRef(i);
            const FPlane& planeCmp = rhs.GetPlaneRef(i);

            if (planeSrc != planeCmp)
                return false;
        }
        return true;
    }

    bool FPlaneBoundedVolume::operator !=(const FPlaneBoundedVolume& rhs) const
    {
        return !(*this == rhs);
    }
    
}; //LostPeterFoundation