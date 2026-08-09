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

#ifndef _TERRAIN_HEIGHTMAP_H_
#define _TERRAIN_HEIGHTMAP_H_

#include "Base.h"

namespace LostPeterVulkan
{
	class vulkanExport TerrainHeightMap : public Base
    {
    public:
        TerrainHeightMap(const String& nameHeightMap);
        virtual ~TerrainHeightMap();

	public:
		typedef std::vector<uint32> HeightMapDataI;
		typedef std::vector<float> HeightMapDataF;

	public:
		String pathRaw;

		int nResolution;
		float fCellSize;

		float fWorldSize;
		float fHalfExtent;
		float fHeightMin;
		float fHeightMax;
		
		uint32 nRawMin;
		uint32 nRawMax;

		HeightMapDataI dataRawI;
		HeightMapDataF dataRawF;

		bool bIsInit;

	public:
		void Destroy();
		bool InitFromRaw16(const String& path,
						   int resolution, 
						   float cellSize);

		bool NewFromRaw16(int resolution, 
						  float cellSize);

	public:
		F_FORCEINLINE int GetResolution() const { return this->nResolution; }
		F_FORCEINLINE float GetCellSize() const { return this->fCellSize; }

		F_FORCEINLINE float GetWorldSize() const { return this->fWorldSize; }
		F_FORCEINLINE float GetHalfExtent() const { return this->fHalfExtent; }
		F_FORCEINLINE float GetHeightMin() const { return this->fHeightMin; }
		F_FORCEINLINE float GetHeightMax() const { return this->fHeightMax; }

		F_FORCEINLINE uint32 GetRawMin() const { return this->nRawMin; }
		F_FORCEINLINE uint32 GetRawMax() const { return this->nRawMax; }

		F_FORCEINLINE const HeightMapDataI& GetDataRawI() const { return this->dataRawI; }
		F_FORCEINLINE const HeightMapDataF& GetDataRawF() const { return this->dataRawF; }

		F_FORCEINLINE bool IsInit() const { return this->bIsInit; }

	public:
		float GetHeightRaw(int x, int z) const;
		float GetHeightWorld(float worldX, float worldZ) const;

		FVector3 GetPosition(int x, int z) const;
		FVector3 GetNormal(int x, int z) const;

	private:
		bool loadRaw16(const String& path);
		bool newRaw16();


	};

}; //LostPeterVulkan

#endif