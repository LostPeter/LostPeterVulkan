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

#ifndef _SCENE_NODE_H_
#define _SCENE_NODE_H_

#include "Node.h"

namespace LostPeterEngine
{
    class engineExport SceneNode : public Node
    {
    public:
        SceneNode(Scene* pScene);
        SceneNode(const String& nameSceneNode, Scene* pScene);
        virtual ~SceneNode();

    public:
	protected:
		MovablePtrMap m_mapMovable;		
		Scene* m_pScene;

		FAABB m_aabbWorld;
		FAABB m_aabbChild;
		FSphere	m_spereWorld;
        bool m_bAABBSet;
		FAABB m_aabbSet;
		
		//WireBoundingBox* m_pWireBoundingBox;
		bool m_bShowBoundingBox;
		
		bool m_bYawFixed;			
		FVector3 m_vYawFixedAxis;		

		SceneNode* m_pAutoTrackTarget;		
		FVector3 m_vAutoTrackOffset;		
		FVector3 m_vAutoTrackLocalDirection;	
		bool m_bIsInSceneGraph;

		bool m_bVisible;
		bool m_bVisibleIsInherit;
		mutable bool m_bDerivedVisible;

    public:
		E_FORCEINLINE Scene* GetScene() const { return m_pScene; }
		SceneNode* GetParentSceneNode() const;
		
		E_FORCEINLINE const FAABB&	GetWorldAABB() const { return m_aabbWorld; }
		E_FORCEINLINE const FSphere& GetWorldSphere() const { return m_spereWorld; }
		E_FORCEINLINE bool	IsAABBSet()	{ return m_bAABBSet; }
		const FAABB& GetAABB();
		void SetAABB(bool bSetAABB, const FAABB& aabb);
		
		E_FORCEINLINE bool	GetShowBoundingBox() const { return m_bShowBoundingBox; }
		E_FORCEINLINE void	SetShowBoundingBox(bool bShow) { m_bShowBoundingBox = bShow; }

		//virtual void AddBoundingBoxToQueue(RenderQueue* pQueue);

		virtual void SetFixedYawAxis(bool bYawFixed, const FVector3& vFixedAxis = FMath::ms_v3UnitY)
		{
			m_bYawFixed	= bYawFixed;
			m_vYawFixedAxis	= vFixedAxis;
		}
		
		virtual SceneNode* GetAutoTrackTarget()	{ return m_pAutoTrackTarget; }
		virtual const FVector3&	GetAutoTrackOffset() { return m_vAutoTrackOffset; }
		virtual const FVector3&	GetAutoTrackLocalDirection() { return m_vAutoTrackLocalDirection; }

		virtual bool IsInSceneGraph() const	{ return m_bIsInSceneGraph; }
		virtual void NotifyRootNode() { m_bIsInSceneGraph = true; }
		virtual void SetInSceneGraph(bool bIsInGraph);
		
		virtual bool GetVisibleIsInherit() const { return m_bVisibleIsInherit; }
		virtual void SetVisibleIsInherit(bool bVisibleIsInherit) { m_bVisibleIsInherit = bVisibleIsInherit; }

		virtual bool IsVisible();
		virtual void SetVisible(bool bVisible);
		virtual void FlipVisibility(bool bCascade = true);
		
		virtual void SetDebugDisplayEnabled(bool bEnabled, bool bCascade = true);

	public:
		virtual uint32 GetAttachedObjectCount() const;
		virtual Movable* GetAttachedObject(uint32 nIndex);
		virtual Movable* GetAttachedObject(const String& strName);
		
		virtual void AttachObject(Movable* pMovable);
		virtual Movable* DetachObject(uint32 nIndex);
		virtual void DetachObject(Movable* pMovable);
		virtual Movable* DetachObject(const String& strName);
		virtual void DetachAllObjects();
    
		virtual SceneNode* CreateChildSceneNode(const FVector3& vTrans = FMath::ms_v3Zero, 
												const FQuaternion& qRot = FMath::ms_qUnit,
												const FVector3& vScale = FMath::ms_v3One);
		virtual SceneNode* CreateChildSceneNode(const String& strName, 
												const FVector3& vTrans = FMath::ms_v3Zero, 
												const FQuaternion& qRot = FMath::ms_qUnit,
												const FVector3& vScale = FMath::ms_v3One);

		virtual SceneNode* CreateChildSceneNodeByAngle(const FVector3& vTrans = FMath::ms_v3Zero, 
													   const FVector3& vAngle = FMath::ms_v3Zero,
													   const FVector3& vScale = FMath::ms_v3One);
		virtual SceneNode* CreateChildSceneNodeByAngle(const String& strName, 
													   const FVector3& vTrans = FMath::ms_v3Zero, 
													   const FVector3& vAngle = FMath::ms_v3Zero,
													   const FVector3& vScale = FMath::ms_v3One);
		
		virtual void RemoveAndDestroyChild(const String& strName);
		virtual void RemoveAndDestroyChild(uint32 nIndex);
		virtual void RemoveAndDestroyAllChildren();

		virtual void UpdateBounds();

		virtual void FindVisibleObjects(ObjectCamera* pCamera, 
                                        RenderQueue* pRenderQueue,
										ObjectVisibleBoundsInfo* pVisibleBounds,
										bool bOnlyShadowCasters = false,
										bool bIncludeChildren = true, 
										bool bDisplayNodes = false);

		virtual void SetDirection(float x, float y, float z, FTransformType typeTransform = F_Transform_Local, const FVector3& vLocalDir = FMath::ms_v3UnitNegZ);
		virtual void SetDirection(const FVector3& vDir, FTransformType typeTransform = F_Transform_Local, const FVector3& vLocalDir = FMath::ms_v3UnitNegZ);
		
		virtual void LookAt(const FVector3& vTarget, FTransformType typeTransform, const FVector3& vLocalDir = FMath::ms_v3UnitNegZ);

		virtual void SetAutoTracking(bool bEnabled, SceneNode* pTarget = nullptr, const FVector3& vLocalDir = FMath::ms_v3UnitNegZ, const FVector3& vOffset = FMath::ms_v3Zero);
		void AutoTrack();

	public:
		virtual void Yaw(float fAngle, FTransformType typeTransform = F_Transform_Local);
		virtual void Update(bool bUpdateChildren, bool bParentHasChanged);
		
	protected:
		virtual void mergeChildrenAABB();
		virtual void setParent(Node* pParent);
		virtual void updateFromParentImpl() const;
		virtual Node* createChildImpl();
		virtual Node* createChildImpl(const String& strName);
    };

}; //LostPeterEngine

#endif