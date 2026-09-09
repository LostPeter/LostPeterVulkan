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
#include "../include/TerrainRender.h"
#include "../include/TerrainCompute.h"
#include "../include/TerrainUtil.h"

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
    int TerrainManager::s_nNodeCount_Step = 20;
	int TerrainManager::s_nRenderDataCount_Init = 500;
    int TerrainManager::s_nRenderDataCount_Step = 20;
	int TerrainManager::s_nRenderInstanceDataCount_Init = 500;
    int TerrainManager::s_nRenderInstanceDataCount_Step = 20;

	TerrainManager::TerrainManager()
        : Base("TerrainManager")	

		, pTerrainSetting(nullptr)
		
		, pNodePool(nullptr)
		, pRenderDataPool(nullptr)
		, pRenderInstanceDataPool(nullptr)
		
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

		destroyStatic();
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
		void TerrainManager::destroyStatic()
		{
			TerrainRender::DestroyStatic();
			TerrainCompute::DestroyStatic();
		}
		void TerrainManager::destroySetting()
		{
			F_DELETE(this->pTerrainSetting)
		}
		void TerrainManager::destroyPools()
		{
			F_DELETE(this->pRenderInstanceDataPool)
			F_DELETE(this->pRenderDataPool)
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
		createStatic();

		return true;
	}
        bool TerrainManager::createPools()
		{
			//pNodePool
			if (this->pNodePool == nullptr)
			{
				this->pNodePool = new ObjectPointerPool<TerrainChunkedNode>();
				this->pNodePool->stepCount = s_nNodeCount_Step;
				this->pNodePool->Reserve(s_nNodeCount_Init);
			}

			//pRenderDataPool
			if (this->pRenderDataPool == nullptr)
			{
				this->pRenderDataPool = new ObjectPointerPool<TerrainRenderData>();
				this->pRenderDataPool->stepCount = s_nRenderDataCount_Step;
				this->pRenderDataPool->Reserve(s_nRenderDataCount_Init);
			}

			//pRenderInstanceDataPool
			if (this->pRenderInstanceDataPool == nullptr)
			{
				this->pRenderInstanceDataPool = new ObjectPointerPool<TerrainRenderInstanceData>();
				this->pRenderInstanceDataPool->stepCount = s_nRenderInstanceDataCount_Step;
				this->pRenderInstanceDataPool->Reserve(s_nRenderInstanceDataCount_Init);
			}


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
			
			F_LogInfo("TerrainManager::createSetting: success, path setting: [%s] !", pathSetting.c_str());
			return true;
		}
		bool TerrainManager::createStatic()
		{
			TerrainRender::InitStatic(this->pTerrainSetting->nPatchQuads);
			TerrainCompute::InitStatic();

			return true;
		}

		bool TerrainManager::createHeightMaps()
		{

			return true;
		}
			TerrainHeightMap* TerrainManager::createHeightMap(TerrainChunkedSetting* pCS)
			{
				TerrainHeightMap* pHeightMap = new TerrainHeightMap(pCS->nameHeightMap);
				if (!pHeightMap->InitFromRaw16(pCS->pathHeightMap,
											   pCS->nX, pCS->nZ,
											   pTerrainSetting->nResolution, pTerrainSetting->fCellSize))
				{
					F_DELETE(pHeightMap)
					F_LogError("*********************** TerrainManager::createHeightMap: Create height map failed, path: [%s] !", pCS->pathHeightMap.c_str());
					return nullptr;
				}
				F_LogInfo("TerrainManager::createHeightMap: Create height map success, path: [%s] !", pCS->pathHeightMap.c_str());

				addHeightMap(pHeightMap);
				return pHeightMap;
			}	
			void TerrainManager::addHeightMap(TerrainHeightMap* pHeightMap)
			{
				TerrainHeightMapPtrMap::iterator itFind = this->mapHeightMaps.find(pHeightMap->nID);
				if (itFind != this->mapHeightMaps.end())
					return;

				this->aHeightMaps.push_back(pHeightMap);
				this->mapHeightMaps[pHeightMap->nID] = pHeightMap;
			}

		bool TerrainManager::createChunkedLods()
		{

			return true;
		}
			TerrainChunkedLod* TerrainManager::createChunkedLod(TerrainHeightMap* pHeightMap)
			{
				TerrainChunkedSetting* pChunkedSetting = this->pTerrainSetting->GetChunkedSetting(pHeightMap->nID);
				F_Assert(pChunkedSetting != nullptr && "TerrainManager::createChunkedLod")

				String nameChunkedLod = "ChunkedLod-" + FUtilString::SaveInt(pHeightMap->nX) + "-" + FUtilString::SaveInt(pHeightMap->nZ);
				TerrainChunkedLod* pChunkedLod = new TerrainChunkedLod(nameChunkedLod);
				if (!pChunkedLod->Init(pChunkedSetting, 
									   pHeightMap,
									   pTerrainSetting->nLeafQuads, pTerrainSetting->nPatchQuads))
				{
					F_LogError("*********************** TerrainManager::createChunkedLod: Create ChunkedLod [%d, %d] failed !", pHeightMap->nX, pHeightMap->nZ);
					return nullptr;
				}
				F_LogInfo("TerrainManager::createChunkedLod: Create ChunkedLod [%d, %d] success !", pHeightMap->nX, pHeightMap->nZ);

				addChunkedLod(pChunkedLod);
				return pChunkedLod;
			}
			void TerrainManager::addChunkedLod(TerrainChunkedLod* pChunkedLod)
			{
				TerrainChunkedLodPtrMap::iterator itFind = this->mapChunkedLods.find(pChunkedLod->pTerrainChunked->nChunkedID);
				if (itFind != this->mapChunkedLods.end())
					return;

				this->aChunkedLods.push_back(pChunkedLod);
				this->mapChunkedLods[pChunkedLod->pTerrainChunked->nChunkedID] = pChunkedLod;
			}

	void TerrainManager::OnCompute(VkCommandBuffer& commandBuffer)
	{
		computeTerrain(commandBuffer);
	}
		void TerrainManager::computeTerrain(VkCommandBuffer& commandBuffer)
		{
			TerrainSetting* pSetting = TerrainSetting::GetSingletonPtr();
			if (pSetting->GetIsGPUCullingAll())
			{
				TerrainCompute::ComputeBatches(commandBuffer);
			}
			else
			{
				for (TerrainChunkedLodPtrVector::iterator it = this->aChunkedLods.begin();
					 it != this->aChunkedLods.end(); ++it)
				{
					(*it)->Compute(commandBuffer);
				}
			}
		}

	void TerrainManager::OnRender(VkCommandBuffer& commandBuffer)
	{
		renderTerrain(commandBuffer);
	}
		void TerrainManager::renderTerrain(VkCommandBuffer& commandBuffer)
		{
			TerrainSetting* pSetting = TerrainSetting::GetSingletonPtr();
			if (pSetting->GetIsGPUCullingAll())
			{
				TerrainRender::RenderBatches(commandBuffer);
			}
			else
			{
				for (TerrainChunkedLodPtrVector::iterator it = this->aChunkedLods.begin();
					 it != this->aChunkedLods.end(); ++it)
				{
					(*it)->Render(commandBuffer);
				}
			}
		}

	void TerrainManager::OnTick()
	{
		updateLod();
	}
		void TerrainManager::updateLod()
		{
			TerrainSetting* pSetting = TerrainSetting::GetSingletonPtr();
			VulkanWindow* pWindow = Base::GetWindowPtr();
			const FVector3& vPosCamera = pWindow->pCamera->GetPos();

			const float lodDx = vPosCamera.x - this->vCenterLod.x;
            const float lodDz = vPosCamera.z - this->vCenterLod.z;
            if ((lodDx * lodDx + lodDz * lodDz) >= pSetting->fLodUpdateDis * pSetting->fLodUpdateDis)
			{
				TerrainRender::BeginRenderBatches();
				{
					for (TerrainChunkedLodPtrVector::iterator it = this->aChunkedLods.begin();
						 it != this->aChunkedLods.end(); ++it)
					{
						(*it)->UpdateLod(vPosCamera);
					}
				}
				TerrainRender::EndRenderBatches();
				
				this->vCenterLod = vPosCamera;
			}
		}

	void TerrainManager::ForceUpdate()
	{
		VulkanWindow* pWindow = Base::GetWindowPtr();

		int nX = 0;
		int nZ = 0;
		TerrainUtil::ParseChunkedXZ(pWindow->pCamera, nX, nZ);
		int nID = TerrainUtil::ToChunkedID(nX, nZ);
		CreateChunk(nX, nZ, nID);
	}

