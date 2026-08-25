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

#include "../include/TerrainChunkedLod.h"
#include "../include/VulkanWindow.h"
#include "../include/TerrainManager.h"
#include "../include/TerrainChunked.h"
#include "../include/TerrainHeightMap.h"

namespace LostPeterVulkan
{
	TerrainChunkedLod::TerrainChunkedLod(const String& nameChunkedLod)
		: Base(nameChunkedLod)

		, pTerrainChunked(nullptr)

	{
		
	}
    TerrainChunkedLod::~TerrainChunkedLod()
	{
		Destroy();
	}

	void TerrainChunkedLod::Destroy()
	{
		F_DELETE(this->pTerrainChunked)
	}

	bool TerrainChunkedLod::Init(int chunkedX, int chunkedZ,
				  				 TerrainHeightMap* pHeightMap,
								 int leafQuads, int patchQuads)
	{
		//1> pTerrainChunked
		String nameChunked = "Chunked-" + FUtilString::SaveInt(chunkedX) + "-" + FUtilString::SaveInt(chunkedZ);
		this->pTerrainChunked = new TerrainChunked(nameChunked);
		if (!this->pTerrainChunked->Init(chunkedX, chunkedZ,
										 pHeightMap,
										 leafQuads, patchQuads))
		{
			F_LogError("*********************** TerrainChunkedLod::Init: Create Chunked [%d, %d] failed !", chunkedX, chunkedZ);
			return false;
		}
		F_LogInfo("TerrainChunkedLod::Init: Create Chunked [%d, %d] success !", chunkedX, chunkedZ);

		return true;
	}


}; //LostPeterVulkan