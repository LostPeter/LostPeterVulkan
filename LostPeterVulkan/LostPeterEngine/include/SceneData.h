/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-02-24
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _SCENE_DATA_H_
#define _SCENE_DATA_H_

#include "Resource.h"

namespace LostPeterEngine
{
    class engineExport SceneData : public Resource
                                 , public FNonCopyable
    {
    public:
        SceneData(ResourceManager* pResourceManager,
                  uint32 nGroup, 
                  const String& strName,
                  const String& strGroupName,
                  ResourceHandle nHandle,
                  bool bIsManualLoad = false,
                  ResourceManualLoader* pResourceManualLoader = nullptr);
        virtual ~SceneData();

    public:
        static const String ms_nameSceneData;

    public:
    protected:
        String m_strPath;

        SceneConfigSetting* m_pSceneConfigSetting;
        ObjectConfigPtrGroupMap m_mapObjectGroupConfig;
        SceneConfigNodePtrVector m_aSceneConfigNode;
        SceneConfigNodePtrMap m_mapSceneConfigNode;

        bool m_bInternalResourcesCreated;

    public:
        F_FORCEINLINE const String& GetPath() const { return m_strPath; }
        F_FORCEINLINE void SetPath(const String& path) { m_strPath = path; }

        F_FORCEINLINE SceneConfigSetting* GetSceneConfigSetting() const { return m_pSceneConfigSetting; }
        F_FORCEINLINE void SetSceneConfigSetting(SceneConfigSetting* pSceneConfigSetting) { m_pSceneConfigSetting = pSceneConfigSetting; }
        F_FORCEINLINE const ObjectConfigPtrGroupMap& GetObjectConfigPtrGroupMap() const { return m_mapObjectGroupConfig; }
        F_FORCEINLINE ObjectConfigPtrGroupMap& GetObjectConfigPtrGroupMap() { return m_mapObjectGroupConfig; }
        F_FORCEINLINE const SceneConfigNodePtrVector& GetSceneConfigNodePtrVector() const { return m_aSceneConfigNode; }
        F_FORCEINLINE SceneConfigNodePtrVector& GetSceneConfigNodePtrVector() { return m_aSceneConfigNode; }
        F_FORCEINLINE const SceneConfigNodePtrMap& GetSceneConfigNodePtrMap() const { return m_mapSceneConfigNode; }
        F_FORCEINLINE SceneConfigNodePtrMap& GetSceneConfigNodePtrMap() { return m_mapSceneConfigNode; }

    public:
        virtual void Destroy();

    public:
    ////ObjectConfig
        bool HasObjectConfig(EObjectType eObject, const String& strName);
        ObjectConfig* GetObjectConfig(EObjectType eObject, const String& strName);
        bool AddObjectConfig(EObjectType eObject, ObjectConfig* pObjectConfig);
        void DeleteObjectConfig(ObjectConfig* pObjectConfig);
        void DeleteObjectConfig(EObjectType eObject, const String& strName);
        void DeleteObjectConfigAll();

    ////SceneConfigNode
        bool HasSceneConfigNode(const String& strName);
        SceneConfigNode* GetSceneConfigNode(const String& strName);
        bool AddSceneConfigNode(SceneConfigNode* pSceneConfigNode);
        void DeleteSceneConfigNode(SceneConfigNode* pSceneConfigNode);
        void DeleteSceneConfigNode(const String& strName);
        void DeleteSceneConfigNodeAll();

    public:
        void SerializerFrom(const SceneData* pSceneData);

    protected:
        virtual void loadImpl();
		virtual void unloadImpl();
		virtual uint32 calculateSize() const;

    protected:
        virtual void destroyInternalResources();
			virtual void destroyInternalResourcesImpl();

		virtual bool createInternalResources();
			virtual void createInternalResourcesImpl();

    protected:
        virtual void addParameterBase();
        virtual void addParameterInherit();
    };

}; //LostPeterEngine

#endif