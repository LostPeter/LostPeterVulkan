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
#include "../include/TerrainUtil.h"

namespace LostPeterVulkan
{
	/////////////////////////// TerrainChunkedNode ///////////////////////
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

	float TerrainChunkedNode::DistanceToAABB(const FVector3& point, const FAABB& box)
	{
		const float dx = std::max({box.m_vMin.x - point.x, 0.0f, point.x - box.m_vMax.x});
		const float dy = std::max({box.m_vMin.y - point.y, 0.0f, point.y - box.m_vMax.y});
		const float dz = std::max({box.m_vMin.z - point.z, 0.0f, point.z - box.m_vMax.z});
		return std::sqrt(dx * dx + dy * dy + dz * dz);
	}
	float TerrainChunkedNode::DistanceToAABBXZ(const FVector3& point, const FAABB& box)
	{
		const float dx = std::max({box.m_vMin.x - point.x, 0.0f, point.x - box.m_vMax.x});
		const float dz = std::max({box.m_vMin.z - point.z, 0.0f, point.z - box.m_vMax.z});
		return std::sqrt(dx * dx + dz * dz);
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

	float TerrainChunkedNode::DistanceToAABB(const FVector3& point)
	{
		return TerrainChunkedNode::DistanceToAABB(point, this->bounds);
	}
	float TerrainChunkedNode::DistanceToAABBXZ(const FVector3& point)
	{
		return TerrainChunkedNode::DistanceToAABBXZ(point, this->bounds);
	}


	/////////////////////////// TerrainChunked ///////////////////////////
	bool TerrainChunked::IntervalsOverlap(int a0, int a1, int b0, int b1)
	{
		return std::max(a0, b0) < std::min(a1, b1);
	}

	TerrainChunked::TerrainChunked(const String& nameChunked)
		: Base(nameChunked)

		, nChunkedX(0)
		, nChunkedZ(0)
		, nChunkedID(-1)

		, pHeightMap(nullptr)
		, nLeafQuads(16)
		, nPatchQuads(16)
		, pRootNode(nullptr)
		, nMaxDepth(0)

		, bIsInit(false)
	{
		
	}
    TerrainChunked::~TerrainChunked()
	{
		Destroy();
	}

	void TerrainChunked::Destroy()
	{
		
	}

	bool TerrainChunked::Init(int chunkedX, int chunkedZ,
							  TerrainHeightMap* pHeightMap,
							  int leafQuads, 
				  			  int patchQuads)
	{
		F_Assert(!IsInit() && leafQuads > 0 && patchQuads > 0 && pHeightMap != nullptr && "TerrainChunked::Init")
		
		this->nChunkedX = chunkedX;
		this->nChunkedZ = chunkedZ;
		this->nChunkedID = TerrainUtil::ToChunkedID(chunkedX, chunkedZ);

		this->pHeightMap = pHeightMap;
		this->nLeafQuads = leafQuads;
		this->nPatchQuads = patchQuads;
		this->pRootNode = buildNode(0, 0, pHeightMap->GetResolution() - 1, 0);

		SetIsInit(true);
		return true;
	}

	void TerrainChunked::SelectDynamicLod(const FVector3& vPos, float fRadiusLod0, float fRadiusLod1, TerrainChunkedNodePtrVector& aNodeSelect)
	{
        FVector3 vLodCenter = vPos;
        vLodCenter.x = FMath::Clamp<float>(vLodCenter.x, this->pRootNode->bounds.m_vMin.x, this->pRootNode->bounds.m_vMax.x);
        vLodCenter.z = FMath::Clamp<float>(vLodCenter.z, this->pRootNode->bounds.m_vMin.z, this->pRootNode->bounds.m_vMax.z);
        selectDynamicRecursive(this->pRootNode, vLodCenter, fRadiusLod0, fRadiusLod1, aNodeSelect);
	}

	void TerrainChunked::BuildRenderData(const TerrainChunkedNodePtrVector& aNodeSelect, TerrainRenderDataVector& aRenderData)
	{
		aRenderData.clear();
		aRenderData.reserve(aNodeSelect.size());

		for (const TerrainChunkedNode* pNode: aNodeSelect)
		{
			const int lod = std::max(0, std::min(2, this->nMaxDepth - pNode->nLevel));
            const FVector3 origin = this->pHeightMap->GetPosition(pNode->nX, pNode->nZ);
            const float sizeWorld = static_cast<float>(pNode->nSize) * this->pHeightMap->GetCellSize();
            std::array<float, 4> stitchStepWorld = stitchStepsForNode(pNode, lod, aNodeSelect);

            FVector3 tint{1.0f, 1.0f, 1.0f};
            if (lod == 1)
            {
                tint = {0.96f, 1.00f, 1.06f};
            }
            else if (lod == 2)
            {
                tint = {1.06f, 0.96f, 0.90f};
            }

			TerrainRenderData rd(origin.x,
								 origin.z,
								 sizeWorld,
								 static_cast<uint32>(lod),
								 tint,
								 stitchStepWorld[0],
								 stitchStepWorld[1],
								 stitchStepWorld[2],
								 stitchStepWorld[3]);
			aRenderData.push_back(rd);
		}
	}

	int TerrainChunked::GetEffectiveSegmentStepCells(const TerrainChunkedNode* pNode, int lod) const
	{
		return std::max(1, (pNode->nSize * (1 << lod)) / this->nPatchQuads);
	}

	TerrainChunkedNode* TerrainChunked::buildNode(int x, int z, int size, int level)
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
            pNode->ppChildren[0] = buildNode(x, 		z, 		  half, level + 1);
            pNode->ppChildren[1] = buildNode(x + half, z, 		  half, level + 1);
            pNode->ppChildren[2] = buildNode(x, 		z + half, half, level + 1);
            pNode->ppChildren[3] = buildNode(x + half, z + half, half, level + 1);
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
		pNode->bounds.SetMinMax(min, max);
	}

