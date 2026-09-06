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

#ifndef _TERRAIN_MANAGER_H_
#define _TERRAIN_MANAGER_H_

#include "Base.h"
#include "ObjectPool.h"

namespace LostPeterVulkan
{
	class vulkanExport TerrainManager : public FSingleton<TerrainManager>
                                   	  , public Base
    {
    public:
        TerrainManager();
        virtual ~TerrainManager();

	public:
		static int s_nNodeCount_Init;
        static int s_nNodeCount_Step;
		static int s_nRenderDataCount_Init;
        static int s_nRenderDataCount_Step;
		static int s_nRenderInstanceDataCount_Init;
        static int s_nRenderInstanceDataCount_Step;

	public:
		TerrainSetting* pTerrainSetting;
		TerrainHeightMapPtrVector aHeightMaps;
		TerrainHeightMapPtrMap mapHeightMaps;
		TerrainChunkedLodPtrVector aChunkedLods;
		TerrainChunkedLodPtrMap mapChunkedLods;

		ObjectPointerPool<TerrainChunkedNode>* pNodePool;
		ObjectPointerPool<TerrainRenderData>* pRenderDataPool;
		ObjectPointerPool<TerrainRenderInstanceData>* pRenderInstanceDataPool;
		
		FVector3 vCenterLod;

	public:
        static TerrainManager& GetSingleton();
		static TerrainManager* GetSingletonPtr();
		
	public:
		F_FORCEINLINE const TerrainHeightMapPtrVector& GetHeightMapPtrVector() const { return this->aHeightMaps; }
		F_FORCEINLINE const TerrainHeightMapPtrMap& GetHeightMapPtrMap() const { return this->mapHeightMaps; }
		F_FORCEINLINE const TerrainChunkedLodPtrVector& GetChunkedLodPtrVector() const { return this->aChunkedLods; }
		F_FORCEINLINE const TerrainChunkedLodPtrMap& GetChunkedLodPtrMap() const { return this->mapChunkedLods; }

		F_FORCEINLINE ObjectPointerPool<TerrainChunkedNode>* GetNodePool() const { return this->pNodePool; }
		F_FORCEINLINE ObjectPointerPool<TerrainRenderData>* GetRenderDataPool() const { return this->pRenderDataPool; }
		F_FORCEINLINE ObjectPointerPool<TerrainRenderInstanceData>* GetRenderInstanceDataPool() const { return this->pRenderInstanceDataPool; }

		F_FORCEINLINE const FVector3& GetCenterLod() const { return this->vCenterLod; }

	public:
        void Destroy();
        bool Init(const String& pathSetting);

		void OnCompute(VkCommandBuffer& commandBuffer);
		void OnRender(VkCommandBuffer& commandBuffer);
		void OnTick();

		void ForceUpdate();

	////Pool
		TerrainChunkedNode* GetNodeFromPool();
		void BackNodeToPool(TerrainChunkedNode* pNode);

		TerrainRenderData* GetRenderDataFromPool();
		void BackRenderDataToPool(TerrainRenderData* pRenderData);

		TerrainRenderInstanceData* GetRenderInstanceDataFromPool();
		void BackRenderInstanceDataToPool(TerrainRenderInstanceData* pRenderInstanceData);

	////HeightMap
		TerrainHeightMap* GetHeightMap(int x, int z);
		TerrainHeightMap* GetHeightMap(int id);
		TerrainHeightMap* CreateHeightMap(int x, int z);
		TerrainHeightMap* CreateHeightMap(int id);
		TerrainHeightMap* CreateHeightMap(TerrainChunkedSetting* pCS);

	////ChunkedLod
		TerrainChunkedLod* GetChunkedLod(int x, int z);
		TerrainChunkedLod* GetChunkedLod(int id);
		TerrainChunkedLod* CreateChunkedLod(int x, int z);
		TerrainChunkedLod* CreateChunkedLod(int id);
		TerrainChunkedLod* CreateChunkedLod(TerrainHeightMap* pHeightMap);

	////Chunk
		void CreateChunk(int x, int z);
		void CreateChunk(int x, int z, int id);

	protected:
	////destroy
		void destroyChunkedLods();
			bool destroyChunkedLod(TerrainChunkedLod* pChunkedLod);
		void destroyHeightMaps();
			bool destroyHeightMap(TerrainHeightMap* pHeightMap);

		void destroyStatic();
		void destroySetting();
		void destroyPools();

	////create
        bool createPools();
		bool createSetting(const String& pathSetting);
		bool createStatic();

		bool createHeightMaps();
			TerrainHeightMap* createHeightMap(TerrainChunkedSetting* pCS);
			void addHeightMap(TerrainHeightMap* pHeightMap);

		bool createChunkedLods();
			TerrainChunkedLod* createChunkedLod(TerrainHeightMap* pHeightMap);
			void addChunkedLod(TerrainChunkedLod* pChunkedLod);

	protected:
	////compute
		void computeTerrain(VkCommandBuffer& commandBuffer);
	////render
		void renderTerrain(VkCommandBuffer& commandBuffer);
	////lod
		void updateLod();	

	};

}; //LostPeterVulkan

#endif