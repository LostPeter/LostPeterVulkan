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

#include "../include/MeshManager.h"
#include "../include/MeshSerializer.h"
#include "../include/Mesh.h"

template<> LostPeterEngine::MeshManager* LostPeterFoundation::FSingleton<LostPeterEngine::MeshManager>::ms_Singleton = nullptr;

namespace LostPeterEngine
{
    MeshManager* MeshManager::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	MeshManager& MeshManager::GetSingleton()
	{  
		F_Assert(ms_Singleton && "MeshManager::GetSingleton")
		return (*ms_Singleton);     
	}


    //Set MeshParam
    void MeshManager::SetMeshParam_Usage(NameValuePairMap& mapParam, uint32 nUsage)
    {
        FUtil::SaveNameValuePair(mapParam, E_GetMeshParamTypeName(E_MeshParam_Usage), FUtilString::SaveUInt(nUsage));
    }
    void MeshManager::SetMeshParam_MeshType(NameValuePairMap& mapParam, FMeshType eMesh)
    {
        FUtil::SaveNameValuePair(mapParam, E_GetMeshParamTypeName(E_MeshParam_MeshType), E_GetMeshParamTypeName(eMesh));
    }
    void MeshManager::SetMeshParam_MeshVertexType(NameValuePairMap& mapParam, FMeshVertexType eMeshVertex)
    {
        FUtil::SaveNameValuePair(mapParam, E_GetMeshParamTypeName(E_MeshParam_MeshVertexType), E_GetMeshParamTypeName(eMeshVertex));
    }
    void MeshManager::SetMeshParam_MeshGeometryType(NameValuePairMap& mapParam, FMeshGeometryType eMeshGeometry)
    {
        FUtil::SaveNameValuePair(mapParam, E_GetMeshParamTypeName(E_MeshParam_MeshGeometryType), E_GetMeshParamTypeName(eMeshGeometry));
    }
    void MeshManager::SetMesheParam_IsFlipY(NameValuePairMap& mapParam, bool bIsFlipY)
    {
        FUtil::SaveNameValuePair(mapParam, E_GetMeshParamTypeName(E_MeshParam_IsFlipY), FUtilString::SaveBool(bIsFlipY));
    }

    //Get MeshParam
    uint32 MeshManager::GetMeshParam_Usage(NameValuePairMap& mapParam)
    {
        const String& strName = E_GetMeshParamTypeName(E_MeshParam_Usage);
        NameValuePairMap::iterator itFind = mapParam.find(strName);
        if (itFind == mapParam.end())
        {
            //F_LogError("*********************** MeshManager::GetMeshParam_Usage: Can not find param name: [%s] from param map !", strName.c_str());
            return MeshManager::ms_nUsage_Default;
        }
        return FUtilString::ParserUInt(itFind->second);
    }
    FMeshType MeshManager::GetMeshParam_MeshType(NameValuePairMap& mapParam)
    {
        const String& strName = E_GetMeshParamTypeName(E_MeshParam_MeshType);
        NameValuePairMap::iterator itFind = mapParam.find(strName);
        if (itFind == mapParam.end())
        {
            //F_LogError("*********************** MeshManager::GetMeshParam_MeshType: Can not find param name: [%s] from param map !", strName.c_str());
            return MeshManager::ms_eMesh_Default;
        }
        return F_ParseMeshType(itFind->second);
    }
    FMeshVertexType MeshManager::GetMeshParam_MeshVertexType(NameValuePairMap& mapParam)
    {
        const String& strName = E_GetMeshParamTypeName(E_MeshParam_MeshVertexType);
        NameValuePairMap::iterator itFind = mapParam.find(strName);
        if (itFind == mapParam.end())
        {
            //F_LogError("*********************** MeshManager::GetMeshParam_MeshVertexType: Can not find param name: [%s] from param map !", strName.c_str());
            return MeshManager::ms_eMeshVertex_Default;
        }
        return F_ParseMeshVertexType(itFind->second);
    }
    FMeshGeometryType MeshManager::GetMeshParam_MeshGeometryType(NameValuePairMap& mapParam)
    {
        const String& strName = E_GetMeshParamTypeName(E_MeshParam_MeshGeometryType);
        NameValuePairMap::iterator itFind = mapParam.find(strName);
        if (itFind == mapParam.end())
        {
            //F_LogError("*********************** MeshManager::GetMeshParam_MeshGeometryType: Can not find param name: [%s] from param map !", strName.c_str());
            return MeshManager::ms_eMeshGeometry_Default;
        }
        return F_ParseMeshGeometryType(itFind->second);
    }
    bool MeshManager::GetMeshParam_IsFlipY(NameValuePairMap& mapParam)
    {
        const String& strName = E_GetMeshParamTypeName(E_MeshParam_IsFlipY);
        NameValuePairMap::iterator itFind = mapParam.find(strName);
        if (itFind == mapParam.end())
        {
            //F_LogError("*********************** MeshManager::GetMeshParam_IsFlipY: Can not find param name: [%s] from param map !", strName.c_str());
            return MeshManager::ms_bIsFlipY_Default;
        }
        return FUtilString::ParserBool(itFind->second);
    }


