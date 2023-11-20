/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-11-07
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _SCENE_DATA_MANAGER_H_
#define _SCENE_DATA_MANAGER_H_

#include "Base.h"

namespace LostPeterEngine
{
    class utilExport SceneDataManager : public FSingleton<SceneDataManager>
                                      , public Base
    {
        friend class SceneDataSerializer;

    public:
        SceneDataManager();
        virtual ~SceneDataManager();

    public:
    protected:
        SceneSerializer* m_pSceneSerializer;
        SceneDataSerializer* m_pSceneDataSerializer;

        ScenePtrVector m_aScene;
        SceneGroupPtrMap m_mapSceneGroup;

    public:
        LP_FORCEINLINE SceneSerializer* GetSceneSerializer() const { return m_pSceneSerializer; }
        LP_FORCEINLINE SceneDataSerializer* GetSceneDataSerializer() const { return m_pSceneDataSerializer; }

        LP_FORCEINLINE const ScenePtrVector& GetScenePtrVector() const { return this->m_aScene; }
        LP_FORCEINLINE ScenePtrVector& GetScenePtrVector() { return this->m_aScene; }
        LP_FORCEINLINE const SceneGroupPtrMap& GetSceneGroupPtrMap() const { return this->m_mapSceneGroup; }
        LP_FORCEINLINE SceneGroupPtrMap& GetSceneGroupPtrMap() { return this->m_mapSceneGroup; }

    public:
        static SceneDataManager& GetSingleton();
		static SceneDataManager* GetSingletonPtr();

    public:
        void Destroy();
        bool Init(uint nGroup, const String& strNameCfg);

    public:
        bool LoadSceneAll();
        Scene* LoadScene(uint nGroup, const String& strName);
        void UnloadScene(Scene* pScene);

        bool HasScene(uint nGroup, const String& strName);
        Scene* GetScene(uint nGroup, const String& strName);
        bool AddScene(uint nGroup, Scene* pScene);
        void DeleteScene(uint nGroup, const String& strName);
        void DeleteSceneAll();

    private:
        Scene* loadScene(uint nGroup, const String& strName, bool bIsFromFile = true);
        Scene* loadScene(SceneInfo* pSI);


    public:
        bool Parser(uint32 nGroup, const String& strName, Scene* pScene);
        bool Parser(uint32 nGroup, const String& strName, ScenePtrVector* pRet = nullptr);

		bool ParserXML(uint32 nGroup, const String& strName, ScenePtrVector* pRet = nullptr);
		bool ParserXML(const char* szFilePath, ScenePtrVector* pRet = nullptr);

		bool ParserBinary(uint32 nGroup, const String& strName, ScenePtrVector* pRet = nullptr);
		bool ParserBinary(const char* szFilePath, ScenePtrVector* pRet = nullptr);

    public:
        bool SaveXML(Scene* pScene);
		bool SaveXML(uint32 nGroup, Scene* pScene);
		bool SaveXML(const char* szFilePath, ScenePtrVector& aSA);

		bool SaveBinary(Scene* pScene);
		bool SaveBinary(uint32 nGroup, Scene* pScene);
		bool SaveBinary(const char* szFilePath, ScenePtrVector& aSA);
    };

}; //LostPeterEngine

#endif