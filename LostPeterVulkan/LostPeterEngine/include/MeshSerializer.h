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

#ifndef _MESH_SERIALIZER_H_
#define _MESH_SERIALIZER_H_

#include "Base.h"

namespace LostPeterEngine
{
    class engineExport MeshSerializer : public FSerializer
                                      , public Base
    {
    public:
        MeshSerializer();
        virtual ~MeshSerializer();

    public:
    protected:
        MeshInfoPtrVector m_aMeshInfo;
        MeshInfoGroupPtrMap m_mapMeshInfoGroup;

    public:
        F_FORCEINLINE const MeshInfoPtrVector& GetMeshInfoPtrVector() const { return m_aMeshInfo; }
        F_FORCEINLINE MeshInfoPtrVector& GetMeshInfoPtrVector() { return m_aMeshInfo; }
        F_FORCEINLINE const MeshInfoGroupPtrMap& GetMeshInfoGroupPtrMap() const { return m_mapMeshInfoGroup; }
        F_FORCEINLINE MeshInfoGroupPtrMap& GetMeshInfoGroupPtrMap() { return m_mapMeshInfoGroup; }
        

    public:
        bool HasMeshInfo(uint nGroup, const String& strName);
        MeshInfo* GetMeshInfo(uint nGroup, const String& strName);
        bool AddMeshInfo(MeshInfo* pMeshInfo);
        void DeleteMeshInfo(uint nGroup, const String& strName);
        void DeleteMeshInfoAll();

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