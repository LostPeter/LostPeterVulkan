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

#include "../include/FAABB.h"
#include "../include/FMath.h"
#include "../include/FPlane.h"
#include "../include/FSphere.h"

namespace LostPeterFoundation
{
    const FAABB FAABB::ms_aabbNull;
    const FAABB FAABB::ms_aabbInfinite(-F_C_MAX_FLOAT, -F_C_MAX_FLOAT, -F_C_MAX_FLOAT,
                                        F_C_MAX_FLOAT,  F_C_MAX_FLOAT,  F_C_MAX_FLOAT);

    FAABB&	FAABB::Add(const FAABB& aabb)																				
    {
        FVector3 min; GetMin(min);
        FVector3 temp; aabb.GetMin(temp);
        FMath::Min(min, temp);

        FVector3 max; GetMax(max);
        aabb.GetMax(temp);
        FMath::Max(max, temp);

        SetMinMax(min, max);
        return *this;
    }

    float FAABB::MakeCube(FAABB& aabb) const	
    {
        FVector3 extent = GetExtents();
        float max = FMath::Max(extent);

        FVector3 center = GetCenter();
        aabb.SetCenterExtents(center, FVector3(max, max, max));
        return max;
    }

    void FAABB::MakeSphere(FSphere& sphere)	const	
    {
        FVector3 center = sphere.GetCenter();
        GetExtents(center);
        float fRad = FMath::Length(center) * 1.00001f;
        sphere.SetRadius(fRad);

        center = sphere.GetCenter();
        GetCenter(center);
    }

    bool FAABB::IsInside(const FAABB& box) const	
    {
        if(box.GetMin(0) > GetMin(0))	
            return false;
        if(box.GetMin(1) > GetMin(1))
            return false;
        if(box.GetMin(2) > GetMin(2))	
            return false;
        if(box.GetMax(0) < GetMax(0))
            return false;
        if(box.GetMax(1) < GetMax(1))	
            return false;
        if(box.GetMax(2) < GetMax(2))	
            return false;

        return true;
    }

    bool FAABB::ContainsPoint(const FVector3& point) const
    {
        if (point.x > GetMax(0) || point.x < GetMin(0)) 
            return false;	
        if (point.y > GetMax(1) || point.y < GetMin(1)) 
            return false;	
        if (point.z > GetMax(2) || point.z < GetMin(2)) 
            return false;
        return true;											
    }

    void FAABB::Merge(const FVector3& point)
    {
        switch ((int32)m_eAABBExtent)
        {
        case F_AABBExtent_Null: 
            SetExtents(point, point);
            return;
        case F_AABBExtent_Finite:
            FMath::MakeCeil(m_vMax, point);
            FMath::MakeFloor(m_vMin, point);
            return;
        case F_AABBExtent_Infinite: 
            return;
        }
        F_Assert(false && "FAABB::Merge: Wrong state !")
    }

    bool FAABB::Merge(const FAABB& aabb)
    {
        if (IsInfinite() || aabb.IsNull())
            return false;
        if (aabb.IsInfinite())
        {
            SetInfinite();
            return true;
        }
        if (IsNull())
        {
            SetExtents(aabb.m_vMin, aabb.m_vMax);
            return true;
        }

        FVector3 min = m_vMin;
        FVector3 max = m_vMax;
        FMath::MakeCeil(max, aabb.m_vMax);
        FMath::MakeFloor(min, aabb.m_vMin);

        SetExtents(min, max);
        return true;
    }

    void FAABB::Transform(const FMatrix4& mat4)
    {
        if (!IsFinite())
            return;

        FVector3 oldMin, oldMax, currentCorner;
        oldMin = m_vMin;
        oldMax = m_vMax;    
        SetNull();

        currentCorner = oldMin;
        Merge(FMath::Transform(mat4, currentCorner));

        currentCorner.z = oldMax.z;
        Merge(FMath::Transform(mat4, currentCorner));

        currentCorner.y = oldMax.y;
        Merge(FMath::Transform(mat4, currentCorner));

        currentCorner.z = oldMin.z;
        Merge(FMath::Transform(mat4, currentCorner));

        currentCorner.x = oldMax.x;
        Merge(FMath::Transform(mat4, currentCorner));

        currentCorner.z = oldMax.z;
        Merge(FMath::Transform(mat4, currentCorner));

        currentCorner.y = oldMin.y;
        Merge(FMath::Transform(mat4, currentCorner));

        currentCorner.z = oldMin.z;
        Merge(FMath::Transform(mat4, currentCorner)); 
    }   

    void FAABB::TransformAffine(const FMatrix4& mat4)
    {
        F_Assert(FMath::IsAffine(mat4) && "FAABB::TransformAffine")
        if (!IsFinite())
            return;

        FVector3 center = GetCenter();
        FVector3 halfSize = GetHalfSize();

        FVector3 newCenter = FMath::TransformAffine(mat4, center);
        FVector3 newHalfSize(
            FMath::Abs(mat4[0][0]) * halfSize.x + FMath::Abs(mat4[0][1]) * halfSize.y + FMath::Abs(mat4[0][2]) * halfSize.z, 
            FMath::Abs(mat4[1][0]) * halfSize.x + FMath::Abs(mat4[1][1]) * halfSize.y + FMath::Abs(mat4[1][2]) * halfSize.z,
            FMath::Abs(mat4[2][0]) * halfSize.x + FMath::Abs(mat4[2][1]) * halfSize.y + FMath::Abs(mat4[2][2]) * halfSize.z);

        SetExtents(newCenter - newHalfSize, newCenter + newHalfSize);
    }

    void FAABB::Rotate(const FMatrix4& mat4, FAABB& aabb) const
    {

    }

    bool FAABB::Intersects(const FRay& ray) const
    {
        float d1,d2;
        return FMath::Intersects(ray, *this, &d1, &d2);
    }

    bool FAABB::Intersects(const FSphere& sphere) const
    {   
        return FMath::Intersects(sphere, *this); 
    }

    bool FAABB::Intersects(const FPlane& plane) const
    {
        return FMath::Intersects(plane, *this);
    }

    // bool FAABB::Intersects(const FSegment& segment) const
    // {
    //     return FMath::Intersects(segment, *this);
    // }

    FAABB FAABB::Intersection(const FAABB& aabb) const
    {
        if (IsNull() || aabb.IsNull())
        {
            return FAABB();
        }
        if (IsInfinite())
        {
            return aabb;
        }
        if (aabb.IsInfinite())
        {
            return *this;
        }

        FVector3 intMin = m_vMin;
        FVector3 intMax = m_vMax;
        FMath::MakeCeil(intMin, aabb.GetMin());
        FMath::MakeFloor(intMax, aabb.GetMax());
        if (intMin.x < intMax.x &&
            intMin.y < intMax.y &&
            intMin.z < intMax.z)
        {
            return FAABB(intMin, intMax);
        }

        return FAABB();
    }

}; //LostPeterFoundation