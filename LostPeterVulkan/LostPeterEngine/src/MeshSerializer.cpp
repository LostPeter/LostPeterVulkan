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

#include "../include/MeshSerializer.h"

namespace LostPeterEngine
{
    #define MESH_TAG_MESH_CFG								"cfg_mesh"
    #define	MESH_TAG_MESH									"mesh"

#define MESH_TAG_ATTRIBUTE_NAME		            "name"
#define MESH_TAG_ATTRIBUTE_PATH			        "path"
#define MESH_TAG_ATTRIBUTE_TYPE_MESH			"type_mesh"
#define MESH_TAG_ATTRIBUTE_TYPE_MESH_GEOMETRY	"type_mesh_geometry"
#define MESH_TAG_ATTRIBUTE_TYPE_VERTEX	        "type_vertex"
#define MESH_TAG_ATTRIBUTE_IS_FLIP_Y	        "is_flip_y"
#define MESH_TAG_ATTRIBUTE_IS_TRANS_LOCAL	    "is_trans_local"
#define MESH_TAG_ATTRIBUTE_TRANS_LOCAL	        "trans_local"


    MeshSerializer::MeshSerializer()
        : Base("MeshSerializer")
    {
        
    }
    MeshSerializer::~MeshSerializer()
    {
        DeleteMeshInfoAll();
    }

    bool MeshSerializer::HasMeshInfo(uint nGroup, const String& strName)
    {
        return GetMeshInfo(nGroup, strName) != nullptr;
    }
    MeshInfo* MeshSerializer::GetMeshInfo(uint nGroup, const String& strName)
    {
        MeshInfoGroupPtrMap::iterator itFind = m_mapMeshInfoGroup.find(nGroup);
        if (itFind == m_mapMeshInfoGroup.end())
            return nullptr;
        MeshInfoPtrMap::iterator itFindMI = itFind->second.find(strName);
        if (itFindMI == itFind->second.end())
            return nullptr;
        return itFindMI->second;
    }
    bool MeshSerializer::AddMeshInfo(MeshInfo* pMeshInfo)
    {
        uint32 nGroup = 0;
        if (!FPathManager::GetSingleton().GetGroup(FPathManager::PathGroup_Mesh, pMeshInfo->pathMesh, nGroup))
        {   
            F_LogError("*********************** MeshSerializer::AddMeshInfo: Can not find group by mesh path: [%s] !", pMeshInfo->pathMesh.c_str());
            F_DELETE(pMeshInfo)
            return false;
        }
        
        MeshInfoGroupPtrMap::iterator itFind = m_mapMeshInfoGroup.find(nGroup);
        if (itFind == m_mapMeshInfoGroup.end())
        {
            MeshInfoPtrMap mapMIs;
            m_mapMeshInfoGroup[nGroup] = mapMIs;
            itFind = m_mapMeshInfoGroup.find(nGroup);
        }
        MeshInfoPtrMap::iterator itFindMI = itFind->second.find(pMeshInfo->nameMesh);
        if (itFindMI != itFind->second.end())
        {
            F_LogError("*********************** MeshSerializer::AddMeshInfo: Mesh name already exist: [%s] !", pMeshInfo->nameMesh.c_str());
            F_DELETE(pMeshInfo)
            return false;
        }

        pMeshInfo->group = nGroup;
        itFind->second.insert(MeshInfoPtrMap::value_type(pMeshInfo->nameMesh, pMeshInfo));
        m_aMeshInfo.push_back(pMeshInfo);
        return true;
    }
    void MeshSerializer::DeleteMeshInfo(uint nGroup, const String& strName)
    {
        MeshInfoGroupPtrMap::iterator itFind = m_mapMeshInfoGroup.find(nGroup);
        if (itFind == m_mapMeshInfoGroup.end())
        {
            return;
        }

        MeshInfoPtrMap::iterator itFindMI = itFind->second.find(strName);
        if (itFindMI != itFind->second.end())
        {
            MeshInfoPtrVector::iterator itFindA = std::find(m_aMeshInfo.begin(), m_aMeshInfo.end(), itFindMI->second);
            if (itFindA != m_aMeshInfo.end())
                m_aMeshInfo.erase(itFindA);
            F_DELETE(itFindMI->second)
            itFind->second.erase(itFindMI);
        }
    }
    void MeshSerializer::DeleteMeshInfoAll()
    {
        for (MeshInfoGroupPtrMap::iterator it = m_mapMeshInfoGroup.begin();
             it != m_mapMeshInfoGroup.end(); ++it)
        {
            MeshInfoPtrMap& mapMIs = it->second;
            for (MeshInfoPtrMap::iterator itMI = mapMIs.begin(); 
                 itMI != mapMIs.end(); ++itMI)
            {
                F_DELETE(itMI->second)
            }
        }
        m_aMeshInfo.clear();
        m_mapMeshInfoGroup.clear();
    }

