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

#include "../include/FSphere.h"
#include "../include/FMath.h"
#include "../include/FAABB.h"
#include "../include/FPlane.h"
#include "../include/FFrustum.h"

namespace LostPeterFoundation
{
    bool FSphere::Intersects(const FVector3& point) const
    {
        return (FMath::Length2(point - m_vCenter) <= m_fRadius * m_fRadius);
    }

    bool FSphere::Intersects(const FSphere& sphere) const
    {
        return FMath::Length2(sphere.m_vCenter - m_vCenter) <= (sphere.m_fRadius + m_fRadius) * (sphere.m_fRadius + m_fRadius);
    }

    // bool FSphere::Intersects(const FAABB& aabb) const
    // {
    //     return FMath::Intersects(*this, aabb);
    // }

    // bool FSphere::Intersects(const FPlane& plane) const
    // {
    //     return FMath::Intersects(*this, plane);
    // }

    // bool FSphere::Intersects(const FFrustum& frustum) const
    // {
    //     return FMath::Intersects(*this, frustum); 
    // }

    bool FSphere::IfInSphere(const FSphere& sphere) const
    {
        float d = sphere.m_fRadius - m_fRadius;
        if (d < 0.0f)
            return false;
        return (FMath::Length2(m_vCenter - sphere.m_vCenter) < d * d);
    }

    bool FSphere::IfOutOfSphere(const FSphere& sphere) const
    {
        float d = sphere.m_fRadius - m_fRadius;
        if (d > 0.0f)
            return false;
        return (FMath::Length2(m_vCenter - sphere.m_vCenter) < d * d);
    }

    bool FSphere::operator ==(const FSphere& sphere) const
    {
        if (!FMath::IsEqual(m_fRadius, sphere.m_fRadius))
            return false;
        if (!FMath::IsEqual(m_vCenter.x, sphere.m_vCenter.x) || 
            !FMath::IsEqual(m_vCenter.y, sphere.m_vCenter.y) || 
            !FMath::IsEqual(m_vCenter.z, sphere.m_vCenter.z))
            return false;
        return true;
    }

    bool FSphere::operator !=(const FSphere& sphere) const
    {
        return !(*this == sphere);
    }
    
}; //LostPeterFoundation