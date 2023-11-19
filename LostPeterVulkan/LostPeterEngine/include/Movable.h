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

#ifndef _MOVABLE_H_
#define _MOVABLE_H_

#include "Base.h"

namespace LostPeter
{
    class utilExport Movable : public Base
    {
    public:
        Movable(const String& nameMovable, Scene* pScene);
        virtual ~Movable();

    public:
        static uint32 GetQueryFlags_Default() { return ms_nQueryFlags_Default; }
		static void	SetQueryFlags_Default(uint32 nFlags) { ms_nQueryFlags_Default = nFlags; }
        static uint32 GetVisibilityFlags_Default() { return ms_nVisibilityFlags_Default; }
		static void	SetVisibilityFlags_Default(uint32 nFlags) { ms_nVisibilityFlags_Default = nFlags; }

    protected:
		static uint32 ms_nQueryFlags_Default;
		static uint32 ms_nVisibilityFlags_Default;

    public:
    protected:
        Scene* m_pScene;	
        MovableListener* m_pMovableListener;
        MovableFactory* m_pMovableFactory;
		Node* m_pParentNode;

        uint32 m_nQueryFlags;	
		uint32 m_nVisibilityFlags;	

		bool m_bParentIsTagPoint;
		bool m_bVisible;					
		bool m_bDebugDisplay;		
        bool m_bBeyondFarDistance;	
        bool m_bRenderingDisabled;

		float m_fUpperDistance;				
		float m_fSquaredUpperDistance;	

		mutable FAABB m_aabbWorld;					
		mutable FSphere	m_sphereWorld;			
        
    public:
        virtual Scene* GetScene() const { return m_pScene; }
        virtual MovableListener* GetMovableListener() const { return m_pMovableListener; }
		virtual void SetMovableListener(MovableListener* pMovableListener) { m_pMovableListener = pMovableListener; }
        virtual MovableFactory* GetMovableFactory() const { return m_pMovableFactory; }
		virtual void SetMovableFactory(MovableFactory* pMovableFactory)	{ m_pMovableFactory = pMovableFactory; }
        virtual Node* GetParentNode() const	{ return m_pParentNode; }
		virtual SceneNode* GetParentSceneNode() const;

        virtual uint32 GetTypeFlags() const;

        virtual uint32 GetQueryFlags() const { return m_nQueryFlags; }
		virtual void SetQueryFlags(uint32 nFlags) { m_nQueryFlags = nFlags; }
		virtual void AddQueryFlags(uint32 nFlags) { m_nQueryFlags |= nFlags; }
		virtual void RemoveQueryFlags(uint32 nFlags) { m_nQueryFlags &= ~nFlags; }
		
		virtual uint32 GetVisibilityFlags() const { return m_nVisibilityFlags; }
		virtual void SetVisibilityFlags(uint32 nFlags) { m_nVisibilityFlags = nFlags; }
		virtual void AddVisibilityFlags(uint32 nFlags) { m_nVisibilityFlags |= nFlags; }
		virtual void RemoveVisibilityFlags(uint32 nFlags) { m_nVisibilityFlags &= ~nFlags; }

		virtual bool GetVisible() const	{ return m_bVisible; }
		virtual void SetVisible(bool bVisible) { m_bVisible = bVisible; }
        virtual bool IsVisible() const;

		virtual bool IsDebugDisplayEnabled() const { return m_bDebugDisplay; }
		virtual void SetDebugDisplayEnabled(bool enabled) { m_bDebugDisplay = enabled; }

		virtual float GetRenderingDistance() const { return m_fUpperDistance; }
		virtual void SetRenderingDistance(float fDist)
		{ 
			m_fUpperDistance = fDist; 
			m_fSquaredUpperDistance = fDist * fDist;
		}

        virtual bool IsAttached() const	{ return m_pParentNode != nullptr; }
		virtual void NotifyAttached(Node* pParent, bool bIsTagPoint = false);
		virtual void NotifyMoved();
		virtual void DetachFromParent();

		virtual bool IsInScene() const;
		virtual void NotifyCurrentCamera(ObjectCamera* pCamera);
		virtual const FMatrix4& GetNodeParentFullTransform() const;

		virtual const FAABB& GetBoundingAABBWorld(bool isDerive = false) const;
		virtual const FSphere& GetBoundingSphereWorld(bool isDerive = false) const;
		
	public:
		virtual const String& GetMovableType() const = 0;
		virtual const FAABB& GetBoundingAABB() const = 0;
		virtual float GetBoundingRadius() const = 0;
    };

}; //LostPeter

#endif