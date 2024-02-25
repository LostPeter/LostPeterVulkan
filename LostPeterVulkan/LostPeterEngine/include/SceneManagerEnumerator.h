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

#ifndef _SCENE_MANAGER_ENUMERATOR_H_
#define _SCENE_MANAGER_ENUMERATOR_H_

#include "Base.h"

namespace LostPeterEngine
{
    class engineExport SceneManagerEnumerator : public FSingleton<SceneManagerEnumerator>
                                              , public Base
    {
    public:
        SceneManagerEnumerator();
        virtual ~SceneManagerEnumerator();

    public:
        static const String ms_strSceneConfigName;

        static const String& GetSceneName_Default();

    public:
    protected:
        SceneSerializer* m_pSceneSerializer;
        ScenePtrVector m_aScene;
        SceneGroupPtrMap m_mapSceneGroup;
        ScenePtrMap m_mapSceneDefaults;

        SceneManagerPtrVector m_aSceneManagers;
        SceneManagerPtrMap m_mapSceneManagers;

    public:
        F_FORCEINLINE SceneSerializer* GetSceneSerializer() const { return m_pSceneSerializer; }
        F_FORCEINLINE const ScenePtrVector& GetScenePtrVector() const { return m_aScene; }
        F_FORCEINLINE ScenePtrVector& GetScenePtrVector() { return m_aScene; }
        F_FORCEINLINE const SceneGroupPtrMap& GetSceneGroupPtrMap() const { return m_mapSceneGroup; }
        F_FORCEINLINE SceneGroupPtrMap& GetSceneGroupPtrMap() { return m_mapSceneGroup; }
        F_FORCEINLINE ScenePtrMap& GetSceneDefaultMap() { return m_mapSceneDefaults; }

        F_FORCEINLINE const SceneManagerPtrVector& GetSceneManagerPtrVector() const { return this->m_aSceneManagers; }
        F_FORCEINLINE SceneManagerPtrVector& GetSceneManagerPtrVector() { return this->m_aSceneManagers; }
        F_FORCEINLINE const SceneManagerPtrMap& GetSceneManagerPtrMap() const { return this->m_mapSceneManagers; }
        F_FORCEINLINE SceneManagerPtrMap& GetSceneManagerPtrMap() { return this->m_mapSceneManagers; }

        bool IsSceneDefault(Scene* pScene);
        Scene* GetSceneDefault(const String& strName);
        Scene* GetSceneDefault(uint32 nGroup, const String& strName);
        Scene* GetScene_Default();

    public:
        static SceneManagerEnumerator& GetSingleton();
		static SceneManagerEnumerator* GetSingletonPtr();

    public:
        void Destroy();
        bool Init(uint32 nGroup, const String& strNameCfg);

    protected:
        void destroySceneDefaults();
        bool initSceneDefaults();

    public:
        bool LoadSceneAll();
        Scene* LoadScene(uint32 nGroup, const String& strName);

        bool HasScene(uint32 nGroup, const String& strName);
        Scene* GetScene(uint32 nGroup, const String& strName);
        bool AddScene(uint32 nGroup, Scene* pScene);
        void DeleteScene(uint32 nGroup, const String& strName);
        void DeleteSceneAll();

     private:
        Scene* loadScene(uint32 nGroup, const String& strName, bool bIsFromFile = true);
        Scene* loadScene(SceneInfo* pSI);

    protected:

    };

}; //LostPeterEngine

#endif