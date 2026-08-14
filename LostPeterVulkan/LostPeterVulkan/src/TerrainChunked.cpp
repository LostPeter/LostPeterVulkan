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
#include "../include/TerrainHeightMap.h"
#include "../include/TerrainManager.h"

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

	void TerrainChunkedNode::Init(int id,
								  int level,
								  int x,
								  int z,
								  int size)
	{
		this->nID = id;
		this->nLevel = level;
		this->nX = x;
		this->nZ = z;
		this->nSize = size;
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

	bool TerrainChunked::Init(TerrainHeightMap* pHeightMap,
							  int leafQuads, 
				  			  int patchQuads)
	{
		F_Assert(leafQuads > 0 && patchQuads > 0 && "TerrainChunked::Init")

		this->pHeightMap = pHeightMap;
		this->nLeafQuads = leafQuads;
		this->nPatchQuads = patchQuads;



		return true;
	}


	TerrainChunkedNode* TerrainChunked::createNode(int x, int z, int size, int level)
	{
		const int id = static_cast<int>(this->aNodes.size());
		TerrainChunkedNode* pNode = TerrainManager::GetSingletonPtr()->pNodePool->Get();
		pNode->Init(id,
					level,
					x,
					z,
					size);
		this->aNodes.push_back(pNode);

        this->nMaxDepth = std::max(this->nMaxDepth, level);
        computeBoundsAndError(pNode);

        if (size > this->nLeafQuads)
        {
            const int half = size / 2;
            pNode->ppChildren[0] = createNode(x, 		z, 		  half, level + 1);
            pNode->ppChildren[1] = createNode(x + half, z, 		  half, level + 1);
            pNode->ppChildren[2] = createNode(x, 		z + half, half, level + 1);
            pNode->ppChildren[3] = createNode(x + half, z + half, half, level + 1);
        }
        return pNode;
	}

	void TerrainChunked::computeBoundsAndError(TerrainChunkedNode* pNode)
	{
		float minHeight = std::numeric_limits<float>::max();
        float maxHeight = std::numeric_limits<float>::lowest();

        for (int z = pNode->nZ; z <= pNode->nZ + pNode->nSize; ++z)
        {
            for (int x = pNode->nX; x <= pNode->nX + pNode->nSize; ++x)
            {
                float h = this->pHeightMap->GetHeightRaw(x, z);
                minHeight = std::min(minHeight, h);
                maxHeight = std::max(maxHeight, h);
            }
        }

        pNode->fHeightMin = minHeight;
        pNode->fHeightMax = maxHeight;
        pNode->fGeometricError = computeGeometricError(pNode);

        const FVector3 minCorner = this->pHeightMap->GetPosition(pNode->nX, pNode->nZ);
        const FVector3 maxCorner = this->pHeightMap->GetPosition(pNode->nX + pNode->nSize, pNode->nZ + pNode->nSize);
		FVector3 min(std::min(minCorner.x, maxCorner.x), minHeight, std::min(minCorner.z, maxCorner.z));
		FVector3 max(std::max(minCorner.x, maxCorner.x), maxHeight, std::max(minCorner.z, maxCorner.z));
		pNode->aabb.SetMinMax(min, max);
	}

	float TerrainChunked::computeGeometricError(TerrainChunkedNode* pNode)
	{
		
		return 0.0f;
	}

}; //LostPeterVulkan