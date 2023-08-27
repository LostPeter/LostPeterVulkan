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

#ifndef _F_RAY_H_
#define _F_RAY_H_

#include "FPreDefine.h"

namespace LostPeterFoundation
{
    class LPF_Export FRay
    {
    public:
        FRay()
            : m_vOrigin(0.0f, 0.0f, 0.0f)
            , m_vDirection(0.0f, 0.0f, 1.0f)
        {

        }
        FRay(const FVector3& vOrigin, const FVector3& vDir)
            : m_vOrigin(vOrigin)
            , m_vDirection(vDir)
        {

        }
        FRay(const FRay& src)
            : m_vOrigin(src.m_vOrigin)
            , m_vDirection(src.m_vDirection)
        {

        }

    public:
        FVector3 m_vOrigin;		
        FVector3 m_vDirection;	
        
    public:
        LP_FORCEINLINE const FVector3& GetOrigin() const { return m_vOrigin; }
        LP_FORCEINLINE void SetOrigin(const FVector3& vOrigin) { m_vOrigin = vOrigin; }
        LP_FORCEINLINE const FVector3& GetDirection() const { return m_vDirection; }
        LP_FORCEINLINE void SetDirection(const FVector3& vDir) { m_vDirection	= vDir; }	

        LP_FORCEINLINE FVector3 GetPoint(float t) const { return m_vOrigin + (m_vDirection * t); }
        float GetDistance(const FVector3& vRayPt) const;

        std::pair<bool, float> Intersects_Plane(const FPlane& plane) const;		
        std::pair<bool, float> Intersects_PlaneBoundedVolume(const FPlaneBoundedVolume& planeBoundedVolume) const;
        std::pair<bool, float> Intersects_Sphere(const FSphere& sphere) const;		
        std::pair<bool, float> Intersects_AABB(const FAABB& aabb) const;

    public:	
        bool operator ==(const FRay& ray) const;
        bool operator !=(const FRay& ray) const;
        FVector3 operator *(float t) const;

        friend std::ostream& operator <<(std::ostream& o, const FRay& ray)
        {
            o << "Ray: (Origin = " << ray.m_vOrigin.x << "," << ray.m_vOrigin.y << "," << ray.m_vOrigin.z
              << ", Direction = " << ray.m_vDirection.x << "," << ray.m_vDirection.y << "," << ray.m_vDirection.z << ")";
            return o;
        }
    };

}; //LostPeterFoundation

#endif