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
        static int s_nNodeCount_Max;
        static int s_nNodeCount_Step;

	public:
		TerrainSetting* pTerrainSetting;
		TerrainHeightMapPtrVector aHeightMaps;
		TerrainHeightMapPtrMap mapHeightMaps;
		TerrainChunkedLodPtrVector aChunkedLods;
		TerrainChunkedLodPtrMap mapChunkedLods;

		ObjectPointerPool<TerrainChunkedNode>* pNodePool;

	public:
        static TerrainManager& GetSingleton();
		static TerrainManager* GetSingletonPtr();
		
	public:
		F_FORCEINLINE const TerrainHeightMapPtrVector& GetHeightMapPtrVector() const { return this->aHeightMaps; }
		F_FORCEINLINE const TerrainHeightMapPtrMap& GetHeightMapPtrMap() const { return this->mapHeightMaps; }
		F_FORCEINLINE const TerrainChunkedLodPtrVector& GetChunkedLodPtrVector() const { return this->aChunkedLods; }
		F_FORCEINLINE const TerrainChunkedLodPtrMap& GetChunkedLodPtrMap() const { return this->mapChunkedLods; }

	public:
        void Destroy();
        bool Init();

		TerrainHeightMap* CreateHeightMap(int x, int z);
		TerrainChunkedLod* CreateChunkedLod(int x, int z);

	protected:
	////destroy
		void destroyChunkedLods();
			bool destroyChunkedLod(TerrainChunkedLod* pChunkedLod);
		void destroyHeightMaps();
			bool destroyHeightMap(TerrainHeightMap* pHeightMap);
		void destroySetting();
		void destroyPools();

	////create
        bool createPools();
		bool createSetting();
		bool createHeightMaps();
		bool createChunkedLods();
		
		TerrainHeightMap* createHeightMap(int x, int z);
		TerrainChunkedLod* createChunkedLod(int x, int z);

	};

}; //LostPeterVulkan

#endif