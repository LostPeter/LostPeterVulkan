/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2026-08-28
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/TerrainCompute.h"
#include "../include/VulkanWindow.h"
#include "../include/TerrainManager.h"
#include "../include/TerrainSetting.h"
#include "../include/TerrainChunked.h"

namespace LostPeterVulkan
{
	TerrainCompute::TerrainCompute(const String& nameCompute)
		: Base(nameCompute)

		, pChunked(nullptr)

	{

	}
    TerrainCompute::~TerrainCompute()
	{
		Destroy();
	}

	void TerrainCompute::Destroy()
	{

	}

	bool TerrainCompute::Init(TerrainChunked* pChunked)
	{
		this->pChunked = pChunked;

		return true;
	}


}; //LostPeterVulkan