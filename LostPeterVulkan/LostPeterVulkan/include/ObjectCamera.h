/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-04
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _OBJECT_CAMERA_H_
#define _OBJECT_CAMERA_H_

#include "ObjectFrustum.h"


namespace LostPeter
{
    class utilExport ObjectCamera : public ObjectFrustum
    {
    public:
        ObjectCamera(const String& nameCamera, Scene* pScene);
        virtual ~ObjectCamera();

    public:
    protected:
        static const String	ms_strMovableType;	


    public:
    protected:
        ObjectVisibleBoundsInfo* m_pObjectVisibleBoundsInfo;

        FVector3 m_vPos;	
		FQuaternion	m_qRot;				
			
		mutable FVector3 m_vDerivedPos;			
		mutable FQuaternion	m_qDerivedRot;		
		
		mutable FVector3 m_vRealPos;
		mutable FQuaternion	m_qRealRot;		

    public:
        LP_FORCEINLINE ObjectVisibleBoundsInfo* GetObjectVisibleBoundsInfo() { return m_pObjectVisibleBoundsInfo; }


        LP_FORCEINLINE const FVector3& GetPosition() const { return m_vPos; }
		LP_FORCEINLINE void	SetPosition(float x, float y, float z)
		{
			m_vPos.x = x;
			m_vPos.y = y;
			m_vPos.z = z;
			//invalidateView();
		}
		LP_FORCEINLINE void	SetPosition(const FVector3& vPos)
		{
			m_vPos = vPos;
			//invalidateView();
		}

		LP_FORCEINLINE void	Move(const FVector3& vDelta)
		{
			m_vPos = m_vPos + vDelta;
			//invalidateView();
		}

		LP_FORCEINLINE void	MoveRelative(const FVector3& vMove)
		{
			FVector3 vTrans = m_qRot * vMove;
			m_vPos = m_vPos + vTrans;
			//invalidateView();
		}
		
		LP_FORCEINLINE FVector3	GetDirection() const { return m_qRot * -FMath::ms_v3UnitZ; }
		void SetDirection(const FVector3& vDir);
		LP_FORCEINLINE void	SetDirection(float x, float y, float z)
		{
			SetDirection(FVector3(x, y, z));
		}

		LP_FORCEINLINE FVector3	GetUp() const { return m_qRot * FMath::ms_v3UnitY; }
		LP_FORCEINLINE FVector3	GetRight() const { return m_qRot * FMath::ms_v3UnitX; }

		LP_FORCEINLINE const FQuaternion& GetOrientation() const { return m_qRot; }
		LP_FORCEINLINE void	SetOrientation(const FQuaternion& qRot)
		{
			m_qRot = FMath::Normalize(qRot);
			//invalidateView();
		}

		LP_FORCEINLINE const FVector3& GetDerivedPosition() const
		{
			//updateView();
			return m_vDerivedPos;
		}

		LP_FORCEINLINE const FQuaternion& GetDerivedOrientation() const
		{
			//updateView();
			return m_qDerivedRot;
		}

		LP_FORCEINLINE FVector3	GetDerivedDirection() const
		{
			//updateView();
			return m_qDerivedRot * FMath::ms_v3UnitNegZ;

		}

		LP_FORCEINLINE FVector3	GetDerivedUp() const
		{
			//updateView();
			return m_qDerivedRot * FMath::ms_v3UnitY;
		}

		LP_FORCEINLINE FVector3	GetDerivedRight() const
		{
			//updateView();
			return m_qDerivedRot * FMath::ms_v3UnitX;
		}

		LP_FORCEINLINE const FVector3& GetRealPosition() const
		{
			//updateView();
			return m_vRealPos;
		}
		
		LP_FORCEINLINE const FQuaternion& GetRealOrientation() const
		{
			//updateView();
			return m_qRealRot;
		}

		LP_FORCEINLINE FVector3	GetRealDirection() const
		{
			//updateView();
			return m_qRealRot * FMath::ms_v3UnitNegZ;
		}

		LP_FORCEINLINE FVector3	GetRealUp() const
		{
			//updateView();
			return m_qRealRot * FMath::ms_v3UnitY;
		}

		LP_FORCEINLINE FVector3	GetRealRight() const
		{
			//updateView();
			return m_qRealRot * FMath::ms_v3UnitX;
		}


    ////Movable
    public:
        virtual const String& GetMovableType() const;
		virtual float GetBoundingRadius() const;

    };

}; //LostPeter

#endif