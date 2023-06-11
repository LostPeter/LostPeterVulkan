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

#include "../include/FFrustum.h"
#include "../include/FAABB.h"

namespace LostPeterFoundation
{
    bool FFrustum::CheckPoint(const FVector3& point)
    {
        for (int32 i = 0; i < 6; i++)
        {  
            if (m_aPlanes[i].GetDistance(point) < 0)
                return false;
        }
        return true;
    }

    bool FFrustum::CheckPoint(float posX, float posY, float posZ)
    {
        FVector3 point(posX, posY, posZ);
        return CheckPoint(point);
    }

    bool FFrustum::CheckCube(float centerX, float centerY, float centerZ, float Size)	
    {
        FVector3 v1(centerX-Size, centerY-Size, centerZ-Size);
        FVector3 v2(centerX+Size, centerY-Size, centerZ-Size);
        FVector3 v3(centerX-Size, centerY+Size, centerZ-Size);
        FVector3 v4(centerX+Size, centerY+Size, centerZ-Size);
        FVector3 v5(centerX-Size, centerY-Size, centerZ+Size);
        FVector3 v6(centerX+Size, centerY-Size, centerZ+Size);
        FVector3 v7(centerX-Size, centerY+Size, centerZ+Size);
        FVector3 v8(centerX+Size, centerY+Size, centerZ+Size);

        for (int32 i = 0; i < 6; i++)
        {
            if (m_aPlanes[i].GetDistance(v1) >= 0)
                continue;
            if (m_aPlanes[i].GetDistance(v2) >= 0)
                continue;
            if (m_aPlanes[i].GetDistance(v3) >= 0)
                continue;
            if (m_aPlanes[i].GetDistance(v4) >= 0)
                continue;
            if (m_aPlanes[i].GetDistance(v5) >= 0)
                continue;
            if (m_aPlanes[i].GetDistance(v6) >= 0)
                continue;
            if (m_aPlanes[i].GetDistance(v7) >= 0)
                continue;
            if (m_aPlanes[i].GetDistance(v8) >= 0)
                continue;

            return false;
        }
        return true;
    }			

    bool FFrustum::CheckRectangle(float centerX, float centerY, float centerZ, float sizeX, float sizeY, float sizeZ)		
    {
        FVector3 v1(centerX-sizeX, centerY-sizeY, centerZ-sizeZ);
        FVector3 v2(centerX+sizeX, centerY-sizeY, centerZ-sizeZ);
        FVector3 v3(centerX-sizeX, centerY+sizeY, centerZ-sizeZ);
        FVector3 v4(centerX+sizeX, centerY+sizeY, centerZ-sizeZ);
        FVector3 v5(centerX-sizeX, centerY-sizeY, centerZ+sizeZ);
        FVector3 v6(centerX+sizeX, centerY-sizeY, centerZ+sizeZ);
        FVector3 v7(centerX-sizeX, centerY+sizeY, centerZ+sizeZ);
        FVector3 v8(centerX+sizeX, centerY+sizeY, centerZ+sizeZ);

        for (int32 i = 0; i < 6; i++)
        {
            if (m_aPlanes[i].GetDistance(v1) >= 0)
                continue;
            if (m_aPlanes[i].GetDistance(v2) >= 0)
                continue;
            if (m_aPlanes[i].GetDistance(v3) >= 0)
                continue;
            if (m_aPlanes[i].GetDistance(v4) >= 0)
                continue;
            if (m_aPlanes[i].GetDistance(v5) >= 0)
                continue;
            if (m_aPlanes[i].GetDistance(v6) >= 0)
                continue;
            if (m_aPlanes[i].GetDistance(v7) >= 0)
                continue;
            if (m_aPlanes[i].GetDistance(v8) >= 0)
                continue;

            return false;
        }
        return true;
    }

    bool FFrustum::CheckAABB(FAABB& aabb)
    {
        FVector3 vCenter = aabb.GetCenter();
        FVector3 vExtent = aabb.GetExtents();
        return CheckRectangle(vCenter.x, vCenter.y, vCenter.z,
                              vExtent.x, vExtent.y, vExtent.z);
    }

    bool FFrustum::CheckSphere(float centerX, float centerY, float centerZ, float radius)
    {
        FVector3 v(centerX, centerY, centerZ);
        for (int32 i = 0; i < 6; i++) 
        {
            if (m_aPlanes[i].GetDistance(v) < -radius)
                return false;
        }
        return true;
    }

    bool FFrustum::operator ==(const FFrustum& rhs) const
    {
        for (int32 i = 0; i < 6; i++)
        {
            const FPlane& planeSrc = GetPlane(i);
            const FPlane& planeCmp = rhs.GetPlane(i);

            if (planeSrc != planeCmp)
                return false;
        }
        return true;
    }

    bool FFrustum::operator !=(const FFrustum& rhs) const
    {
        return !(*this == rhs);
    }
    
}; //LostPeterFoundation