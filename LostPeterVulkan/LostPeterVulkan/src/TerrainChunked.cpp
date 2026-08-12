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

#include "../include/TerrainChunked.h"
#include "../include/VulkanWindow.h"

namespace LostPeterVulkan
{
	TerrainChunkedNode::TerrainChunkedNode()
		: nID(-1)
		, nLevel(0)
		, nX(0)
		, nZ(0)
		, nSize(0)
		, fHeightMin(0.0f)
		, fHeightMax(0.0f)
		, fGeometricError(0.0f)
	{
		ClearChildren();
	}
	bool TerrainChunkedNode::HasChildren() const
	{	
		for (int i = 0; i < 4; i++)
		{
			if (this->ppChildren[i] != nullptr)
				return true;
		}
		return false;
	}
	void TerrainChunkedNode::ClearChildren()
	{
		for (int i = 0; i < 4; i++)
		{
			this->ppChildren[i] = nullptr;
		}
	}


	TerrainChunked::TerrainChunked(const String& nameChunked)
		: Base(nameChunked)

		, pHeightMap(nullptr)
		, nLeafQuads(16)
		, nPatchQuads(16)
		, pRootNode(nullptr)
		, nMaxDepth(0)
	{
		
	}
    TerrainChunked::~TerrainChunked()
	{
		Destroy();
	}

	void TerrainChunked::Destroy()
	{
		
	}

	bool TerrainChunked::Init(TerrainHeightMap* pHeightMap)
	{
		this->pHeightMap = pHeightMap;


		return true;
	}


}; //LostPeterVulkan