    NameValuePairMap MeshManager::ms_mapParam_Default;
        uint32 MeshManager::ms_nUsage_Default = 0;
        FMeshType MeshManager::ms_eMesh_Default = F_Mesh_File;
        FMeshVertexType MeshManager::ms_eMeshVertex_Default = F_MeshVertex_Pos3Color4Normal3Tangent3Tex2;
        FMeshGeometryType MeshManager::ms_eMeshGeometry_Default = F_MeshGeometry_EntityQuad;
        bool MeshManager::ms_bIsFlipY_Default = false;

    const String& MeshManager::GetMeshParamValue(EMeshParamType type)
    {
        const String& strName = E_GetMeshParamTypeName(type);
        NameValuePairMap::iterator itFind = ms_mapParam_Default.find(strName);
        F_Assert(itFind != ms_mapParam_Default.end() && "MeshManager::GetMeshParamValue")
        return itFind->second;
    }

    const String MeshManager::ms_strMeshConfigName = "Cfg_Mesh.xml";
    MeshManager::MeshManager()
        : ResourceManager(E_GetResourceTypeName(E_Resource_Mesh), E_Resource_Mesh)
        , m_pMeshSerializer(nullptr)
    {

    }

    MeshManager::~MeshManager()
    {
        Destroy();
    }

    void MeshManager::Destroy()
    {
        F_DELETE(m_pMeshSerializer)
        
        ResourceManager::Destroy();
    }
    
    bool MeshManager::Init(uint nGroup, const String& strNameCfg)
    {
        //1> Mesh Cfg Path 
        String strPathCfgMesh = FPathManager::GetSingleton().GetFilePath(nGroup, strNameCfg);
        if (strPathCfgMesh.empty())
        {
            F_LogError("*********************** MeshManager::Init: Can not get file path from group: [%u], name: [%s] !", nGroup, strNameCfg.c_str());
            return false;
        }

        //2> Mesh Serializer
        m_pMeshSerializer = new MeshSerializer();
        if (!m_pMeshSerializer->LoadFile(strPathCfgMesh))
        {
            F_LogError("*********************** MeshManager::Init: Load file mesh cfg failed, group: [%u], name: [%s] !", nGroup, strNameCfg.c_str());
            return false;
        }
        
        return true;
    }