	float TerrainChunked::computeGeometricError(TerrainChunkedNode* pNode)
	{
		const int coarseQuads = std::min(this->nPatchQuads, pNode->nSize);
        const int stride = std::max(1, pNode->nSize / coarseQuads);
        float maxError = 0.0f;

        for (int z = pNode->nZ; z <= pNode->nZ + pNode->nSize; ++z)
        {
            for (int x = pNode->nX; x <= pNode->nX + pNode->nSize; ++x)
            {
                const int localX = x - pNode->nX;
                const int localZ = z - pNode->nZ;
                const int cellX = std::min(coarseQuads - 1, localX / stride);
                const int cellZ = std::min(coarseQuads - 1, localZ / stride);

                const int xA = pNode->nX + cellX * stride;
                const int zA = pNode->nZ + cellZ * stride;
                const int xB = std::min(pNode->nX + pNode->nSize, xA + stride);
                const int zB = std::min(pNode->nZ + pNode->nSize, zA + stride);

                const float tx = (xB == xA) ? 0.0f : static_cast<float>(x - xA) / static_cast<float>(xB - xA);
                const float tz = (zB == zA) ? 0.0f : static_cast<float>(z - zA) / static_cast<float>(zB - zA);

                const float h00 = this->pHeightMap->GetHeightRaw(xA, zA);
                const float h10 = this->pHeightMap->GetHeightRaw(xB, zA);
                const float h01 = this->pHeightMap->GetHeightRaw(xA, zB);
                const float h11 = this->pHeightMap->GetHeightRaw(xB, zB);
                const float hx0 = h00 * (1.0f - tx) + h10 * tx;
                const float hx1 = h01 * (1.0f - tx) + h11 * tx;
                const float reconstructed = hx0 * (1.0f - tz) + hx1 * tz;
                const float exact = this->pHeightMap->GetHeightRaw(x, z);

                maxError = std::max(maxError, std::abs(exact - reconstructed));
            }
        }

        return maxError;
	}

	void TerrainChunked::selectDynamicRecursive(TerrainChunkedNode* pNode, const FVector3& vCenter, float fRadiusLod0, float fRadiusLod1, TerrainChunkedNodePtrVector& aNodeSelect)
	{
        const float distance = pNode->DistanceToAABBXZ(vCenter); 

        const int lod0Level = this->nMaxDepth;
        const int lod1Level = std::max(0, this->nMaxDepth - 1);
        const int lod2Level = std::max(0, this->nMaxDepth - 2);
        const int targetLevel = (distance <= fRadiusLod0) ? lod0Level : ((distance <= fRadiusLod1) ? lod1Level : lod2Level);

        if (pNode->HasChildren() && pNode->nLevel < targetLevel)
        {
			for (int i = 0; i < 4; i++)
			{
				if (pNode->ppChildren[i])
				{
					selectDynamicRecursive(pNode->ppChildren[i], vCenter, fRadiusLod0, fRadiusLod1, aNodeSelect);
				}
			}
            return;
        }
        aNodeSelect.push_back(pNode);
	}

	std::array<float, 4> TerrainChunked::stitchStepsForNode(const TerrainChunkedNode* pNode, int lod, const TerrainChunkedNodePtrVector& aNodeSelect) const
	{
		std::array<int, 4> stitchStepCells{};
        const int selfStepCells = GetEffectiveSegmentStepCells(pNode, lod);

        for (const TerrainChunkedNode* pNeighbor : aNodeSelect)
        {
            if (pNeighbor->nID == pNode->nID)
            {
                continue;
            }

            const int neighborLod = std::max(0, std::min(2, this->nMaxDepth - pNeighbor->nLevel));
            const int neighborStepCells = GetEffectiveSegmentStepCells(pNeighbor, neighborLod);
            if (neighborStepCells <= selfStepCells)
            {
                continue;
            }

            const bool zOverlap = IntervalsOverlap(pNode->nZ, pNode->nZ + pNode->nSize, pNeighbor->nZ, pNeighbor->nZ + pNeighbor->nSize);
            const bool xOverlap = IntervalsOverlap(pNode->nX, pNode->nX + pNode->nSize, pNeighbor->nX, pNeighbor->nX + pNeighbor->nSize);

            if (zOverlap && pNeighbor->nX + pNeighbor->nSize == pNode->nX)
            {
                stitchStepCells[0] = std::max(stitchStepCells[0], neighborStepCells);
            }
            if (zOverlap && pNode->nX + pNode->nSize == pNeighbor->nX)
            {
                stitchStepCells[1] = std::max(stitchStepCells[1], neighborStepCells);
            }
            if (xOverlap && pNeighbor->nZ + pNeighbor->nSize == pNode->nZ)
            {
                stitchStepCells[2] = std::max(stitchStepCells[2], neighborStepCells);
            }
            if (xOverlap && pNode->nZ + pNode->nSize == pNeighbor->nZ)
            {
                stitchStepCells[3] = std::max(stitchStepCells[3], neighborStepCells);
            }
        }

        std::array<float, 4> stitchStepWorld{};
        for (size_t i = 0; i < stitchStepWorld.size(); ++i)
        {
            stitchStepWorld[i] = static_cast<float>(stitchStepCells[i]) * this->pHeightMap->GetCellSize();
        }
        return stitchStepWorld;
	}

}; //LostPeterVulkan