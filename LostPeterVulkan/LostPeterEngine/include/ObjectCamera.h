/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
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


namespace LostPeterEngine
{
    class engineExport ObjectCamera : public ObjectFrustum
    {
    public:
        ObjectCamera(const String& nameCamera, SceneManager* pSceneManager);
        virtual ~ObjectCamera();

    public:
    protected:
        static const String	ms_strMovableType;	


    public:
    protected:
        ObjectVisibleBoundsInfo* m_pObjectVisibleBoundsInfo;

	////Local
        FVector3 m_vPosLocal;	
		FQuaternion	m_qRotLocal;				
			
	////World
		mutable FVector3 m_vPosWorld;			
		mutable FQuaternion	m_qRotWorld;	
		
	////Real
		mutable FVector3 m_vPosReal;
		mutable FQuaternion	m_qRotReal;		


		Viewport* m_pViewport;	

    public:
        F_FORCEINLINE ObjectVisibleBoundsInfo* GetObjectVisibleBoundsInfo() { return m_pObjectVisibleBoundsInfo; }


	////Local
        F_FORCEINLINE const FVector3& GetPositionLocal() const { return m_vPosLocal; }
		F_FORCEINLINE void SetPositionLocal(float x, float y, float z)
		{
			m_vPosLocal.x = x;
			m_vPosLocal.y = y;
			m_vPosLocal.z = z;
			//invalidateView();
		}
		F_FORCEINLINE void SetPositionLocal(const FVector3& vPosLocal)
		{
			m_vPosLocal = vPosLocal;
			//invalidateView();
		}

		F_FORCEINLINE void Move(const FVector3& vDelta)
		{
			m_vPosLocal = m_vPosLocal + vDelta;
			//invalidateView();
		}
		F_FORCEINLINE void MoveRelative(const FVector3& vMove)
		{
			FVector3 vTrans = m_qRotLocal * vMove;
			m_vPosLocal = m_vPosLocal + vTrans;
			//invalidateView();
		}
		

		F_FORCEINLINE const FQuaternion& GetRotationLocal() const { return m_qRotLocal; }
		F_FORCEINLINE void SetRotationLocal(const FQuaternion& qRotLocal)
		{
			m_qRotLocal = FMath::Normalize(qRotLocal);
			//invalidateView();
		}

		F_FORCEINLINE FVector3 GetDirectionLocal() const { return m_qRotLocal * FMath::ms_v3UnitNegZ; }
		void SetDirectionLocal(const FVector3& vDirLocal);
		F_FORCEINLINE void SetDirectionLocal(float x, float y, float z)
		{
			SetDirectionLocal(FVector3(x, y, z));
		}
		F_FORCEINLINE FVector3 GetUpLocal() const { return m_qRotLocal * FMath::ms_v3UnitY; }
		F_FORCEINLINE FVector3 GetRightLocal() const { return m_qRotLocal * FMath::ms_v3UnitX; }

		
	////World
		F_FORCEINLINE const FVector3& GetPositionWorld() const
		{
			//updateView();
			return m_vPosWorld;
		}
		F_FORCEINLINE const FQuaternion& GetRotationWorld() const
		{
			//updateView();
			return m_qRotWorld;
		}

		F_FORCEINLINE FVector3 GetDirectionWorld() const
		{
			//updateView();
			return m_qRotWorld * FMath::ms_v3UnitNegZ;
		}
		F_FORCEINLINE FVector3 GetUpWorld() const
		{
			//updateView();
			return m_qRotWorld * FMath::ms_v3UnitY;
		}
		F_FORCEINLINE FVector3 GetRightWorld() const
		{
			//updateView();
			return m_qRotWorld * FMath::ms_v3UnitX;
		}


	////Real
		F_FORCEINLINE const FVector3& GetPositionReal() const
		{
			//updateView();
			return m_vPosReal;
		}
		F_FORCEINLINE const FQuaternion& GetRotationReal() const
		{
			//updateView();
			return m_qRotReal;
		}

		F_FORCEINLINE FVector3 GetDirectionReal() const
		{
			//updateView();
			return m_qRotReal * FMath::ms_v3UnitNegZ;
		}
		F_FORCEINLINE FVector3 GetUpReal() const
		{
			//updateView();
			return m_qRotReal * FMath::ms_v3UnitY;
		}
		F_FORCEINLINE FVector3 GetRightReal() const
		{
			//updateView();
			return m_qRotReal * FMath::ms_v3UnitX;
		}


		F_FORCEINLINE Viewport* GetViewport() const { return m_pViewport; }
		F_FORCEINLINE void	NotifyViewport(Viewport* pViewport)	{ m_pViewport = pViewport; }

		

	public:
		virtual const FMatrix4& GetViewMatrix() const;
		virtual const FMatrix4&	GetViewMatrix(bool bOwnFrustumOnly) const;

		virtual void RenderScene(Viewport* pViewport, bool bIncludeOverlays);


    ////Movable
    public:
        virtual const String& GetMovableType() const;
		virtual float GetBoundingRadius() const;
    };

}; //LostPeterEngine

#endif