    Mesh* MeshManager::LoadMesh(uint nGroup, const String& strName, const String& strGroupName /*= ResourceGroupManager::ms_strNameResourceGroup_AutoDetect*/)
    {
        if (m_pMeshSerializer == nullptr)
            return nullptr;

        Mesh* pMesh = GetMesh(strName, strGroupName);
        if (pMesh != nullptr)
        {
            pMesh->AddRef();
        }
        else
        {
            MeshInfo* pMeshInfo = m_pMeshSerializer->GetMeshInfo(nGroup, strName);
            if (pMeshInfo == nullptr)
            {
                F_LogError("*********************** MeshManager::LoadMesh: Can not find mesh info, group: [%u], name: [%s], groupName: [%s] !", nGroup, strName.c_str(), strGroupName.c_str());
                return nullptr;
            }
            pMesh = loadMesh(pMeshInfo);
            if (!pMesh)
            {
                return nullptr;
            }
        }
        return pMesh;
    }
        Mesh* MeshManager::loadMesh(MeshInfo* pMI)
        {
            Mesh* pMesh = CreateMesh(pMI->pathMesh,
                                     pMI->group,
                                     pMI->nameMesh,
                                     ResourceGroupManager::ms_strNameResourceGroup_Internal,
                                     0,
                                     pMI->eMesh,
                                     pMI->eMeshVertex,
                                     pMI->eMeshGeometry,
                                     pMI->isFlipY,
                                     nullptr);
            if (!pMesh)
            {
                F_LogError("*********************** MeshManager::loadMesh: CreateMesh failed, group: [%u], name: [%s], path: [%s] !", pMI->group, pMI->nameMesh.c_str(), pMI->pathMesh.c_str());
                return nullptr;
            }
            return pMesh;
        }

    bool MeshManager::HasMesh(const String& strName)
    {
        return GetResourceByName(strName) != nullptr;
    }
    bool MeshManager::HasMesh(const String& strName, const String& strGroupName)
    {
        return GetResourceByName(strName, strGroupName) != nullptr;
    }
    Mesh* MeshManager::GetMesh(const String& strName)
    {
        Resource* pResource = GetResourceByName(strName);
        if (pResource == nullptr)
            return nullptr;
        return (Mesh*)pResource;
    }
    Mesh* MeshManager::GetMesh(const String& strName, const String& strGroupName)
    {
        Resource* pResource = GetResourceByName(strName, strGroupName);
        if (pResource == nullptr)
            return nullptr;
        return (Mesh*)pResource;
    }

    ResourceCreateOrRetrieveResult MeshManager::CreateOrRetrieveMesh(const String& strPath,
                                                                     uint32 nGroup, 
                                                                     const String& strName, 
                                                                     const String& strGroupName, 
                                                                     bool bIsManualLoad /*= false*/,
                                                                     ResourceManualLoader* pManualLoader /*= nullptr*/, 
                                                                     const NameValuePairMap* pLoadParams /*= nullptr*/,
                                                                     uint32 nUsage /*= MeshManager::ms_nUsage_Default*/,
                                                                     FMeshType eMesh /*= MeshManager::ms_eMesh_Default*/,
                                                                     FMeshVertexType eMeshVertex /*= MeshManager::ms_eMeshVertex_Default*/,
                                                                     FMeshGeometryType eMeshGeometry /*= MeshManager::ms_eMeshGeometry_Default*/,
                                                                     bool bIsFlipY /*= MeshManager::ms_bIsFlipY_Default*/,
                                                                     FMeshCreateParam* pMeshCreateParam /*= nullptr*/)
    {
        NameValuePairMap mapMeshParam;
        if (bIsManualLoad && pLoadParams)
        {
            FUtil::CopyNameValuePairMapTo(pLoadParams, &mapMeshParam);
        }

        ResourceCreateOrRetrieveResult result = ResourceManager::CreateOrRetrieve(nGroup,
                                                                                  strName,
                                                                                  strGroupName,
                                                                                  bIsManualLoad,
                                                                                  pManualLoader,
                                                                                  &mapMeshParam);
		if (!result.first || !result.second)
		{
            F_LogError("*********************** Mesh::CreateOrRetrieveMesh: CreateOrRetrieve resource failed, group: [%d], name: [%s]", nGroup, strName.c_str());
			return result;
		}

        Mesh* pMesh = (Mesh*)result.first;
        pMesh->SetPath(strPath);
        pMesh->SetUsage(nUsage);
        pMesh->SetMeshType(eMesh);
        pMesh->SetMeshVertexType(eMeshVertex);
        pMesh->SetMeshGeometryType(eMeshGeometry);
        pMesh->SetIsFlipY(bIsFlipY);
        pMesh->SetMeshCreateParam(pMeshCreateParam);

		return result;
    }

