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

#ifndef _SCENE_H_
#define _SCENE_H_

#include "Base.h"

namespace LostPeterEngine
{
    class engineExport Scene : public Base
    {
    public:
        Scene(uint _group, const String& nameScene);
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
        ObjectPtrGroupMap m_mapObjectGroups;
        ObjectCamera* m_pMainObjectCamera;
        ObjectLight* m_pMainObjectLight;

    public:
    public:
        void Destroy();

    public:
        virtual bool LoadScene();

        virtual void ClearScene();

    ////SceneManager/SceneNode
    public:
        E_FORCEINLINE const SceneNodePtrMap& GetSceneNodePtrMap() const { return this->m_mapSceneNodes; }
        E_FORCEINLINE SceneNodePtrMap& GetSceneNodePtrMap() { return this->m_mapSceneNodes; }

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
        E_FORCEINLINE const ViewportPtrMap& GetViewportPtrMap() const { return this->m_mapViewports; }
        E_FORCEINLINE ViewportPtrMap& GetViewportPtrMap() { return this->m_mapViewports; }



    ////Object
    public:
        E_FORCEINLINE const ObjectPtrGroupMap& GetObjectPtrGroupMap() const { return this->m_mapObjectGroups; }
        E_FORCEINLINE ObjectPtrGroupMap& GetObjectPtrGroupMap() { return this->m_mapObjectGroups; }
        ObjectPtrMap* GetObjectPtrMapByType(EObjectType type);
        ObjectPtrMap* GetObjectPtrMapByType(int type);

        virtual Object* GetObject(EObjectType type, const String& strName);
        virtual bool HasObject(EObjectType type, const String& strName); 
        virtual Object* DestroyObject(EObjectType type, const String& strName);
        virtual void DestroyObjectAll(EObjectType type);
        virtual void DestroyObjectAll();


        //ObjectMesh
        virtual ObjectMesh* GetObjectMesh(const String& strName);
		virtual bool HasObjectMesh(const String& strName);

		virtual ObjectMesh* CreateObjectMesh(const String& strName);

		virtual void DestroyObjectMesh(ObjectMesh* pObjectMesh);
		virtual void DestroyObjectMesh(const String& strName);
		virtual void DestroyObjectMeshAll();


        //ObjectSkinMesh
        virtual ObjectSkinMesh* GetObjectSkinMesh(const String& strName);
		virtual bool HasObjectSkinMesh(const String& strName);

		virtual ObjectSkinMesh* CreateObjectSkinMesh(const String& strName);

		virtual void DestroyObjectSkinMesh(ObjectSkinMesh* pObjectSkinMesh);
		virtual void DestroyObjectSkinMesh(const String& strName);
		virtual void DestroyObjectSkinMeshAll();


        //ObjectCamera
        E_FORCEINLINE ObjectCamera* GetMainObjectCamera() const { return m_pMainObjectCamera; }

        virtual ObjectCamera* GetObjectCamera(const String& strName);
		virtual bool HasObjectCamera(const String& strName);

		virtual ObjectCamera* CreateObjectCamera(const String& strName, SceneManager* pSceneManager, bool bMainCamera = false);

		virtual void DestroyObjectCamera(ObjectCamera* pObjectCamera);
		virtual void DestroyObjectCamera(const String& strName);
		virtual void DestroyObjectCameraAll();


        //ObjectLight
        E_FORCEINLINE ObjectLight* GetMainObjectLight() const { return m_pMainObjectLight; }

        virtual ObjectLight* GetObjectLight(const String& strName);
		virtual bool HasObjectLight(const String& strName);

		virtual ObjectLight* CreateObjectLight(const String& strName, bool bMainLight = false);

		virtual void DestroyObjectLight(ObjectLight* pObjectLight);
		virtual void DestroyObjectLight(const String& strName);
		virtual void DestroyObjectLightAll();


        //ObjectTerrain
        virtual ObjectTerrain* GetObjectTerrain(const String& strName);
		virtual bool HasObjectTerrain(const String& strName);

		virtual ObjectTerrain* CreateObjectTerrain(const String& strName);

		virtual void DestroyObjectTerrain(ObjectTerrain* pObjectTerrain);
		virtual void DestroyObjectTerrain(const String& strName);
		virtual void DestroyObjectTerrainAll();


        //ObjectWater
        virtual ObjectWater* GetObjectWater(const String& strName);
		virtual bool HasObjectWater(const String& strName);

		virtual ObjectWater* CreateObjectWater(const String& strName);

		virtual void DestroyObjectWater(ObjectWater* pObjectWater);
		virtual void DestroyObjectWater(const String& strName);
		virtual void DestroyObjectWaterAll();


        //ObjectSky
        virtual ObjectSky* GetObjectSky(const String& strName);
		virtual bool HasObjectSky(const String& strName);

		virtual ObjectSky* CreateObjectSky(const String& strName);

		virtual void DestroyObjectSky(ObjectSky* pObjectSky);
		virtual void DestroyObjectSky(const String& strName);
		virtual void DestroyObjectSkyAll();


        //ObjectCloud
        virtual ObjectCloud* GetObjectCloud(const String& strName);
		virtual bool HasObjectCloud(const String& strName);

		virtual ObjectCloud* CreateObjectCloud(const String& strName);

		virtual void DestroyObjectCloud(ObjectCloud* pObjectCloud);
		virtual void DestroyObjectCloud(const String& strName);
		virtual void DestroyObjectCloudAll();


        //ObjectParticle
        virtual ObjectParticle* GetObjectParticle(const String& strName);
		virtual bool HasObjectParticle(const String& strName);

		virtual ObjectParticle* CreateObjectParticle(const String& strName);

		virtual void DestroyObjectParticle(ObjectParticle* pObjectParticle);
		virtual void DestroyObjectParticle(const String& strName);
		virtual void DestroyObjectParticleAll();

    };

}; //LostPeterEngine

#endif