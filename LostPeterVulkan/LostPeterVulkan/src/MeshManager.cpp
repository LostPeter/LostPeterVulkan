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
    }
    bool MeshManager::Init(uint nGroup, const String& strNameCfgMesh)
    {
        

        return true;
    }

}; //LostPeter