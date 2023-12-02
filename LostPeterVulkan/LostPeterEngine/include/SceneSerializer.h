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

#ifndef _SCENE_SERIALIZER_H_
#define _SCENE_SERIALIZER_H_

#include "Base.h"

namespace LostPeterEngine
{
    class engineExport SceneSerializer : public FSerializer
                                       , public Base
    {
    public:
        SceneSerializer();
        virtual ~SceneSerializer();

    public:
    protected:
        SceneInfoPtrVector m_aSceneInfo;
        SceneInfoGroupPtrMap m_mapSceneInfoGroup;

    public:
        E_FORCEINLINE const SceneInfoPtrVector& GetSceneInfoPtrVector() const { return m_aSceneInfo; }
        E_FORCEINLINE SceneInfoPtrVector& GetSceneInfoPtrVector() { return m_aSceneInfo; }
        E_FORCEINLINE const SceneInfoGroupPtrMap& GetSceneInfoGroupPtrMap() const { return m_mapSceneInfoGroup; }
        E_FORCEINLINE SceneInfoGroupPtrMap& GetSceneInfoGroupPtrMap() { return m_mapSceneInfoGroup; }


    public:
        bool HasSceneInfo(uint nGroup, const String& strName);
        SceneInfo* GetSceneInfo(uint nGroup, const String& strName);
        bool AddSceneInfo(SceneInfo* pSceneInfo);
        void DeleteSceneInfo(uint nGroup, const String& strName);
        void DeleteSceneInfoAll();

    protected:
        //File Content XML
        virtual bool serializeXML();
        virtual bool deserializeXML();

        //File Content Binary
        virtual bool serializeBinary();
        virtual bool deserializeBinary();
    };

}; //LostPeterEngine

#endif