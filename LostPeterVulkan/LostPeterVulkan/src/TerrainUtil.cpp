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

	int TerrainUtil::ToChunkedID(int x, int z)
	{
		return z * s_nIDMax + x;
	}

	void TerrainUtil::FromChunkedID(int id, int& x, int& z)
	{
		z = id / s_nIDMax;
		x = id - z * s_nIDMax;
	}

}; //LostPeterVulkan