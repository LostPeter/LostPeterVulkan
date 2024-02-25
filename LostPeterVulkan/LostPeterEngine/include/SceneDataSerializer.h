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

#ifndef _SCENE_DATA_SERIALIZER_H_
#define _SCENE_DATA_SERIALIZER_H_

#include "PreDefine.h"

namespace LostPeterEngine
{
    class engineExport SceneDataSerializer
    {
    public:
        SceneDataSerializer(SceneDataManager* pSceneDataManager);
        virtual ~SceneDataSerializer();

    public:
        SceneDataManager* m_pSceneDataManager;

    public:
        bool Parser(uint32 nGroup, const String& strName, SceneData* pSceneData, SceneDataPtrVector* pRet = nullptr);

		bool ParserXML(uint32 nGroup, const String& strName, SceneDataPtrVector* pRet = nullptr);
		bool ParserXML(uint32 nGroup, const String& strName, const String& strPath, SceneData* pSceneData = nullptr, SceneDataPtrVector* pRet = nullptr);

		bool ParserBinary(uint32 nGroup, const String& strName, SceneDataPtrVector* pRet = nullptr);
		bool ParserBinary(uint32 nGroup, const String& strName, const String& strPath, SceneData* pSceneData = nullptr, SceneDataPtrVector* pRet = nullptr);
        
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