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

#ifndef _F_PLANE_BOUNDED_VOLUME_H_
#define _F_PLANE_BOUNDED_VOLUME_H_

#include "FPreDefine.h"
#include "FPlane.h"

namespace LostPeterFoundation
{
    class LPF_Export FPlaneBoundedVolume
    {
    public:
        FPlaneBoundedVolume()  
            : m_ePlaneSide(F_PlaneSide_Negative)
        {

        }
        FPlaneBoundedVolume(FPlaneSideType ePlaneSideOutside)  
            : m_ePlaneSide(ePlaneSideOutside)
        {

        }
        ~FPlaneBoundedVolume()
        {

        }

    public:
        FPlaneVector m_aPlanes;
        FPlaneSideType m_ePlaneSide;

    public:
        int GetPlaneCount() const;
        const FPlane& GetPlaneRef(int index) const;
        FPlane* GetPlanePtr(int index);
        FPlane* GetPlanePtr();

    public:
        std::pair<bool, float> Intersects_Ray(const FRay& ray);
        bool Intersects_AABB(const FAABB& aabb) const;
        bool Intersects_Sphere(const FSphere& sphere) const;

    public:
        bool operator ==(const FPlaneBoundedVolume& rhs) const;
        bool operator !=(const FPlaneBoundedVolume& rhs) const;

        friend std::ostream& operator <<(std::ostream& o, const FPlaneBoundedVolume& planeBoundedVolume)
        {
            for (FPlaneVector::const_iterator it = planeBoundedVolume.m_aPlanes.begin(); 
                 it != planeBoundedVolume.m_aPlanes.end(); ++it)
            {
                const FPlane& plane = *it;
                o << plane;
            }
            return o;
        }
    };

}; //LostPeterFoundation

#endif