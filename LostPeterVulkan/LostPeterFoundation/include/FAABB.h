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

#ifndef _F_AABB_H_
#define _F_AABB_H_

#include "FPreDefine.h"

namespace LostPeterFoundation
{
    class foundationExport FAABB
    {
    public:
        FAABB()
            : m_vMin(-0.5f, -0.5f, -0.5f)
            , m_vMax(0.5f, 0.5f, 0.5f)
            , m_eAABBExtent(F_AABBExtent_Null)
            , m_pCorners(nullptr)
        {
            
        }
        FAABB(FAABBExtentType eAABBExtent)
            : m_vMin(-0.5f, -0.5f, -0.5f)
            , m_vMax(0.5f, 0.5f, 0.5f)
            , m_eAABBExtent(eAABBExtent)
            , m_pCorners(nullptr)
        {

        }
        FAABB(const FAABB& aabb) 
            : m_pCorners(nullptr)
        {
            if(aabb.IsNull())
				SetNull();
			else if(aabb.IsInfinite())
				SetInfinite();
			else
				SetExtents(aabb.m_vMin, aabb.m_vMax);
        }
        FAABB(const FVector3& min, const FVector3& max)
            : m_pCorners(nullptr)
        {
            SetExtents(min, max);
        }
        FAABB(float minX, float minY, float minZ, 
                   float maxX, float maxY, float maxZ)
            : m_pCorners(nullptr)
        {
            SetExtents(minX, minY, minZ, maxX, maxY, maxZ);
        }
        ~FAABB()
        {
            
        }

    public:
        static const FAABB	ms_aabbNull;		
        static const FAABB	ms_aabbInfinite;

    public:
        FVector3 m_vMin;				
        FVector3 m_vMax;
        FAABBExtentType m_eAABBExtent;
        FVector3* m_pCorners;

    public:
        //Min
        F_FORCEINLINE const FVector3& GetMin() const { return m_vMin; }
        F_FORCEINLINE FVector3& GetMin() { return m_vMin; }
        F_FORCEINLINE void GetMin(FVector3& vMin) const { vMin = m_vMin; }
        F_FORCEINLINE float GetMin(uint32 axis) const	{ return m_vMin[axis]; }
        F_FORCEINLINE void SetMin(const FVector3& vMin) 
        { 
            m_vMin = vMin; 
            m_eAABBExtent = F_AABBExtent_Finite;
        }
        F_FORCEINLINE void SetMin(float x, float y, float z)
        {
            m_vMin.x = x;
            m_vMin.y = y;
            m_vMin.z = z;
            m_eAABBExtent = F_AABBExtent_Finite;
        }
        F_FORCEINLINE void SetMinX(float x) { m_vMin.x = x; m_eAABBExtent = F_AABBExtent_Finite; }
        F_FORCEINLINE void SetMinY(float y) { m_vMin.y = y; m_eAABBExtent = F_AABBExtent_Finite; }
        F_FORCEINLINE void SetMinZ(float z) { m_vMin.z = z; m_eAABBExtent = F_AABBExtent_Finite; }

        //Max
        F_FORCEINLINE const FVector3& GetMax() const	{ return m_vMax; }
        F_FORCEINLINE FVector3& GetMax() { return m_vMax; }
        F_FORCEINLINE void GetMax(FVector3& vMax) const { vMax = m_vMax; }
        F_FORCEINLINE float GetMax(uint32 axis) const	{ return m_vMax[axis]; }
        F_FORCEINLINE void SetMax(const FVector3& vMax) 
        { 
            m_vMax = vMax; 
            m_eAABBExtent = F_AABBExtent_Finite;
        }
        F_FORCEINLINE void SetMax(float x,float y,float z)
        {
            m_vMax.x = x;
            m_vMax.y = y;
            m_vMax.z = z;
            m_eAABBExtent = F_AABBExtent_Finite;
        }
        F_FORCEINLINE void SetMaxX(float x) { m_vMax.x = x; m_eAABBExtent = F_AABBExtent_Finite; }
        F_FORCEINLINE void SetMaxY(float y) { m_vMax.y = y; m_eAABBExtent = F_AABBExtent_Finite; }
        F_FORCEINLINE void SetMaxZ(float z) { m_vMax.z = z; m_eAABBExtent = F_AABBExtent_Finite; }

