/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-11-11
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _OBJECT_FRUSTUM_H_
#define _OBJECT_FRUSTUM_H_

#include "Object.h"

namespace LostPeterEngine
{
    class engineExport ObjectFrustum : public Object
    {
    public:
        ObjectFrustum(const String& nameObject, SceneManager* pSceneManager);
        virtual ~ObjectFrustum();

    public:
    protected:
        mutable FAABB m_aabb;
        
        float m_fFovY;						
		float m_fNearDist;					
		float m_fFarDist;		

    public:
        virtual float GetFovY() const { return m_fFovY; }
		virtual void SetFovY(float fFovY)
		{
			m_fFovY = fFovY;
			invalidateFrustum();
		}

		virtual float GetNearClipDistance() const { return m_fNearDist; }
		virtual void SetNearClipDistance(float fNearDist)
		{
			F_Assert(fNearDist > 0 && "ObjectFrustum::SetNearClipDistance");
			
			m_fNearDist = fNearDist;
			invalidateFrustum();
		}

		virtual float GetFarClipDistance() const { return m_fFarDist; }
		virtual void SetFarClipDistance(float fFarDist)
		{
			m_fFarDist = fFarDist;
			invalidateFrustum();
		}

        virtual const FMatrix4&	GetProjectionMatrixWithRSDepth() const;

    ////Movable
    public:
        virtual const FAABB& GetBoundingAABB() const;


    protected:
        virtual void invalidateFrustum() const
		{
			
		}

    };

}; //LostPeterEngine

#endif