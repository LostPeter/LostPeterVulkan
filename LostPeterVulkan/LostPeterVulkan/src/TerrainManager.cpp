/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2026-07-26
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/TerrainManager.h"
#include "../include/VulkanWindow.h"
#include "../include/TerrainSetting.h"
#include "../include/TerrainChunked.h"

template<> LostPeterVulkan::TerrainManager* LostPeterFoundation::FSingleton<LostPeterVulkan::TerrainManager>::ms_Singleton = nullptr;

namespace LostPeterVulkan
{
	TerrainManager* TerrainManager::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	TerrainManager& TerrainManager::GetSingleton()
	{  
		F_Assert(ms_Singleton && "TerrainManager::GetSingleton")
		return (*ms_Singleton);     
	}

	int TerrainManager::s_nNodeCount_Init = 500;
    int TerrainManager::s_nNodeCount_Max = 100000;
    int TerrainManager::s_nNodeCount_Step = 20;

	TerrainManager::TerrainManager()
        : Base("TerrainManager")	

		, pNodePool(nullptr)
		, pTerrainSetting(nullptr)

    {

    }
    TerrainManager::~TerrainManager()
    {
        Destroy();
    }

	void TerrainManager::Destroy()
	{
		destroySetting();
		destroyPools();
	}
		void TerrainManager::destroyPools()
		{
			F_DELETE(this->pNodePool)
		}
		void TerrainManager::destroySetting()
		{
			F_DELETE(this->pTerrainSetting)
		}

    bool TerrainManager::Init()
	{
		createPools();
		createSetting();


		return true;
	}
        void TerrainManager::createPools()
		{
			if (this->pNodePool != nullptr)
				return;

			this->pNodePool = new ObjectPointerPool<TerrainChunkedNode>();
			this->pNodePool->stepCount = s_nNodeCount_Step;
			this->pNodePool->Reserve(s_nNodeCount_Init);
		}
		void TerrainManager::createSetting()
		{
			if (this->pTerrainSetting != nullptr)
				return;

			this->pTerrainSetting = new TerrainSetting();
		}

}; //LostPeterVulkan