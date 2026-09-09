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
#include "../include/TerrainSetting.h"
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

	bool TerrainChunkedLod::Init(TerrainChunkedSetting* pChunkedSetting, 
				  				 TerrainHeightMap* pHeightMap,
								 int leafQuads, int patchQuads)
	{
		//1> pTerrainChunked
		String nameChunked = "Chunked-" + FUtilString::SaveInt(pChunkedSetting->nX) + "-" + FUtilString::SaveInt(pChunkedSetting->nZ);
		this->pTerrainChunked = new TerrainChunked(nameChunked);
		if (!this->pTerrainChunked->Init(pChunkedSetting,
										 pHeightMap,
										 leafQuads, patchQuads))
		{
			F_LogError("*********************** TerrainChunkedLod::Init: Create Chunked [%d, %d] failed !", pChunkedSetting->nX, pChunkedSetting->nZ);
			return false;
		}
		F_LogInfo("TerrainChunkedLod::Init: Create Chunked [%d, %d] success !", pChunkedSetting->nX, pChunkedSetting->nZ);

		return true;
	}

	void TerrainChunkedLod::UpdateLod(const FVector3& vCenterLod)
	{	
		if (!this->pTerrainChunked)
			return;

		TerrainSetting* pSetting = TerrainSetting::GetSingletonPtr();
		VulkanWindow* pWindow = Base::GetWindowPtr();

		//1> Nodes
		TerrainChunkedNodePtrVector aNodeSelect;
		this->pTerrainChunked->SelectDynamicLod(vCenterLod, pSetting->fLodRadius0, pSetting->fLodRadius1, aNodeSelect);

		//2> RenderDatas
		TerrainRenderDataPtrVector aRenderData;
		this->pTerrainChunked->BuildRenderData(aNodeSelect, aRenderData);

		//3> Batches
		this->pTerrainChunked->BuildBatches(aRenderData);
	}

	void TerrainChunkedLod::Compute(VkCommandBuffer& commandBuffer)
	{
		if (!this->pTerrainChunked)
			return;
		
		this->pTerrainChunked->Compute(commandBuffer);
	}
	void TerrainChunkedLod::Render(VkCommandBuffer& commandBuffer)
	{
		if (!this->pTerrainChunked)
			return;

		this->pTerrainChunked->Render(commandBuffer);
	}

}; //LostPeterVulkan