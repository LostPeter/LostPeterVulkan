/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-11-07
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _SCENE_DATA_SERIALIZER_H_
#define _SCENE_DATA_SERIALIZER_H_

#include "PreDefine.h"

namespace LostPeter
{
    class utilExport SceneDataSerializer
    {
    public:
        SceneDataSerializer(SceneDataManager* pSceneDataManager);
        virtual ~SceneDataSerializer();

    public:
        SceneDataManager* m_pSceneDataManager;

    public:
        bool Parser(uint32 nGroup, const String& strName, Scene* pScene, ScenePtrVector* pRet = nullptr);

		bool ParserXML(uint32 nGroup, const String& strName, ScenePtrVector* pRet = nullptr);
		bool ParserXML(const char* szFilePath, Scene* pScene = nullptr, ScenePtrVector* pRet = nullptr);

		bool ParserBinary(uint32 nGroup, const String& strName, ScenePtrVector* pRet = nullptr);
		bool ParserBinary(const char* szFilePath, Scene* pScene = nullptr, ScenePtrVector* pRet = nullptr);
        
    public:
        bool SaveXML(Scene* pScene);
		bool SaveXML(uint32 nGroup, Scene* pScene);
		bool SaveXML(const char* szFilePath, ScenePtrVector& aSA);
		
		bool SaveBinary(Scene* pScene);
		bool SaveBinary(uint32 nGroup, Scene* pScene);
		bool SaveBinary(const char* szFilePath, ScenePtrVector& aSA);
    };

}; //LostPeter

#endif