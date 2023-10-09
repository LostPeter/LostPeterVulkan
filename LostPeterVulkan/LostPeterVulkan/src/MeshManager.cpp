/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-04
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/MeshManager.h"
#include "../include/VulkanWindow.h"
#include "../include/MeshSerializer.h"

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
    bool MeshManager::Init(uint nGroup, const String& strNameCfgMesh)
    {
        //1> Mesh Cfg Path 
        String strPathCfgMesh = FPathManager::GetSingleton().GetFilePath(nGroup, strNameCfgMesh);
        if (strPathCfgMesh.empty())
        {
            F_LogError("*********************** MeshManager::Init: Can not get file path from group: [%u], name: [%s] !", nGroup, strNameCfgMesh.c_str());
            return false;
        }

        //2> Mesh Serializer
        m_pMeshSerializer = new MeshSerializer();
        if (!m_pMeshSerializer->LoadFile(strPathCfgMesh))
        {
            F_LogError("*********************** MeshManager::Init: Load file mesh cfg failed, group: [%u], name: [%s] !", nGroup, strNameCfgMesh.c_str());
            return false;
        }
        
        return true;
    }

    Mesh* MeshManager::GetMesh(uint nGroup, const String& strNameMesh)
    {
        MeshGroupPtrMap::iterator itFindGroup = m_mapMeshGroup.find(nGroup);
        if (itFindGroup == m_mapMeshGroup.end())
        {
            F_LogError("*********************** MeshManager::GetMesh: Can not find group: [%u], name: [%s]", nGroup, strNameMesh.c_str());
            return nullptr;
        }

        MeshPtrMap::iterator itFindMesh = itFindGroup->second.find(strNameMesh);
        if (itFindMesh == itFindGroup->second.end())
        {
            F_LogError("*********************** MeshManager::GetMesh: Can not find mesh from group: [%u], name: [%s]", nGroup, strNameMesh.c_str());
            return nullptr;
        }
        return itFindMesh->second;
    }

    bool MeshManager::LoadMeshAll()
    {
        return true;
    }
    bool MeshManager::LoadMesh(uint nGroup, const String& strNameMesh)
    {
        return true;
    }

    void MeshManager::DeleteMesh(uint nGroup, const String& strNameMesh)
    {

    }

    void MeshManager::DeleteMeshAll()
    {

    }

}; //LostPeter