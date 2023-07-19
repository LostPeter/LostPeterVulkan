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

#ifndef _F_SPHERE_H_
#define _F_SPHERE_H_

#include "FPreDefine.h"

namespace LostPeterFoundation
{
    class LPF_Export FSphere
    {
    public:
        FSphere()
            : m_vCenter(0.0f, 0.0f, 0.0f)
            , m_fRadius(1.0f)
        {

        }
        FSphere(const FVector3& vCenter, float fRadius)
            : m_vCenter(vCenter)
            , m_fRadius(fRadius)
        {
            
        }
        FSphere(const FSphere& sphere)
            : m_vCenter(sphere.m_vCenter)
            , m_fRadius(sphere.m_fRadius)
        {
        
        }

        ~FSphere()
        {

        }

    public:
        FVector3 m_vCenter;		
        float m_fRadius;

    public:
        LP_FORCEINLINE const FVector3& GetCenter() const { return m_vCenter; }
        LP_FORCEINLINE void SetCenter(const FVector3& center) { m_vCenter = center; }
        LP_FORCEINLINE float GetRadius() const { return m_fRadius; }
        LP_FORCEINLINE void SetRadius(float radius) { m_fRadius = radius; }

        LP_FORCEINLINE void Set(const FVector3& center, float radius)
        {
            m_vCenter = center;
            m_fRadius = radius;
        }

        bool Intersects_Point(const FVector3& point) const;
        bool Intersects_Plane(const FPlane& plane) const;
        bool Intersects_Sphere(const FSphere& sphere) const;
        bool Intersects_AABB(const FAABB& aabb) const;
        bool Intersects_Frustum(const FFrustum& frustum) const;


        bool IfInSphere(const FSphere& sphere) const;
        bool IfOutOfSphere(const FSphere& sphere) const;

    public:
        bool operator ==(const FSphere& sphere) const;
        bool operator !=(const FSphere& sphere) const;

        friend std::ostream& operator <<(std::ostream& o, const FSphere& sphere)
        {
            o << "Sphere: (Center = " << sphere.m_vCenter.x << "," << sphere.m_vCenter.y << "," << sphere.m_vCenter.z
            << ", Radius = " << sphere.m_fRadius << ")";
            return o;
        }
    };

}; //LostPeterFoundation

#endif