        //Min-Max
        F_FORCEINLINE void SetMinMax(const FVector3& min, const FVector3& max)
        {
            m_vMin = min;		
            m_vMax = max;
            m_eAABBExtent = F_AABBExtent_Finite;
        }
        F_FORCEINLINE void SetCenterExtents(const FVector3& center, const FVector3& extent)
        {
            m_vMin = center - extent;		
            m_vMax = center + extent;
            m_eAABBExtent = F_AABBExtent_Finite;
        }
        F_FORCEINLINE void SetPoint(const FVector3& pt) 
        { 
            m_vMin = m_vMax = pt; 
            m_eAABBExtent = F_AABBExtent_Finite;
        }

        
        //Center
        F_FORCEINLINE FVector3 GetCenter() const
        {
            return (m_vMax + m_vMin) * 0.5f;
        }
        F_FORCEINLINE void GetCenter(FVector3& center) const
        {
            center = (m_vMax + m_vMin) * 0.5f;
        }
        F_FORCEINLINE float GetCenter(uint32 axis) const
        {
            return (m_vMax[axis] + m_vMin[axis]) * 0.5f;
        }

        //Size/Extents
        F_FORCEINLINE FVector3 GetSize() const 
        { 
            switch ((int32)m_eAABBExtent)
            {
            case F_AABBExtent_Null:
                return FVector3(0, 0, 0);
            case F_AABBExtent_Finite:
                return m_vMax - m_vMin; 
            case F_AABBExtent_Infinite:
                return FVector3(F_C_MAX_FLOAT, F_C_MAX_FLOAT, F_C_MAX_FLOAT);
            }
            return FVector3(0, 0, 0);
        }
        F_FORCEINLINE FVector3 GetHalfSize() const 
        { 
            switch ((int32)m_eAABBExtent)
            {
            case F_AABBExtent_Null:
                return FVector3(0, 0, 0);
            case F_AABBExtent_Finite:
                return (m_vMax - m_vMin) * 0.5f;
            case F_AABBExtent_Infinite:
                return FVector3(F_C_MAX_FLOAT, F_C_MAX_FLOAT, F_C_MAX_FLOAT);
            }
            return FVector3(0, 0, 0);
        }
        F_FORCEINLINE FVector3 GetExtents() const { return (m_vMax - m_vMin) * 0.5f; }
        F_FORCEINLINE void GetExtents(FVector3& extents) const { extents = (m_vMax - m_vMin) * 0.5f; }
        F_FORCEINLINE float GetExtents(uint32 axis) const { return (m_vMax[axis] - m_vMin[axis]) * 0.5f; }
        F_FORCEINLINE void SetExtents(const FVector3& min, const FVector3& max)
        {
            F_Assert((min.x <= max.x && min.y <= max.y && min.z <= max.z) && "FAABB::SetExtents")
            m_vMin = min;
            m_vMax = max;
            m_eAABBExtent = F_AABBExtent_Finite;
        }
        F_FORCEINLINE void SetExtents(float minX, float minY, float minZ, float maxX, float maxY, float maxZ)
        {
            F_Assert((minX <= maxX && minY <= maxY && minZ <= maxZ) && "FAABB::SetExtents")
            m_vMin.x = minX;
            m_vMin.y = minY;
            m_vMin.z = minZ;
            m_vMax.x = maxX;
            m_vMax.y = maxY;
            m_vMax.z = maxZ;
            m_eAABBExtent = F_AABBExtent_Finite;
        }
        
        F_FORCEINLINE void GetDiagonal(FVector3& diagonal) const { diagonal = m_vMax - m_vMin; }
        F_FORCEINLINE float GetWidth() const { return m_vMax.x - m_vMin.x; }
        F_FORCEINLINE float GetHeight() const { return m_vMax.y - m_vMin.y; }
        F_FORCEINLINE  float GetDepth() const { return m_vMax.z - m_vMin.z;	}
        F_FORCEINLINE float GetVolume() const { return GetWidth() * GetHeight() * GetDepth(); }


        F_FORCEINLINE void Extend(const FVector3& p)
        {
            if(p.x > m_vMax.x)	
                m_vMax.x = p.x;
            if(p.x < m_vMin.x)	
                m_vMin.x = p.x;

            if(p.y > m_vMax.y)	
                m_vMax.y = p.y;
            if(p.y < m_vMin.y)	
                m_vMin.y = p.y;

            if(p.z > m_vMax.z)	
                m_vMax.z = p.z;
            if(p.z < m_vMin.z)	
                m_vMin.z = p.z;
        }

    public:
        F_FORCEINLINE bool IsValid() const
        {
            if(m_vMin.x > m_vMax.x)	
                return false;
            if(m_vMin.y > m_vMax.y)
                return false;
            if(m_vMin.z > m_vMax.z)	
                return false;
            return true;
        }

        F_FORCEINLINE bool IsNull() const
        {
            return m_eAABBExtent == F_AABBExtent_Null;
        }
        F_FORCEINLINE bool IsInfinite() const
        {
            return m_eAABBExtent == F_AABBExtent_Infinite;
        }
        F_FORCEINLINE bool IsFinite() const
        {
            return m_eAABBExtent == F_AABBExtent_Finite;
        }

