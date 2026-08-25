/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2026-08-22
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/TerrainUtil.h"
#include "../include/VulkanWindow.h"
#include "../include/TerrainSetting.h"

namespace LostPeterVulkan
{
	const int TerrainUtil::s_nIDMax = 100000;
	const String TerrainUtil::s_strNameAssets = "Assets";
	const String TerrainUtil::s_strNameTerrain = "Terrain";

	String TerrainUtil::GetTerrainSettingPath(const String& nameSetting)
	{
		return FUtil::GetPathAssets() + s_strNameTerrain + "/" + nameSetting;
	}
	String TerrainUtil::GetTerrainHeightMapPath(const String& nameHeightMap)
	{
		return FUtil::GetPathAssets() + s_strNameTerrain + "/" + nameHeightMap;
	}
	String TerrainUtil::GetTerrainHeightMapRelativePath(const String& nameHeightMap)
	{
		return s_strNameAssets + "/" + s_strNameTerrain + "/" + nameHeightMap;
	}


	int TerrainUtil::ToChunkedID(int x, int z)
	{
		return z * s_nIDMax + x;
	}

	void TerrainUtil::FromChunkedID(int id, int& x, int& z)
	{
		z = id / s_nIDMax;
		x = id - z * s_nIDMax;
	}


	void TerrainUtil::ParseChunkedXZ(float posX, float posZ, int& x, int& z)
	{
		TerrainSetting* pSetting = TerrainSetting::GetSingletonPtr();
		int nStartX = pSetting->GetStartX();
		int nStartZ = pSetting->GetStartZ();
		int nCountX = pSetting->GetCountX();
		int nCountZ = pSetting->GetCountZ();
		int nSizeX = pSetting->GetSizeX();
		int nSizeZ = pSetting->GetSizeZ();

		int nX = posX / nSizeX;
		int nZ = posZ / nSizeZ;
		x = nX < nStartX ? nStartX : (nX > (nSizeX + nCountX) ? (nSizeX + nCountX) : nX);
		z = nZ < nStartZ ? nStartZ : (nZ > (nSizeZ + nCountZ) ? (nSizeZ + nCountZ) : nZ); 
	}
	void TerrainUtil::ParseChunkedXZ(const FVector2& pos, int& x, int& z)
	{
		ParseChunkedXZ(pos.x, pos.y, x, z);
	}
	void TerrainUtil::ParseChunkedXZ(const FVector3& pos, int& x, int& z)
	{
		ParseChunkedXZ(pos.x, pos.z, x, z);
	}
	void TerrainUtil::ParseChunkedXZ(const FCamera* pCamera, int& x, int& z)
	{
		const FVector3& pos = pCamera->GetPos();
		ParseChunkedXZ(pos.x, pos.z, x, z);
	}

}; //LostPeterVulkan