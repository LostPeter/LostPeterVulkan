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

#include "../include/TerrainHeightMap.h"
#include "../include/VulkanWindow.h"
#include "../include/TerrainUtil.h"

namespace LostPeterVulkan
{
	TerrainHeightMap::TerrainHeightMap(const String& nameHeightMap)
		: Base(nameHeightMap)

		, pathRaw("")
		, nX(0)
		, nZ(0)
		, nID(-1)

		, nResolution(1025)
		, fCellSize(2.0f)
		
		, fWorldSize(2048)
		, fHalfExtent(1024)
		, fHeightMin(0.0f)
		, fHeightMax(1000.0f)

		, nRawMin(0)
		, nRawMax(1000)

		, bIsInit(false)

	{
		
	}	
    TerrainHeightMap::~TerrainHeightMap()
	{
		Destroy();	
	}

	void TerrainHeightMap::Destroy()
	{

	}

	bool TerrainHeightMap::InitFromRaw16(const String& path,
										 int x, int z,
										 int resolution, 
										 float cellSize)
	{
		this->pathRaw = path;
		this->nX = x;
		this->nZ = z;
		this->nID = TerrainUtil::ToChunkedID(x, z);

		this->nResolution = resolution;
		this->fCellSize = cellSize;
		
		this->fWorldSize = static_cast<float>(resolution - 1) * cellSize;
		this->fHalfExtent = static_cast<float>(resolution - 1) * cellSize * 0.5f;

		if (resolution < 17 || ((resolution - 1) & (resolution - 2)) != 0)
        {
			String msg = "*********************** TerrainHeightMap::InitFromRaw16: resolution is wrong, must be 2^n + 1 and at least 17, value: " + FUtilString::SaveInt(resolution);
			F_LogError(msg.c_str());
			throw std::runtime_error(msg);
        }
		this->dataRawF.resize(static_cast<size_t>(resolution) * static_cast<size_t>(resolution));

		if (!loadRaw16(path))
		{
			F_LogError("*********************** TerrainHeightMap::InitFromRaw16: loadRaw16 failed, path: [%s] !", path.c_str());
			return false;
		}

		SetIsInit(true);
		return true;
	}

	bool TerrainHeightMap::NewFromRaw16(int x, int z,
										int resolution, 
						  				float cellSize)
	{
		this->nX = x;
		this->nZ = z;
		this->nID = TerrainUtil::ToChunkedID(x, z);

		this->nResolution = resolution;
		this->fCellSize = cellSize;

		this->fWorldSize = static_cast<float>(resolution - 1) * cellSize;
		this->fHalfExtent = static_cast<float>(resolution - 1) * cellSize * 0.5f;

		if (resolution < 17 || ((resolution - 1) & (resolution - 2)) != 0)
        {
			String msg = "*********************** TerrainHeightMap::NewFromRaw16: resolution is wrong, must be 2^n + 1 and at least 17, value: " + FUtilString::SaveInt(resolution);
			F_LogError(msg.c_str());
			throw std::runtime_error(msg);
        }
		this->dataRawF.resize(static_cast<size_t>(resolution) * static_cast<size_t>(resolution));

		if (!newRaw16())
		{
			F_LogError("*********************** TerrainHeightMap::InitFromRaw16: newRaw16 failed !");
			return false;
		}

		SetIsInit(true);
		return true;
	}

	float TerrainHeightMap::GetHeightRaw(int x, int z) const
	{
		x = std::max(0, std::min(this->nResolution - 1, x));
        z = std::max(0, std::min(this->nResolution - 1, z));
        return this->dataRawF[static_cast<size_t>(z) * static_cast<size_t>(this->nResolution) + static_cast<size_t>(x)];
	}
	float TerrainHeightMap::GetHeightWorld(float worldX, float worldZ) const
	{
		const float gx = FMath::Clamp<float>((worldX + this->fHalfExtent) / this->fCellSize, 0.0f, static_cast<float>(this->nResolution - 1));
        const float gz = FMath::Clamp<float>((worldZ + this->fHalfExtent) / this->fCellSize, 0.0f, static_cast<float>(this->nResolution - 1));

        const int x0 = static_cast<int>(std::floor(gx));
        const int z0 = static_cast<int>(std::floor(gz));
        const int x1 = std::min(x0 + 1, this->nResolution - 1);
        const int z1 = std::min(z0 + 1, this->nResolution - 1);
        const float tx = gx - static_cast<float>(x0);
        const float tz = gz - static_cast<float>(z0);

        const float h00 = GetHeightRaw(x0, z0);
        const float h10 = GetHeightRaw(x1, z0);
        const float h01 = GetHeightRaw(x0, z1);
        const float h11 = GetHeightRaw(x1, z1);
        const float hx0 = h00 * (1.0f - tx) + h10 * tx;
        const float hx1 = h01 * (1.0f - tx) + h11 * tx;
        return hx0 * (1.0f - tz) + hx1 * tz;
	}

