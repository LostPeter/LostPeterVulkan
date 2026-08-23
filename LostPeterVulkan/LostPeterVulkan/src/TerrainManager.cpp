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
#include "../include/TerrainHeightMap.h"
#include "../include/TerrainChunked.h"
#include "../include/TerrainChunkedLod.h"

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
		destroyChunkedLods();
		destroyHeightMaps();

		destroySetting();
		destroyPools();
	}
		void TerrainManager::destroyChunkedLods()
		{
			for (TerrainChunkedLodPtrVector::iterator it = this->aChunkedLods.begin();
				 it != this->aChunkedLods.end(); ++it) 
			{
				TerrainChunkedLod* pChunkedLod = *it;
				F_DELETE(pChunkedLod)
			}
			this->aChunkedLods.clear();
			this->mapChunkedLods.clear();
		}
			bool TerrainManager::destroyChunkedLod(TerrainChunkedLod* pChunkedLod)
			{
				F_Assert(pChunkedLod != nullptr && "TerrainManager::destroyChunkedLod")

				TerrainChunked* pChunked = pChunkedLod->GetTerrainChunked();
				TerrainChunkedLodPtrMap::iterator itFind = this->mapChunkedLods.find(pChunked->GetChunkedID());
				if (itFind == this->mapChunkedLods.end())
				{
					F_LogError("*********************** TerrainManager::destroyChunkedLod failed, ChunkedLod: [%d, %d] !", pChunked->GetChunkedX(), pChunked->GetChunkedZ());
					return false;
				}
				this->mapChunkedLods.erase(itFind);
				TerrainChunkedLodPtrVector::iterator itA = std::find(this->aChunkedLods.begin(), this->aChunkedLods.end(), pChunkedLod);
				if (itA != this->aChunkedLods.end())
				{
					this->aChunkedLods.erase(itA);
				}

				F_DELETE(pChunkedLod)
				return true;
			}
		void TerrainManager::destroyHeightMaps()
		{
			for (TerrainHeightMapPtrVector::iterator it = this->aHeightMaps.begin();
				 it != this->aHeightMaps.end(); ++it) 
			{
				TerrainHeightMap* pHeightMap = *it;
				F_DELETE(pHeightMap)
			}
			this->aHeightMaps.clear();
			this->mapHeightMaps.clear();
		}
			bool TerrainManager::destroyHeightMap(TerrainHeightMap* pHeightMap)
			{
				F_Assert(pHeightMap != nullptr && "TerrainManager::destroyHeightMap")

				TerrainHeightMapPtrMap::iterator itFind = this->mapHeightMaps.find(pHeightMap->GetID());
				if (itFind == this->mapHeightMaps.end())
				{
					F_LogError("*********************** TerrainManager::destroyHeightMap failed, HeightMap: [%d, %d] !", pHeightMap->GetX(), pHeightMap->GetZ());
					return false;
				}
				this->mapHeightMaps.erase(itFind);
				TerrainHeightMapPtrVector::iterator itA = std::find(this->aHeightMaps.begin(), this->aHeightMaps.end(), pHeightMap);
				if (itA != this->aHeightMaps.end())
				{
					this->aHeightMaps.erase(itA);
				}
				
				F_DELETE(pHeightMap)
				return true;
			}
		void TerrainManager::destroySetting()
		{
			F_DELETE(this->pTerrainSetting)
		}
		void TerrainManager::destroyPools()
		{
			F_DELETE(this->pNodePool)
		}


    bool TerrainManager::Init(const String& pathSetting)
	{
		createPools();
		if (!createSetting(pathSetting))
		{
			F_LogError("*********************** TerrainManager::Init: failed, path: [%s] !", pathSetting.c_str());
			return false;
		}

		return true;
	}
        bool TerrainManager::createPools()
		{
			if (this->pNodePool != nullptr)
				return true;

			this->pNodePool = new ObjectPointerPool<TerrainChunkedNode>();
			this->pNodePool->stepCount = s_nNodeCount_Step;
			this->pNodePool->Reserve(s_nNodeCount_Init);

			return true;
		}
		bool TerrainManager::createSetting(const String& pathSetting)
		{
			if (this->pTerrainSetting != nullptr)
				return true;

			this->pTerrainSetting = new TerrainSetting();
			if (!this->pTerrainSetting->Init(pathSetting))
			{
				F_LogError("*********************** TerrainManager::createSetting: failed, path: [%s] !", pathSetting.c_str());
				return false;
			}
			
			return true;
		}
		bool TerrainManager::createHeightMaps()
		{

			return true;
		}
		bool TerrainManager::createChunkedLods()
		{

			return true;
		}
		TerrainHeightMap* TerrainManager::createHeightMap(int x, int z)
		{

			return nullptr;
		}	
		TerrainChunkedLod* TerrainManager::createChunkedLod(int x, int z)
		{

			return nullptr;
		}

	TerrainHeightMap* TerrainManager::CreateHeightMap(int x, int z)
	{

		return nullptr;
	}
	TerrainChunkedLod* TerrainManager::CreateChunkedLod(int x, int z)
	{

		return nullptr;
	}

}; //LostPeterVulkan