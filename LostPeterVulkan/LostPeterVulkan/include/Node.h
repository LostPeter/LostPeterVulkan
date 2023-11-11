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

#ifndef _NODE_H_
#define _NODE_H_

#include "Base.h"

namespace LostPeter
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
        FVector3 m_vPos;
		FVector3 m_vScale;
		FQuaternion m_qRot;
		bool m_bInheritRot;
		bool m_bInheritScale;

		mutable FVector3 m_vDerivedPos;   
		mutable FVector3 m_vDerivedScale;
		mutable FQuaternion m_qDerivedRot;

		FVector3 m_vInitialPos;
		FVector3 m_vInitialScale;
		FQuaternion	m_qInitialRot;

		mutable FMatrix4 m_matCachedTransform;
		mutable bool m_bCachedTransformOutOfDate;


    public:
		virtual Node* GetParent() const	{ return m_pParent; }

		virtual NodeListener* GetNodeListener() const { return m_pNodeListener; }
		virtual void SetNodeListener(NodeListener* pNodeListener) { m_pNodeListener = pNodeListener; }

        LP_FORCEINLINE NodePtrMap& GetNodeMapChild() { return m_mapChild; }
		LP_FORCEINLINE const NodePtrMap& GetNodeMapChild() const { return m_mapChild; }


        virtual const FVector3& GetPosition() const	{ return m_vPos; }
		virtual void SetPosition(const FVector3& vPos)
		{
			m_vPos = vPos;
			NeedUpdate();
		}
		virtual void SetPosition(float x, float y, float z)
		{
			SetPosition(FVector3(x, y, z));
		}

		virtual const FVector3& GetScale() const { return m_vScale; }
		virtual void SetScale(const FVector3& vScale)
		{
			m_vScale = vScale;
			NeedUpdate();
		}
		virtual void SetScale(float x, float y, float z)
		{
			SetScale(FVector3(x, y, z));
		}	

		virtual const FQuaternion& GetOrientation() const { return m_qRot; }
		virtual void SetOrientation(const FQuaternion& qRot)
		{
			m_qRot = FMath::Normalize(qRot);
			NeedUpdate();
		}
		virtual void SetOrientation(float x, float y, float z, float w)
		{
			SetOrientation(FQuaternion(x, y, z, w));
		}
		virtual void ResetOrientation()
		{
			m_qRot = FMath::ms_qUnit;
			NeedUpdate();
		}

		virtual bool GetInheritOrientation() const { return m_bInheritRot; }
		virtual void SetInheritOrientation(bool bInherit)
		{
			m_bInheritRot = bInherit;
			NeedUpdate();
		}
		virtual bool GetInheritScale() const { return m_bInheritScale; }
		virtual void SetInheritScale(bool bInherit)
		{
			m_bInheritScale = bInherit;
			NeedUpdate();
		}

		virtual void SetInitialState()
		{
			m_vInitialPos = m_vPos;
			m_qInitialRot = m_qRot;
			m_vInitialScale	= m_vScale;
		}

		virtual void ResetToInitialState()
		{
			m_vPos = m_vInitialPos;
			m_qRot = m_qInitialRot;
			m_vScale = m_vInitialScale;

			NeedUpdate();
		}

		virtual const FVector3&	GetInitialPosition() const { return m_vInitialPos; }
		virtual const FQuaternion&	GetInitialOrientation() const { return m_qInitialRot; }
		virtual const FVector3&	GetInitialScale() const	{ return m_vInitialScale; }

		virtual const FVector3&	GetDerivedPosition() const;
		virtual void SetDerivedPosition(const FVector3& vPos);
		
		virtual const FQuaternion&	GetDerivedOrientation() const;
		virtual void SetDerivedOrientation(const FQuaternion& qRot);

		virtual const FVector3&	GetDerivedScale() const;

    public:
    public:
		virtual void Scale(const FVector3& vScale)
		{
			m_vScale = m_vScale * vScale;
			NeedUpdate();
		}
		virtual void Scale(float x, float y, float z)
		{
			m_vScale.x *= x;
			m_vScale.y *= y;
			m_vScale.z *= z;
			NeedUpdate();
		}

		virtual void Translate(const FVector3& vDis, FTransformType typeTransform = F_Transform_Parent);
		virtual void Translate(float x, float y, float z, FTransformType typeTransform = F_Transform_Parent);
		virtual void Translate(const FMatrix3& mat3Axes, const FVector3& vMove, FTransformType typeTransform = F_Transform_Parent);
		virtual void Translate(const FMatrix3& mat3Axes, float x, float y, float z, FTransformType typeTransform = F_Transform_Parent);

		virtual void Roll(float fAngle, FTransformType typeTransform = F_Transform_Local);	//Z
		virtual void Pitch(float fAngle, FTransformType typeTransform = F_Transform_Local);	//X
		virtual void Yaw(float fAngle, FTransformType typeTransform = F_Transform_Local);	//Y

		virtual void Rotate(const FVector3& vAxis, float fAngle, FTransformType typeTransform = F_Transform_Local);
		virtual void Rotate(const FQuaternion& qRot, FTransformType typeTransform = F_Transform_Local);
		
		virtual FMatrix3 GetLocalAxes() const
		{
			FVector3 vAxisX = FMath::ms_v3UnitX;
			FVector3 vAxisY = FMath::ms_v3UnitY;
			FVector3 vAxisZ = FMath::ms_v3UnitZ;

			vAxisX = m_qRot * vAxisX;
			vAxisY = m_qRot * vAxisY;
			vAxisZ = m_qRot * vAxisZ;

			return FMatrix3(vAxisX.x, vAxisY.x, vAxisZ.x,
						    vAxisX.y, vAxisY.y, vAxisZ.y,
						    vAxisX.z, vAxisY.z, vAxisZ.z);
		}
		
		virtual const FMatrix4&	GetFullTransform() const;

		virtual FVector3 ConvertWorldToLocalPosition(const FVector3& vPosWorld);
		virtual FVector3 ConvertLocalToWorldPosition(const FVector3& vPosLocal);

		virtual FQuaternion	ConvertWorldToLocalOrientation(const FQuaternion& qRotWorld);
		virtual FQuaternion	ConvertLocalToWorldOrientation(const FQuaternion& qRotLocal);

	public:
		virtual Node* CreateChild(const FVector3& vTranslate = FMath::ms_v3Zero, const FQuaternion& qRotate = FMath::ms_qUnit);
		virtual Node* CreateChild(const String& strName, const FVector3& vTranslate = FMath::ms_v3Zero, const FQuaternion& qRotate = FMath::ms_qUnit);

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

}; //LostPeter

#endif