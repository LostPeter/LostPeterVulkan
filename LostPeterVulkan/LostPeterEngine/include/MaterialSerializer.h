/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-06
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _MATERIAL_SERIALIZER_H_
#define _MATERIAL_SERIALIZER_H_

#include "Base.h"

namespace LostPeterEngine
{
    class engineExport MaterialSerializer : public FSerializer
                                          , public Base
    {
    public:
        MaterialSerializer();
        virtual ~MaterialSerializer();

    public:
    protected:
        MaterialInfoPtrVector m_aMaterialInfo;
        MaterialInfoGroupPtrMap m_mapMaterialInfoGroup;

    public:
        F_FORCEINLINE const MaterialInfoPtrVector& GetMaterialInfoPtrVector() const { return m_aMaterialInfo; }
        F_FORCEINLINE MaterialInfoPtrVector& GetMaterialInfoPtrVector() { return m_aMaterialInfo; }
        F_FORCEINLINE const MaterialInfoGroupPtrMap& GetMaterialInfoGroupPtrMap() const { return m_mapMaterialInfoGroup; }
        F_FORCEINLINE MaterialInfoGroupPtrMap& GetMaterialInfoGroupPtrMap() { return m_mapMaterialInfoGroup; }


    public:
        bool HasMaterialInfo(uint nGroup, const String& strName);
        MaterialInfo* GetMaterialInfo(uint nGroup, const String& strName);
        bool AddMaterialInfo(MaterialInfo* pMaterialInfo);
        void DeleteMaterialInfo(uint nGroup, const String& strName);
        void DeleteMaterialInfoAll();

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