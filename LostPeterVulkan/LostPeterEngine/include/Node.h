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

#ifndef _NODE_H_
#define _NODE_H_

#include "Base.h"

namespace LostPeterEngine
{
    class utilExport Node : public Base
    {
    public:
        Node();
        Node(const String& nameNode);
        virtual ~Node();


    public:
    protected:
        static uint32 ms_nNextID;

        static uint32 GetNextID() { return ++ms_nNextID; }
        static String GetNameAuto();

        static NodePtrVector ms_aNodeQueuedUpdates;		

    public:
    protected:
        Node* m_pParent;
        NodeListener* m_pNodeListener;

		NodePtrMap m_mapChild;
        mutable NodePtrSet m_setNeedToUpdate;
		mutable bool m_bNeedParentUpdate;	
		mutable bool m_bNeedChildUpdate;		
		mutable bool m_bParentNotified;
		mutable bool m_bQueuedForUpdate;

    protected:
	////Initial
		FVector3 m_vPosInitial;
		FQuaternion	m_qRotInitial;
		FVector3 m_vScaleInitial;

	////Local
        FVector3 m_vPosLocal;
		FQuaternion m_qRotLocal;
		FVector3 m_vScaleLocal;

		bool m_bRotIsInherit;
		bool m_bScaleIsInherit;

	////World
		mutable FVector3 m_vPosWorld;   
		mutable FQuaternion m_qRotWorld;
		mutable FVector3 m_vScaleWorld;

	////Transform Matrix4
		mutable FMatrix4 m_mat4TransformWorld;
		mutable bool m_bTransformWorldOutOfDate;

    public:
		LP_FORCEINLINE Node* GetParent() const	{ return m_pParent; }

		LP_FORCEINLINE NodeListener* GetNodeListener() const { return m_pNodeListener; }
		LP_FORCEINLINE void SetNodeListener(NodeListener* pNodeListener) { m_pNodeListener = pNodeListener; }

        LP_FORCEINLINE NodePtrMap& GetNodeMapChild() { return m_mapChild; }
		LP_FORCEINLINE const NodePtrMap& GetNodeMapChild() const { return m_mapChild; }

	public:
	////Initial
		LP_FORCEINLINE const FVector3& GetPositionInitial() const { return m_vPosInitial; }
		LP_FORCEINLINE const FQuaternion& GetRotationInitial() const { return m_qRotInitial; }
		LP_FORCEINLINE const FVector3&	GetScaleInitial() const	{ return m_vScaleInitial; }
		LP_FORCEINLINE void SetInitialState()
		{
			m_vPosInitial = m_vPosLocal;
			m_qRotInitial = m_qRotLocal;
			m_vScaleInitial	= m_vScaleLocal;
		}
		virtual void ResetToInitialState()
		{
			m_vPosLocal = m_vPosInitial;
			m_qRotLocal = m_qRotInitial;
			m_vScaleLocal = m_vScaleInitial;

			NeedUpdate();
		}

	////Local
        LP_FORCEINLINE const FVector3& GetPositionLocal() const { return m_vPosLocal; }
		LP_FORCEINLINE void SetPositionLocalOnly(const FVector3& vPosLocal)
		{
			m_vPosLocal = vPosLocal;
		}
		LP_FORCEINLINE void SetPositionLocal(const FVector3& vPosLocal)
		{
			SetPositionLocalOnly(vPosLocal);
			NeedUpdate();
		}
		LP_FORCEINLINE void SetPositionLocal(float x, float y, float z)
		{
			SetPositionLocal(FVector3(x, y, z));
		}

		LP_FORCEINLINE const FQuaternion& GetRotationLocal() const { return m_qRotLocal; }
		LP_FORCEINLINE void SetRotationLocalOnly(const FQuaternion& qRotLocal)
		{
			m_qRotLocal = FMath::Normalize(qRotLocal);
		}
		LP_FORCEINLINE void SetRotationLocal(const FQuaternion& qRotLocal)
		{
			SetRotationLocalOnly(qRotLocal);
			NeedUpdate();
		}
		LP_FORCEINLINE void SetRotationLocal(float x, float y, float z, float w)
		{
			SetRotationLocal(FQuaternion(x, y, z, w));
		}
		LP_FORCEINLINE void ResetRotationLocal()
		{
			m_qRotLocal = FMath::ms_qUnit;
			NeedUpdate();
		}

		LP_FORCEINLINE void SetAngleLocalOnly(const FVector3& vAngleLocal)
		{
			m_qRotLocal = FMath::ToQuaternion(vAngleLocal);
		}
		LP_FORCEINLINE void SetAngleLocal(const FVector3& vAngleLocal)
		{
			SetAngleLocalOnly(vAngleLocal);
			NeedUpdate();
		}
		LP_FORCEINLINE void SetAngleLocal(float x, float y, float z)
		{
			SetAngleLocal(FVector3(x, y, z));
		}


		LP_FORCEINLINE const FVector3& GetScaleLocal() const { return m_vScaleLocal; }
		LP_FORCEINLINE void SetScaleLocalOnly(const FVector3& vScaleLocal)
		{
			m_vScaleLocal = vScaleLocal;
		}
		LP_FORCEINLINE void SetScaleLocal(const FVector3& vScaleLocal)
		{
			SetScaleLocalOnly(vScaleLocal);
			NeedUpdate();
		}
		LP_FORCEINLINE void SetScaleLocal(float x, float y, float z)
		{
			SetScaleLocal(FVector3(x, y, z));
		}	