    Mesh* MeshManager::Prepare(const String& strPath,
                               uint32 nGroup, 
                               const String& strName, 
                               const String& strGroupName, 
                               uint32 nUsage /*= MeshManager::ms_nUsage_Default*/,
                               FMeshType eMesh /*= MeshManager::ms_eMesh_Default*/,
                               FMeshVertexType eMeshVertex /*= MeshManager::ms_eMeshVertex_Default*/,
                               FMeshGeometryType eMeshGeometry /*= MeshManager::ms_eMeshGeometry_Default*/,
                               bool bIsFlipY /*= MeshManager::ms_bIsFlipY_Default*/,
                               FMeshCreateParam* pMeshCreateParam /*= nullptr*/)
    {
        ResourceCreateOrRetrieveResult result = CreateOrRetrieveMesh(strPath,
                                                                     nGroup,
                                                                     strName,
                                                                     strGroupName,
                                                                     false,
                                                                     nullptr,
                                                                     nullptr,
                                                                     nUsage,
                                                                     eMesh,
                                                                     eMeshVertex,
                                                                     eMeshGeometry,
                                                                     bIsFlipY,
                                                                     pMeshCreateParam);
		Mesh* pMesh = (Mesh*)result.first;
        if (!pMesh)
            return nullptr;
		pMesh->Prepare();

		return pMesh;
    }

    Mesh* MeshManager::CreateMesh(const String& strPath,
                                  uint32 nGroup, 
                                  const String& strName, 
                                  const String& strGroupName, 
                                  uint32 nUsage /*= MeshManager::ms_nUsage_Default*/,
                                  FMeshType eMesh /*= MeshManager::ms_eMesh_Default*/,
                                  FMeshVertexType eMeshVertex /*= MeshManager::ms_eMeshVertex_Default*/,
                                  FMeshGeometryType eMeshGeometry /*= MeshManager::ms_eMeshGeometry_Default*/,
                                  bool bIsFlipY /*= MeshManager::ms_bIsFlipY_Default*/,
                                  FMeshCreateParam* pMeshCreateParam /*= nullptr*/)         
    {
        ResourceCreateOrRetrieveResult result = CreateOrRetrieveMesh(strPath,
                                                                     nGroup,
                                                                     strName,
                                                                     strGroupName,
                                                                     false,
                                                                     nullptr,
                                                                     nullptr,
                                                                     nUsage,
                                                                     eMesh,
                                                                     eMeshVertex,
                                                                     eMeshGeometry,
                                                                     bIsFlipY,
                                                                     pMeshCreateParam);
		Mesh* pMesh = (Mesh*)result.first;
        if (!pMesh)
            return nullptr;
		pMesh->Load();

        pMesh->AddRef();
		return pMesh;
    }       

    Resource* MeshManager::createImpl(uint32 nGroup,
                                      const String& strName,
                                      const String& strGroupName,
                                      ResourceHandle nHandle, 
                                      bool bIsManualLoad,
                                      ResourceManualLoader* pManualLoader, 
                                      const NameValuePairMap* pLoadParams)
    {
        return new Mesh(this,
                        nGroup, 
                        strName,
                        strGroupName,
                        nHandle,
                        bIsManualLoad,
                        pManualLoader);
    }

}; //LostPeterEngine