////Pool
	TerrainChunkedNode* TerrainManager::GetNodeFromPool()
	{
		return this->pNodePool->Get();
	}
	void TerrainManager::BackNodeToPool(TerrainChunkedNode* pNode)
	{
		this->pNodePool->Back(pNode);
	}

	TerrainRenderData* TerrainManager::GetRenderDataFromPool()
	{
		return this->pRenderDataPool->Get();
	}
	void TerrainManager::BackRenderDataToPool(TerrainRenderData* pRenderData)
	{
		this->pRenderDataPool->Back(pRenderData);
	}

	TerrainRenderInstanceData* TerrainManager::GetRenderInstanceDataFromPool()
	{
		return this->pRenderInstanceDataPool->Get();
	}
	void TerrainManager::BackRenderInstanceDataToPool(TerrainRenderInstanceData* pRenderInstanceData)
	{
		this->pRenderInstanceDataPool->Back(pRenderInstanceData);
	}


////HeightMap
	TerrainHeightMap* TerrainManager::GetHeightMap(int x, int z)
	{
		int nID = TerrainUtil::ToChunkedID(x, z);
		return GetHeightMap(nID);
	}
	TerrainHeightMap* TerrainManager::GetHeightMap(int id)
	{
		TerrainHeightMapPtrMap::iterator itFind = this->mapHeightMaps.find(id);
		if (itFind != this->mapHeightMaps.end())
			return itFind->second;
		return nullptr;
	}
	TerrainHeightMap* TerrainManager::CreateHeightMap(int x, int z)
	{
		int nID = TerrainUtil::ToChunkedID(x, z);
		return CreateHeightMap(nID);
	}
	TerrainHeightMap* TerrainManager::CreateHeightMap(int id)
	{
		TerrainChunkedSetting* pCS = TerrainSetting::GetSingleton().GetChunkedSetting(id);
		if (pCS == nullptr)
		{
			F_LogError("*********************** TerrainManager::CreateHeightMap: Can not find chunked setting by id: [%d] !", id);
			return nullptr;
		}
		return CreateHeightMap(pCS);
	}
	TerrainHeightMap* TerrainManager::CreateHeightMap(TerrainChunkedSetting* pCS)
	{
		TerrainHeightMapPtrMap::iterator itFind = this->mapHeightMaps.find(pCS->nID);
		if (itFind != this->mapHeightMaps.end())
			return itFind->second;
		return createHeightMap(pCS);
	}