		LP_FORCEINLINE bool GetRotationIsInherit() const { return m_bRotIsInherit; }
		LP_FORCEINLINE void SetRotationIsInherit(bool bRotIsInherit)
		{
			m_bRotIsInherit = bRotIsInherit;
			NeedUpdate();
		}
		LP_FORCEINLINE bool GetScaleIsInherit() const { return m_bScaleIsInherit; }
		LP_FORCEINLINE void SetScaleIsInherit(bool bScaleIsInherit)
		{
			m_bScaleIsInherit = bScaleIsInherit;
			NeedUpdate();
		}

	////World
		virtual const FVector3&	GetPositionWorld() const;
		virtual void SetPositionWorld(const FVector3& vPosWorld);
		
		virtual const FQuaternion& GetRotationWorld() const;
		virtual void SetRotationWorld(const FQuaternion& qRotWorld);

		virtual const FVector3&	GetScaleWorld() const;

    public:
    public:
		virtual void Translate(const FVector3& vMove, FTransformType typeTransform = F_Transform_Parent);
		virtual void Translate(float x, float y, float z, FTransformType typeTransform = F_Transform_Parent);
		virtual void Translate(const FMatrix3& mat3, const FVector3& vMove, FTransformType typeTransform = F_Transform_Parent);
		virtual void Translate(const FMatrix3& mat3, float x, float y, float z, FTransformType typeTransform = F_Transform_Parent);

		virtual void Rotate(const FVector3& vAxis, float fAngle, FTransformType typeTransform = F_Transform_Local);
		virtual void Rotate(const FQuaternion& qRot, FTransformType typeTransform = F_Transform_Local);
		virtual void Roll(float fAngle, FTransformType typeTransform = F_Transform_Local);	//Z
		virtual void Pitch(float fAngle, FTransformType typeTransform = F_Transform_Local);	//X
		virtual void Yaw(float fAngle, FTransformType typeTransform = F_Transform_Local);	//Y

		virtual void Scale(const FVector3& vScale)
		{
			m_vScaleLocal = m_vScaleLocal * vScale;
			NeedUpdate();
		}
		virtual void Scale(float x, float y, float z)
		{
			m_vScaleLocal.x *= x;
			m_vScaleLocal.y *= y;
			m_vScaleLocal.z *= z;
			NeedUpdate();
		}
		
		virtual FMatrix3 GetLocalTransformMatrix3() const
		{
			FVector3 vAxisX = FMath::ms_v3UnitX;
			FVector3 vAxisY = FMath::ms_v3UnitY;
			FVector3 vAxisZ = FMath::ms_v3UnitZ;

			vAxisX = m_qRotLocal * vAxisX;
			vAxisY = m_qRotLocal * vAxisY;
			vAxisZ = m_qRotLocal * vAxisZ;

			return FMatrix3(vAxisX.x, vAxisY.x, vAxisZ.x,
						    vAxisX.y, vAxisY.y, vAxisZ.y,
						    vAxisX.z, vAxisY.z, vAxisZ.z);
		}
		
		virtual const FMatrix4&	GetWorldTransformMatrix4() const;

		virtual FVector3 ConvertPositionFromWorldToLocal(const FVector3& vPosWorld);
		virtual FVector3 ConvertPositionFromLocalToWorld(const FVector3& vPosLocal);

		virtual FQuaternion	ConvertRotationFromWorldToLocal(const FQuaternion& qRotWorld);
		virtual FQuaternion	ConvertRotationFromLocalToWorld(const FQuaternion& qRotLocal);

	public:
		virtual Node* CreateChild(const FVector3& vTranslate = FMath::ms_v3Zero, 
								  const FQuaternion& qRotate = FMath::ms_qUnit, 
								  const FVector3& vScale = FMath::ms_v3One);
		virtual Node* CreateChild(const String& strName, 
								  const FVector3& vTranslate = FMath::ms_v3Zero,
								  const FQuaternion& qRotate = FMath::ms_qUnit, 
								  const FVector3& vScale = FMath::ms_v3One);

		virtual Node* CreateChildByAngle(const FVector3& vTranslate = FMath::ms_v3Zero, 
										 const FVector3& vAngle = FMath::ms_v3Zero, 
										 const FVector3& vScale = FMath::ms_v3One);
		virtual Node* CreateChildByAngle(const String& strName, 
								  		 const FVector3& vTranslate = FMath::ms_v3Zero,
								  		 const FVector3& vAngle = FMath::ms_v3Zero, 
								  		 const FVector3& vScale = FMath::ms_v3One);

		virtual uint32 GetChildrenCount() const;
		virtual Node* GetChild(uint32 nIndex) const;    
		virtual Node* GetChild(const String& strName) const;

		virtual void AddChild(Node* pChild);
		virtual Node* RemoveChild(uint32 nIndex);
		virtual Node* RemoveChild(Node* pChild);
		virtual Node* RemoveChild(const String& strName);
		virtual void RemoveAllChildren();
    
	public:	
		virtual void Update(bool bUpdateChildren, bool bParentHasChanged);
		
		virtual void NeedUpdate(bool bForceParentUpdate = false);
		virtual void RequestUpdate(Node* pChild, bool bForceParentUpdate = false);
		virtual void CancelUpdate(Node* pChild);

		static void	QueueNeedUpdate(Node* pNode);
		static void	ProcessQueuedUpdates();

    protected:
		virtual void setParent(Node* pParent);
        virtual void updateFromParent() const;
		virtual void updateFromParentImpl() const;
		virtual Node* createChildImpl() = 0;
		virtual Node* createChildImpl(const String& strName) = 0;
    };

}; //LostPeterEngine

#endif