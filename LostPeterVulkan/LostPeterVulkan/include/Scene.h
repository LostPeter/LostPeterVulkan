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

#ifndef _SCENE_H_
#define _SCENE_H_

#include "Base.h"

namespace LostPeter
{
    class utilExport Scene : public Base
    {
    public:
        Scene(uint _group, 
              const String& nameScene);
        virtual ~Scene();

    public:
        static String ms_nameRootSceneNode;

    public:
    ////SceneManager/SceneNode
        SceneManager* m_pSceneManager;
        SceneNodePtrMap m_mapSceneNodes;
        SceneNodePtrSet	m_setAutoTrackingSceneNodes;
        SceneNode* m_pRootSceneNode;

    ////Viewport
        ViewportPtrMap m_mapViewports;
        Viewport* m_pViewportMain;

    ////Object
        ObjectPtrVector m_aObjects;
        ObjectPtrMap m_mapObjects;

    public:
    public:
        void Destroy();

    public:
        bool LoadScene();
		void UnloadScene();    

    ////SceneManager/SceneNode
    public:
        LP_FORCEINLINE const SceneNodePtrMap& GetSceneNodePtrMap() const { return this->m_mapSceneNodes; }
        LP_FORCEINLINE SceneNodePtrMap& GetSceneNodePtrMap() { return this->m_mapSceneNodes; }

        virtual SceneNode* CreateRootSceneNode(const String& strName, 
                                               const FVector3& vTrans = FMath::ms_v3Zero, 
											   const FQuaternion& qRot = FMath::ms_qUnit,
											   const FVector3& vScale = FMath::ms_v3One);
        virtual SceneNode* CreateRootSceneNode(const String& strName, 
                                               const FVector3& vTrans = FMath::ms_v3Zero, 
											   const FVector3& vAngle = FMath::ms_v3Zero,
											   const FVector3& vScale = FMath::ms_v3One);
        virtual SceneNode* GetRootSceneNode();
		
		virtual SceneNode* GetSceneNode(const String& strName) const;
		virtual bool HasSceneNode(const String& strName) const;
        virtual bool AddSceneNode(SceneNode* pSceneNode);

		virtual SceneNode* CreateSceneNode();
		virtual SceneNode* CreateSceneNode(const String& strName);
        virtual SceneNode* CreateSceneNode(const String& strName, SceneNode* pParent);

		virtual void DestroySceneNode(const String& strName);
		virtual void DestroySceneNode(SceneNode* pSceneNode);
		
		virtual void NotifyAutoTrackingSceneNode(SceneNode* pSceneNode, bool bAutoTrack);

    protected:
		virtual SceneNode* createSceneNodeImpl();
		virtual SceneNode* createSceneNodeImpl(const String& strName);

    ////Viewport
    public:
        LP_FORCEINLINE const ViewportPtrMap& GetViewportPtrMap() const { return this->m_mapViewports; }
        LP_FORCEINLINE ViewportPtrMap& GetViewportPtrMap() { return this->m_mapViewports; }



    ////Object
    public:
        LP_FORCEINLINE const ObjectPtrVector& GetObjectPtrVector() const { return this->m_aObjects; }
        LP_FORCEINLINE ObjectPtrVector& GetObjectPtrVector() { return this->m_aObjects; }
        LP_FORCEINLINE const ObjectPtrMap& GetObjectPtrMap() const { return this->m_mapObjects; }
        LP_FORCEINLINE ObjectPtrMap& GetObjectPtrMap() { return this->m_mapObjects; }



    };

}; //LostPeter

#endif