        F_FORCEINLINE void SetNull()
        {
            m_eAABBExtent = F_AABBExtent_Null;
        }
        F_FORCEINLINE void SetInfinite()
        {
            m_eAABBExtent = F_AABBExtent_Finite;
        }

    public:
        FAABB&	Add(const FAABB& aabb);
        
        float MakeCube(FAABB& cube) const;	
        void MakeSphere(FSphere& sphere) const;

        bool IsInside(const FAABB& box)	const;	

        bool ContainsPoint(const FVector3& point) const;

        void Merge(const FVector3& point);
        bool Merge(const FAABB& aabb);

        void Transform(const FMatrix4& mat4);
        void TransformAffine(const FMatrix4& mat4);
        void Rotate(const FMatrix4& mat4, FAABB& aabb) const;

        bool Intersects_Point(const FVector3& v) const
        {
            if (IsNull())
                return false;
            if (IsInfinite())
                return true;

            return v.x >= m_vMin.x && v.x <= m_vMax.x && 
                v.y >= m_vMin.y && v.y <= m_vMax.y && 
                v.z >= m_vMin.z && v.z <= m_vMax.z;
        }

        bool Intersects_Axis(const FAABB& aabb, uint32 axis) const
        {
            if (IsNull() || aabb.IsNull())
                return false;
            if (IsInfinite() || aabb.IsInfinite())
                return true;

            if (m_vMax[axis] < aabb.m_vMin[axis] || aabb.m_vMax[axis] < m_vMin[axis])
                return false;
            return true;
        }
        bool Intersects_AABB(const FAABB& aabb) const
        {
            if (IsNull() || aabb.IsNull())
                return false;
            if (IsInfinite() || aabb.IsInfinite())
                return true;

            if (m_vMax.x < aabb.m_vMin.x)
                return false;
            if (m_vMax.y < aabb.m_vMin.y)
                return false;
            if (m_vMax.z < aabb.m_vMin.z)
                return false;

            if (m_vMin.x > aabb.m_vMax.x)
                return false;
            if (m_vMin.y > aabb.m_vMax.y)
                return false;
            if (m_vMin.z > aabb.m_vMax.z)
                return false;
            return true;
        }

        bool Intersects_Ray(const FRay& ray) const;
        bool Intersects_Plane(const FPlane& plane) const;
        //bool Intersects_Segment(const FSegment& segment) const;
        bool Intersects_Sphere(const FSphere& sphere) const;
        FAABB Intersection_AABB(const FAABB& aabb) const;


        bool Contains(const FVector3& v) const
        {
            return Intersects_Point(v);
        }
        bool Contains(const FAABB& other) const
        {
            if (other.IsNull() || IsInfinite())
                return true;
            if (IsNull() || other.IsInfinite())
                return false;

            return this->m_vMin.x <= other.m_vMin.x &&
                this->m_vMin.y <= other.m_vMin.y &&
                this->m_vMin.z <= other.m_vMin.z &&
                other.m_vMax.x <= this->m_vMax.x &&
                other.m_vMax.y <= this->m_vMax.y &&
                other.m_vMax.z <= this->m_vMax.z;
        }

    public:
        FAABB& operator =(const FAABB& rhs)
        {
            SetExtents(rhs.m_vMin, rhs.m_vMax);
            return *this;
        }

        bool operator ==(const FAABB& rhs) const
        {
            return this->m_vMin == rhs.m_vMin &&
                this->m_vMax == rhs.m_vMax;
        }

        bool operator !=(const FAABB& rhs) const
        {
            return !(*this == rhs);
        }

        FAABB& operator *=(float s)
        {
            FVector3 center; GetCenter(center);
            FVector3 extents; GetExtents(extents);
            SetCenterExtents(center, extents * s);
            return *this;
        }

        FAABB& operator /=(float s)
        {
            FVector3 center; GetCenter(center);
            FVector3 extents; GetExtents(extents);
            SetCenterExtents(center, extents / s);
            return *this;
        }

        FAABB& operator +=(const FVector3& trans)
        {
            m_vMin += trans;
            m_vMax += trans;
            return *this;
        }

        friend std::ostream& operator <<(std::ostream& o, const FAABB& aabb)
        {
            o << "AABB: (Min = " << aabb.m_vMin.x << "," << aabb.m_vMin.y << "," << aabb.m_vMin.z
              << ", Max = " << aabb.m_vMax.x << "," << aabb.m_vMax.y << "," << aabb.m_vMax.z << ")";
            return o;
        }
    };

}; //LostPeterFoundation

#endif