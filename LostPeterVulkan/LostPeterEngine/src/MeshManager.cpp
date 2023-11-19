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

template<> LostPeter::MeshManager* LostPeterFoundation::FSingleton<LostPeter::MeshManager>::ms_Singleton = nullptr;

namespace LostPeter
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

    MeshManager::MeshManager()
        : Base("MeshManager")
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
        DeleteMeshAll();
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

    bool MeshManager::LoadMeshAll()
    {
        if (m_pMeshSerializer == nullptr)
            return false;

        DeleteMeshAll();
        MeshInfoPtrVector& aMeshInfos = m_pMeshSerializer->GetMeshInfoPtrVector();
        for (MeshInfoPtrVector::iterator it = aMeshInfos.begin();
             it != aMeshInfos.end(); ++it)
        {
            if (!loadMesh(*it))
                continue;
        }

        return true;
    }
    Mesh* MeshManager::LoadMesh(uint nGroup, const String& strName)
    {
        if (m_pMeshSerializer == nullptr)
            return nullptr;

        Mesh* pMesh = GetMesh(nGroup, strName);
        if (pMesh == nullptr)
        {
            MeshInfo* pMeshInfo = m_pMeshSerializer->GetMeshInfo(nGroup, strName);
            if (pMeshInfo == nullptr)
            {
                F_LogError("*********************** MeshManager::LoadMesh: Can not find mesh info, group: [%u], name: [%s] !", nGroup, strName.c_str());
                return nullptr;
            }
            if (!loadMesh(pMeshInfo))
            {
                return nullptr;
            }
        }
        return pMesh;
    }
    Mesh* MeshManager::loadMesh(MeshInfo* pMI)
    {
        Mesh* pMesh = new Mesh(pMI->group,
                               pMI->nameMesh,
                               pMI->pathMesh,
                               pMI->typeMesh,
                               pMI->typeVertex,
                               pMI->typeGeometryType,
                               nullptr);
        if (!pMesh->LoadMesh(pMI->isFlipY, pMI->isTransformLocal, pMI->matTransformLocal))
        {
            F_LogError("*********************** MeshManager::loadMesh: Load mesh failed, name: [%s], path: [%s] !", pMI->nameMesh.c_str(), pMI->pathMesh.c_str());
            F_DELETE(pMesh)
            return nullptr;
        }

        if (AddMesh(pMI->group, pMesh))
        {
            F_LogInfo("MeshManager::loadMesh: Load mesh success, [%u]-[%s]-[%s]-[%d] !", 
                      pMI->group, 
                      pMI->nameMesh.c_str(), 
                      pMI->pathMesh.c_str(), 
                      (int)pMesh->aMeshSubs.size());
        }
        return pMesh;
    }

    bool MeshManager::HasMesh(uint nGroup, const String& strName)
    {
        return GetMesh(nGroup, strName) != nullptr;
    }

    Mesh* MeshManager::GetMesh(uint nGroup, const String& strName)
    {
        MeshGroupPtrMap::iterator itFindGroup = m_mapMeshGroup.find(nGroup);
        if (itFindGroup == m_mapMeshGroup.end())
        {
            return nullptr;
        }

        MeshPtrMap::iterator itFindMesh = itFindGroup->second.find(strName);
        if (itFindMesh == itFindGroup->second.end())
        {
            return nullptr;
        }
        return itFindMesh->second;
    }

    bool MeshManager::AddMesh(uint nGroup, Mesh* pMesh)
    {
        MeshGroupPtrMap::iterator itFind = m_mapMeshGroup.find(nGroup);
        if (itFind == m_mapMeshGroup.end())
        {
            MeshPtrMap mapMesh;
            m_mapMeshGroup[nGroup] = mapMesh;
            itFind = m_mapMeshGroup.find(nGroup);
        }
        const String& strName = pMesh->GetName();
        MeshPtrMap::iterator itFindMesh = itFind->second.find(strName);
        if (itFindMesh != itFind->second.end())
        {
            F_LogError("*********************** MeshManager::AddMesh: Mesh name already exist: [%s] !", strName.c_str());
            F_DELETE(pMesh)
            return false;
        }

        itFind->second.insert(MeshPtrMap::value_type(strName, pMesh));
        m_aMesh.push_back(pMesh);
        return true;
    }

    void MeshManager::DeleteMesh(uint nGroup, const String& strName)
    {
        MeshGroupPtrMap::iterator itFind = m_mapMeshGroup.find(nGroup);
        if (itFind == m_mapMeshGroup.end())
        {
            return;
        }

        MeshPtrMap::iterator itFindMesh = itFind->second.find(strName);
        if (itFindMesh != itFind->second.end())
        {
            MeshPtrVector::iterator itFindA = std::find(m_aMesh.begin(), m_aMesh.end(), itFindMesh->second);
            if (itFindA != m_aMesh.end())
                m_aMesh.erase(itFindA);
            F_DELETE(itFindMesh->second)
            itFind->second.erase(itFindMesh);
        }
    }

    void MeshManager::DeleteMeshAll()
    {
        for (MeshGroupPtrMap::iterator it = m_mapMeshGroup.begin();
             it != m_mapMeshGroup.end(); ++it)
        {
            MeshPtrMap& mapMesh = it->second;
            for (MeshPtrMap::iterator itMesh = mapMesh.begin(); 
                 itMesh != mapMesh.end(); ++itMesh)
            {
                F_DELETE(itMesh->second)
            }
        }
        m_aMesh.clear();
        m_mapMeshGroup.clear();
    }

}; //LostPeter