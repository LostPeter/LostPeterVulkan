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

#ifndef _F_FRUSTUM_H_
#define _F_FRUSTUM_H_

#include "FPreDefine.h"
#include "FPlane.h"

namespace LostPeterFoundation
{
    class LPF_Export FFrustum
    {
    public:
        FFrustum()
        {

        }
        ~FFrustum()
        {

        }

    public:
        FPlane m_aPlanes[6];

    public:
        LP_FORCEINLINE const FPlane& GetPlane(FFrustumPlaneType eType) const { return (m_aPlanes[(int32)eType]); }
        LP_FORCEINLINE const FPlane& GetPlane(int32 index) const { return (m_aPlanes[index]); }

    public:
        bool CheckPoint(const FVector3& point);	
        bool CheckPoint(float posX, float posY, float posZ);
        bool CheckCube(float centerX, float centerY, float centerZ, float Size);				
        bool CheckRectangle(float centerX, float centerY, float centerZ, float sizeX, float sizeY, float sizeZ);		
        bool CheckAABB(FAABB& aabb);
        bool CheckSphere(float centerX, float centerY, float centerZ, float radius);

    public:
        bool operator ==(const FFrustum& rhs) const;
        bool operator !=(const FFrustum& rhs) const;

        friend std::ostream& operator <<(std::ostream& o, const FFrustum& frustum)
        {
            for (int32 i = 0; i < 6; i++)
            {
                const FPlane& plane = frustum.m_aPlanes[i];
                o << "Frustum Plane: " << i << ": "
                  << plane;
            }
            return o;
        }	
    };

}; //LostPeterFoundation

#endif