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

namespace LostPeterVulkan
{
	const int TerrainUtil::s_nIDMax = 100000;
	const String TerrainUtil::s_strNameTerrain = "Terrain";

	int TerrainUtil::ToChunkedID(int x, int z)
	{
		return z * s_nIDMax + x;
	}

	void TerrainUtil::FromChunkedID(int id, int& x, int& z)
	{
		z = id / s_nIDMax;
		x = id - z * s_nIDMax;
	}

	String TerrainUtil::GetTerrainSettingPath(const String& nameSetting)
	{
		return FUtil::GetPathAssets() + s_strNameTerrain + "/" + nameSetting;
	}
	String TerrainUtil::GetTerrainHeightMapPath(const String& nameHeightMap)
	{
		return FUtil::GetPathAssets() + s_strNameTerrain + "/" + nameHeightMap;
	}

}; //LostPeterVulkan