	FVector3 TerrainHeightMap::GetPosition(int x, int z) const
	{
		return FVector3(static_cast<float>(x) * this->fCellSize - this->fHalfExtent,
						GetHeightRaw(x, z),
						static_cast<float>(z) * this->fCellSize - this->fHalfExtent);
	}
	FVector3 TerrainHeightMap::GetNormal(int x, int z) const
	{
		const float dx = GetHeightRaw(x + 1, z) - GetHeightRaw(x - 1, z);
        const float dz = GetHeightRaw(x, z + 1) - GetHeightRaw(x, z - 1);
		FVector3 vNormal(-dx, 2.0f * this->fCellSize, -dz);
		return FMath::Normalize(vNormal);
	}


	bool TerrainHeightMap::loadRaw16(const String& path)
	{
		CharVector bytes;
        if (!FUtil::LoadAssetFileContent(path.c_str(), bytes))
        {
            F_LogError("*********************** TerrainHeightMap::loadRaw16: failed, path: [%s] !", path.c_str());
            return false;
        }
        if (bytes.size() <= 0)
        {
            return false;
        }

		this->dataRawI.resize(this->dataRawF.size());
        uint16_t rawMin = std::numeric_limits<uint16_t>::max();
        uint16_t rawMax = std::numeric_limits<uint16_t>::lowest();
        for (size_t i = 0; i < this->dataRawI.size(); ++i)
        {
            const uint16_t value = static_cast<uint16_t>(bytes[i * 2u]) |
                                   static_cast<uint16_t>(static_cast<uint16_t>(bytes[i * 2u + 1u]) << 8u);
            this->dataRawI[i] = value;
            rawMin = std::min(rawMin, value);
            rawMax = std::max(rawMax, value);
        }

        if (rawMax <= rawMin)
        {
			String msg = "*********************** TerrainHeightMap::loadRaw16: heightmap RAW has no usable height range, path: " + path;
			F_LogError(msg.c_str());
			throw std::runtime_error(msg);
        }

		constexpr float kRawHeightScale = 360.0f;
        constexpr float kRawHeightOffset = -90.0f;
        this->nRawMin = rawMin;
        this->nRawMax = rawMax;
        const float invRange = 1.0f / static_cast<float>(rawMax - rawMin);
        this->fHeightMin = std::numeric_limits<float>::max();
        this->fHeightMax = std::numeric_limits<float>::lowest();
        for (size_t i = 0; i < this->dataRawI.size(); ++i)
        {
            const float normalized = static_cast<float>(this->dataRawI[i] - rawMin) * invRange;
            const float heightValue = normalized * kRawHeightScale + kRawHeightOffset;
            this->dataRawF[i] = heightValue;
            this->fHeightMin = std::min(this->fHeightMin, heightValue);
            this->fHeightMax = std::max(this->fHeightMax, heightValue);
        }

		return true;
	}
	bool TerrainHeightMap::newRaw16()
	{
		const float last = static_cast<float>(this->nResolution - 1);
        this->fHeightMin = std::numeric_limits<float>::max();
        this->fHeightMax = std::numeric_limits<float>::lowest();
        for (int z = 0; z < this->nResolution; ++z)
        {
            for (int x = 0; x < this->nResolution; ++x)
            {
                const float nx = (static_cast<float>(x) / last - 0.5f) * 2.0f;
                const float nz = (static_cast<float>(z) / last - 0.5f) * 2.0f;
                const float radial = std::sqrt(nx * nx + nz * nz);

                const float rolling =
                    34.0f * std::sin(nx * 7.0f + 0.35f) * std::cos(nz * 5.2f - 0.8f) +
                    18.0f * std::sin((nx + nz) * 11.0f);

                const float ridge =
                    92.0f * std::exp(-((nx + 0.18f) * (nx + 0.18f) * 7.5f + (nz - 0.28f) * (nz - 0.28f) * 20.0f)) +
                    64.0f * std::exp(-((nx - 0.34f) * (nx - 0.34f) * 24.0f + (nz + 0.18f) * (nz + 0.18f) * 8.0f));

                const float valley = -38.0f * std::exp(-std::abs(nx + nz * 0.45f) * 3.6f);
                const float islandFalloff = FMath::Clamp<float>(1.25f - radial * 0.58f, 0.35f, 1.0f);
                const float h = (rolling + ridge + valley) * islandFalloff;

                this->dataRawF[static_cast<size_t>(z) * static_cast<size_t>(this->nResolution) + static_cast<size_t>(x)] = h;
                this->fHeightMin = std::min(this->fHeightMin, h);
                this->fHeightMax = std::max(this->fHeightMax, h);
            }
        }

		return true;
	}

}; //LostPeterVulkan