////ChunkedLod
	TerrainChunkedLod* TerrainManager::GetChunkedLod(int x, int z)
	{
		int nID = TerrainUtil::ToChunkedID(x, z);
		return GetChunkedLod(nID);
	}
	TerrainChunkedLod* TerrainManager::GetChunkedLod(int id)
	{
		TerrainChunkedLodPtrMap::iterator itFind = this->mapChunkedLods.find(id);
		if (itFind != this->mapChunkedLods.end())
			return itFind->second;
		return nullptr;
	}
	TerrainChunkedLod* TerrainManager::CreateChunkedLod(int x, int z)
	{
		int nID = TerrainUtil::ToChunkedID(x, z);
		return CreateChunkedLod(nID);
	}
	TerrainChunkedLod* TerrainManager::CreateChunkedLod(int id)
	{
		TerrainChunkedLod* pChunkedLod = GetChunkedLod(id);
		if (pChunkedLod != nullptr)
			return pChunkedLod;

		TerrainHeightMap* pHeightMap = GetHeightMap(id);
		if (pHeightMap == nullptr)
		{
			pHeightMap = CreateHeightMap(id);
			if (pHeightMap == nullptr)
			{
				F_LogError("*********************** TerrainManager::CreateChunkedLod: Can not find and create height map by id: [%d] !", id);
				return nullptr;
			}
		}
		return CreateChunkedLod(pHeightMap);
	}
	TerrainChunkedLod* TerrainManager::CreateChunkedLod(TerrainHeightMap* pHeightMap)
	{
		TerrainChunkedLod* pChunkedLod = GetChunkedLod(pHeightMap->nID);
		if (pChunkedLod != nullptr)
			return pChunkedLod;
		return createChunkedLod(pHeightMap);
	}

////Chunk
	void TerrainManager::CreateChunk(int x, int z)
	{
		int nID = TerrainUtil::ToChunkedID(x, z);
		return CreateChunk(x, z, nID);
	}
	void TerrainManager::CreateChunk(int x, int z, int id)
	{
		F_LogInfo("***** TerrainManager::CreateChunk: Start to load chunk: [%d, %d] - [%d] !", x, z, id);
		{
			//1> TerrainHeightMap
			TerrainHeightMap* pHeightMap = CreateHeightMap(id);
			if (!pHeightMap)
			{
				F_LogError("*********************** TerrainManager::CreateChunk: Can not create height map: [%d, %d] - [%d] !", x, z, id);
				return;
			}

			//2> TerrainChunkedLod
			TerrainChunkedLod* pChunkedLod = CreateChunkedLod(pHeightMap);
			if (!pChunkedLod)
			{
				F_LogError("*********************** TerrainManager::CreateChunk: Can not create chunked lod: [%d, %d] - [%d] !", x, z, id);
				return;
			}
		}
		F_LogInfo("***** TerrainManager::CreateChunk: Complete to load chunk: [%d, %d] - [%d] !", x, z, id);
	}

}; //LostPeterVulkan