    //File Content XML
    bool MeshSerializer::serializeXML()
    {
        F_Assert(m_pFileXML != nullptr && "MeshSerializer::serializeXML")


        return false;
    }
    bool MeshSerializer::deserializeXML()
    {
        F_Assert(m_pFileXML != nullptr && "MeshSerializer::deserializeXML")
        
        FXMLDocument* pXMLDocument = m_pFileXML->GetXMLDocument();
        FXMLElement* pRoot = pXMLDocument->GetElementRoot();
        int count_child = pRoot->GetElementChildrenCount();
		for (int i = 0; i < count_child; i++)
        {
            FXMLElement* pChild = pRoot->GetElementChild(i);

            //name
            String strNameMesh;
            if (!pChild->ParserAttribute_String(MESH_TAG_ATTRIBUTE_NAME, strNameMesh))
            {
                F_LogError("*********************** MeshSerializer::deserializeXML: Can not find attribute: 'name', mesh index: [%d] !", i);
                continue;
            }
            //path
            String strPathMesh;
            if (!pChild->ParserAttribute_String(MESH_TAG_ATTRIBUTE_PATH, strPathMesh))
            {
                F_LogError("*********************** MeshSerializer::deserializeXML: Can not find attribute: 'path', mesh index: [%d] !", i);
                continue;
            }
            //type_mesh
            String strTypeMesh;
            if (!pChild->ParserAttribute_String(MESH_TAG_ATTRIBUTE_TYPE_MESH, strTypeMesh))
            {
                F_LogError("*********************** MeshSerializer::deserializeXML: Can not find attribute: 'type_mesh', mesh index: [%d] !", i);
                continue;
            }
            FMeshType typeMesh = F_ParseMeshType(strTypeMesh);
            //type_mesh_geometry
            String strTypeMeshGeometry = "";
            FMeshGeometryType typeGeometryType = F_MeshGeometry_EntityTriangle;
            if (typeMesh == F_Mesh_Geometry)
            {
                if (!pChild->ParserAttribute_String(MESH_TAG_ATTRIBUTE_TYPE_MESH_GEOMETRY, strTypeMeshGeometry))
                {
                    F_LogError("*********************** MeshSerializer::deserializeXML: Can not find attribute: 'type_mesh_geometry', mesh index: [%d] !", i);
                    continue;
                }
                typeGeometryType = F_ParseMeshGeometryType(strTypeMeshGeometry);
            }
            //type_vertex
            String strTypeVertex;
            if (!pChild->ParserAttribute_String(MESH_TAG_ATTRIBUTE_TYPE_VERTEX, strTypeVertex))
            {
                F_LogError("*********************** MeshSerializer::deserializeXML: Can not find attribute: 'type_vertex', mesh index: [%d] !", i);
                continue;
            }
            FMeshVertexType typeVertex = F_ParseMeshVertexType(strTypeVertex);
            //is_flip_y
            bool isFlipY = false;
            if (!pChild->ParserAttribute_Bool(MESH_TAG_ATTRIBUTE_IS_FLIP_Y, isFlipY))
            {
                F_LogError("*********************** MeshSerializer::deserializeXML: Can not find attribute: 'is_flip_y', mesh index: [%d] !", i);
                continue;
            }
            //is_trans_local
            bool isTransLocal = false;
            if (!pChild->ParserAttribute_Bool(MESH_TAG_ATTRIBUTE_IS_TRANS_LOCAL, isTransLocal))
            {
                F_LogError("*********************** MeshSerializer::deserializeXML: Can not find attribute: 'is_trans_local', mesh index: [%d] !", i);
                continue;
            }
            //trans_local
            FMatrix4 matTransformLocal = FMath::ms_mat4Unit;
            if (isTransLocal)
            {
                if (!pChild->ParserAttribute_Matrix4(MESH_TAG_ATTRIBUTE_TRANS_LOCAL, matTransformLocal))
                {
                    F_LogError("*********************** MeshSerializer::deserializeXML: Can not find attribute: 'trans_local', mesh index: [%d] !", i);
                    continue;
                }
            }

            MeshInfo* pMeshInfo = new MeshInfo(strNameMesh,
                                               strPathMesh,
                                               typeMesh,
                                               typeVertex,
                                               typeGeometryType,
                                               nullptr,
                                               isFlipY,
                                               isTransLocal,
                                               matTransformLocal);
            if (AddMeshInfo(pMeshInfo))
            {
                F_LogInfo("MeshSerializer::deserializeXML: Add mesh info success, [%s]-[%s]-[%s]-[%s]-[%s]-[%d]-[%d] !",
                          strNameMesh.c_str(), 
                          strPathMesh.c_str(), 
                          strTypeMesh.c_str(), 
                          strTypeMeshGeometry.c_str(), 
                          strTypeVertex.c_str(), 
                          isFlipY ? 1 : 0, 
                          isTransLocal ? 1 : 0);
            }
        }

        return true;
    }

    //File Content Binary
    bool MeshSerializer::serializeBinary()
    {
        return false;
    }
    bool MeshSerializer::deserializeBinary()
    {
        return false;
    }

}; //LostPeterEngine