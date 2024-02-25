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

#include "ResourceManager.h"

namespace LostPeterEngine
{
    class engineExport SceneDataManager : public FSingleton<SceneDataManager>
                                        , public ResourceManager
    {
        friend class SceneDataSerializer;

    public:
        SceneDataManager();
        virtual ~SceneDataManager();

    public:
    protected:
        SceneDataSerializer* m_pSceneDataSerializer;

    public:
        F_FORCEINLINE SceneDataSerializer* GetSceneDataSerializer() const { return m_pSceneDataSerializer; }

    public:
        static SceneDataManager& GetSingleton();
		static SceneDataManager* GetSingletonPtr();

    public:
        virtual void Destroy();

    public:
        SceneData* NewSceneData(uint32 nGroup, const String& strName, const String& strGroupName = ResourceGroupManager::ms_strNameResourceGroup_AutoDetect);
		bool AddSceneData(SceneData* pSceneData);
		
		SceneData* LoadSceneData(uint32 nGroup, const String& strName, bool bIsFromFile, const String& strGroupName = ResourceGroupManager::ms_strNameResourceGroup_AutoDetect);
        void UnloadSceneData(SceneData* pSceneData);

        bool HasSceneData(const String& strName);
        bool HasSceneData(const String& strName, const String& strGroupName);
        SceneData* GetSceneData(const String& strName);
        SceneData* GetSceneData(const String& strName, const String& strGroupName);

    public:
		virtual ResourceCreateOrRetrieveResult CreateOrRetrieveSceneData(const String& strPath,
                                                                         uint32 nGroup, 
                                                                         const String& strName, 
                                                                         const String& strGroupName, 
                                                                         bool bIsManualLoad = false,
                                                                         ResourceManualLoader* pManualLoader = nullptr, 
                                                                         const NameValuePairMap* pLoadParams = nullptr);

    public:
        virtual SceneData* Prepare(const String& strPath,
								   uint32 nGroup, 
								   const String& strName, 
								   const String& strGroupName);

    public:
        virtual SceneData* CreateSceneData(const String& strPath,
                                           uint32 nGroup, 
                                           const String& strName, 
                                           const String& strGroupName);

	protected:
        virtual Resource* createImpl(uint32 nGroup,
									 const String& strName,
			                         const String& strGroupName,
									 ResourceHandle nHandle, 
                                     bool bIsManualLoad,
                                     ResourceManualLoader* pManualLoader, 
			                         const NameValuePairMap* pLoadParams);

    public:
        bool Parser(uint32 nGroup, const String& strName, SceneData* pSceneData);
        bool Parser(uint32 nGroup, const String& strName, SceneDataPtrVector* pRet = nullptr);

		bool ParserXML(uint32 nGroup, const String& strName, SceneDataPtrVector* pRet = nullptr);
		bool ParserXML(uint32 nGroup, const String& strName, const String& strPath, SceneDataPtrVector* pRet = nullptr);

		bool ParserBinary(uint32 nGroup, const String& strName, SceneDataPtrVector* pRet = nullptr);
		bool ParserBinary(uint32 nGroup, const String& strName, const String& strPath, SceneDataPtrVector* pRet = nullptr);

    public:
        bool SaveXML(Scene* pScene);
		bool SaveXML(uint32 nGroup, Scene* pScene);
		bool SaveXML(const String& strPath, ScenePtrVector& aSA);

		bool SaveBinary(Scene* pScene);
		bool SaveBinary(uint32 nGroup, Scene* pScene);
		bool SaveBinary(const String& strPath, ScenePtrVector& aSA);
    };

}; //LostPeterEngine

#endif