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

#include "../include/TerrainSetting.h"
#include "../include/VulkanWindow.h"

namespace LostPeterVulkan
{
	TerrainSetting::TerrainSetting(const String& nameSetting)
		: Base(nameSetting)

		, fLodPixelError(2.5f)
		, bHeadless(false)
		, nAutoCloseMs(0)
		
	{
		
	}
    TerrainSetting::~TerrainSetting()
	{
		
	}


}; //LostPeterVulkan