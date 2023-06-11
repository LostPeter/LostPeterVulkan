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

#ifndef _F_PLANE_H_
#define _F_PLANE_H_

#include "FPreDefine.h"

namespace LostPeterFoundation
{
    class LPF_Export FPlane
    {
    public:
        FPlane()
            : m_vNormal(0.0f)
            , m_fDistance(0)
        {

        }
        FPlane(const FVector3& vNormal, float fDistance)
            : m_vNormal(vNormal)
            , m_fDistance(fDistance)
        {

        }
        FPlane(float a, float b, float c, float d)
            : m_vNormal(a, b, c)
            , m_fDistance(d)
        {

        }
        FPlane(const FVector3& vNormal, const FVector3& vPoint)
        {
            Redefine(vNormal, vPoint);
        }
        FPlane(const FVector3& vPoint0, const FVector3& vPoint1, const FVector3& vPoint2)
        {
            Redefine(vPoint0, vPoint1, vPoint2);
        }
        FPlane(const FPlane& src)
            : m_vNormal(src.m_vNormal)
            , m_fDistance(src.m_fDistance)
        {

        }
        ~FPlane()
        {

        }

    public:
        FVector3 m_vNormal;			
        float m_fDistance;		
        
    public:
        LP_FORCEINLINE const FVector3& GetNormal() const { return m_vNormal; }
        LP_FORCEINLINE FVector3& GetNormal() { return m_vNormal; }
        LP_FORCEINLINE float GetDistance() const { return m_fDistance; }

    public:
        FPlaneSideType GetSide(const FVector3& vPoint) const;								
        FPlaneSideType GetSide(const FAABB& aabb) const;								
        FPlaneSideType GetSide(const FVector3& center, const FVector3& halfSize) const;		
        float GetDistance(const FVector3& vPoint) const;							
        void Redefine(const FVector3& vPoint0, const FVector3& vPoint1, const FVector3& vPoint2);
        void Redefine(const FVector3& vNormal, const FVector3& vPoint);			
        FVector3 ProjectVector(const FVector3& v) const;									
        float Normalize();							

    public:
        bool operator ==(const FPlane& rhs) const
        {
            return (rhs.m_fDistance == m_fDistance && rhs.m_vNormal == m_vNormal);
        }
        bool operator !=(const FPlane& rhs) const
        {
            return (rhs.m_fDistance != m_fDistance && rhs.m_vNormal != m_vNormal);
        }

        FPlane operator *(const FMatrix4& mat4) const;

        friend std::ostream& operator <<(std::ostream& o, const FPlane& plane)
        {
            o << "Plane: (Normal = " << plane.m_vNormal.x << "," << plane.m_vNormal.y << "," << plane.m_vNormal.z 
              << ", Distance = " << plane.m_fDistance << ")";
            return o;
        }
    };

}; //LostPeterFoundation

#endif