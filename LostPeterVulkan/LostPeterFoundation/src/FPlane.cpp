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

#include "../include/FPlane.h"
#include "../include/FMath.h"
#include "../include/FAABB.h"

namespace LostPeterFoundation
{
    FPlaneSideType FPlane::GetSide(const FVector3& vPoint) const
    {
        float fDistance = GetDistance(vPoint);
        if (fDistance < 0.0)
            return F_PlaneSide_Negative;
        if (fDistance > 0.0)
            return F_PlaneSide_Positive;
        return F_PlaneSide_None;
    }

    FPlaneSideType FPlane::GetSide(const FAABB& aabb) const
    {
        if (!aabb.IsValid()) 
            return F_PlaneSide_None;

        FVector3 center;
        aabb.GetCenter(center);	
        return GetSide(center, aabb.GetHalfSize());
    }

    FPlaneSideType FPlane::GetSide(const FVector3& center, const FVector3& halfSize) const
    {
        float dist = GetDistance(center);
        float maxAbsDist = FMath::DotAbs(m_vNormal, halfSize);
        if (dist < -maxAbsDist)
            return F_PlaneSide_Negative;
        if (dist > +maxAbsDist)
            return F_PlaneSide_Positive;
        return F_PlaneSide_Both;
    }

    float FPlane::GetDistance(const FVector3& vPoint) const
    {
        return FMath::Dot(m_vNormal, vPoint) + m_fDistance;
    }

    void FPlane::Redefine(const FVector3& vPoint0, const FVector3& vPoint1, const FVector3& vPoint2)
    {
        FVector3 vEdge1 = vPoint1 - vPoint0;
        FVector3 vEdge2 = vPoint2 - vPoint0;
        m_vNormal = FMath::Cross(vEdge1, vEdge2);
        m_vNormal = FMath::Normalize(m_vNormal);
        m_fDistance = -FMath::Dot(m_vNormal, vPoint0);
    }

    void FPlane::Redefine(const FVector3& vNormal, const FVector3& vPoint)		
    {
        m_vNormal = vNormal;
        m_fDistance = -FMath::Dot(vNormal, vPoint);
    }

    FVector3 FPlane::ProjectVector(const FVector3& p) const
    {
        FMatrix3 xform;
        xform[0][0] = 1.0f - m_vNormal.x * m_vNormal.x;
        xform[0][1] = -m_vNormal.x * m_vNormal.y;
        xform[0][2] = -m_vNormal.x * m_vNormal.z;
        xform[1][0] = -m_vNormal.y * m_vNormal.x;
        xform[1][1] = 1.0f - m_vNormal.y * m_vNormal.y;
        xform[1][2] = -m_vNormal.y * m_vNormal.z;
        xform[2][0] = -m_vNormal.z * m_vNormal.x;
        xform[2][1] = -m_vNormal.z * m_vNormal.y;
        xform[2][2] = 1.0f - m_vNormal.z * m_vNormal.z;

        return xform * p;
    }

    float FPlane::Normalize()														
    {
        float fLength = FMath::Length(m_vNormal);
        if (fLength > FMath::ms_fEpsilon)
        {
            float fInvLength = 1 / fLength;
            m_vNormal *= fInvLength;
            m_fDistance *= fInvLength;
        }
        return fLength;
    }

    //Point - InPlaneSameSide/NotInPlaneSameSide
    bool FPlane::IsPoint2InSameSide(const FVector3& pt1, const FVector3& pt2)
    {
        return FMath::Points2_InPlaneSameSide(pt1, pt2, *this);
    }
	bool FPlane::IsPoint2NotInSameSide(const FVector3& pt1, const FVector3& pt2)
    {
        return FMath::Points2_NotInPlaneSameSide(pt1, pt2, *this);
    }

    FPlane FPlane::operator *(const FMatrix4& mat4) const
    {
        FPlane ret;
        FMatrix4 invTrans = FMath::TransposeMatrix4(FMath::InverseMatrix4(mat4));
        FVector4 v4(m_vNormal.x, m_vNormal.y, m_vNormal.z, m_fDistance);
        v4 = invTrans * v4;
        ret.m_vNormal.x = v4.x; 
        ret.m_vNormal.y = v4.y; 
        ret.m_vNormal.z = v4.z;
        ret.m_fDistance = v4.w / FMath::Length(ret.m_vNormal);
        ret.m_vNormal = FMath::Normalize(ret.m_vNormal);

        return ret;
    }
    
}; //LostPeterFoundation