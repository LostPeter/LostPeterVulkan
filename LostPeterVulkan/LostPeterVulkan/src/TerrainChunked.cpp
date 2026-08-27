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
#include "../include/TerrainSetting.h"
#include "../include/TerrainHeightMap.h"
#include "../include/TerrainManager.h"
#include "../include/TerrainUtil.h"
#include "../include/VKTexture.h"

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

	void TerrainChunkedNode::BackNodeToPool()
	{
		for (int i = 0; i < 4; i++)
		{
			if (ppChildren[i] != nullptr)
			{
				ppChildren[i]->BackNodeToPool();
			}
		}
		TerrainManager::GetSingleton().BackNodeToPool(this);
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

		//Node
		, nLeafQuads(16)
		, nPatchQuads(16)
		, pRootNode(nullptr)
		, nMaxDepth(0)

		//HeightMap
		, pHeightMap(nullptr)
		, pTexture_HeightMap(nullptr)
		, pTexture_NormalMap(nullptr)

		//TextureDiffuse/Normal/Control
		, pTexture_Diffuse(nullptr)
		, pTexture_Normal(nullptr)
		, pTexture_Control(nullptr)

		//Render
		, pRender(nullptr)

		, bIsInit(false)
	{
		
	}
    TerrainChunked::~TerrainChunked()
	{
		Destroy();
	}

	void TerrainChunked::Destroy()
	{
		destroyTextures();
		destroyNodes();
	}

	bool TerrainChunked::Init(TerrainChunkedSetting* pChunkedSetting, 
							  TerrainHeightMap* pHeightMap,
							  int leafQuads, int patchQuads)
	{
		F_Assert(!IsInit() && leafQuads > 0 && patchQuads > 0 && pHeightMap != nullptr && "TerrainChunked::Init")

		//1> Node
		this->nChunkedX = pChunkedSetting->nX;
		this->nChunkedZ = pChunkedSetting->nZ;
		this->nChunkedID = pChunkedSetting->nID;
		this->pHeightMap = pHeightMap;
		this->nLeafQuads = leafQuads;
		this->nPatchQuads = patchQuads;
		this->pRootNode = buildNode(0, 0, pHeightMap->GetResolution() - 1, 0);

		//2> Texture
		if (!createTextures(pChunkedSetting))
		{
			F_LogError("*********************** TerrainChunked::Init: createTextures failed !");
			return false;
		}

		//3> Render
		if (!createRender())
		{
			F_LogError("*********************** TerrainChunked::Init: createRender failed !");
			return false;
		}

		F_LogInfo("TerrainChunked::Init: Create chunk: [%d, %d] success !", this->nChunkedX, this->nChunkedZ);
		SetIsInit(true);
		return true;
	}
		void TerrainChunked::destroyNodes()
		{
			if (this->pRootNode != nullptr)
			{
				this->pRootNode->BackNodeToPool();
			}
			this->pRootNode = nullptr;
		}
		void TerrainChunked::destroyRender()
		{	
			F_DELETE(this->pRender)
		}
		void TerrainChunked::destroyTextures()
		{
			F_DELETE(this->pTexture_NormalMap)
			F_DELETE(this->pTexture_HeightMap)

			F_DELETE(this->pTexture_Diffuse)
			F_DELETE(this->pTexture_Normal)
			F_DELETE(this->pTexture_Control)
		}

		bool TerrainChunked::createTextures(TerrainChunkedSetting* pChunkedSetting)
		{
			//1> Texture_HeightMap
			{
				String nameTexture = "Texture-TerrainHeightMap-" + FUtilString::SaveInt(pChunkedSetting->nX) + "-" + FUtilString::SaveInt(pChunkedSetting->nZ);
				StringVector aPathTextureHeightMap;
				this->pTexture_HeightMap = new VKTexture(0,
														 nameTexture,
														 aPathTextureHeightMap,
														 F_Texture_2D,
														 F_TexturePixelFormat_R16_UNORM,
														 F_TextureFilter_Bilinear,
														 F_TextureAddressing_Clamp,
														 F_TextureBorderColor_OpaqueBlack,
														 true,
														 true);
				this->pTexture_HeightMap->rtImageUsage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
				this->pTexture_HeightMap->poTextureImageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				this->pTexture_HeightMap->LoadTexture(pChunkedSetting->nResolution, 
													  pChunkedSetting->nResolution,
													  1,
													  2,
													  (uint8*)this->pHeightMap->dataRawI.data());
				
				F_LogInfo("TerrainChunked::createTextures: Create terrain render texture [TerrainHeightMap] - [%d, %d] success !",
						  pChunkedSetting->nResolution, pChunkedSetting->nResolution);
			}

			//2> Texture_NormalMap
			{
				String nameTexture = "Texture-TerrainNormalMap" + FUtilString::SaveInt(pChunkedSetting->nX) + "-" + FUtilString::SaveInt(pChunkedSetting->nZ);
				StringVector aPathTextureNormalMap;
				this->pTexture_NormalMap = new VKTexture(0,
														 nameTexture,
														 aPathTextureNormalMap,
														 F_Texture_2D,
														 F_TexturePixelFormat_R8G8B8A8_UNORM,
														 F_TextureFilter_Bilinear,
														 F_TextureAddressing_Clamp,
														 F_TextureBorderColor_OpaqueBlack,
														 true,
														 true);
				this->pTexture_NormalMap->rtImageUsage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_STORAGE_BIT;
				this->pTexture_NormalMap->poTextureImageLayout = VK_IMAGE_LAYOUT_GENERAL;
				this->pTexture_NormalMap->LoadTexture(pChunkedSetting->nResolution, 
													  pChunkedSetting->nResolution,
												  	  1,
													  4,
													  nullptr);

				F_LogInfo("TerrainChunked::createTextures: Create terrain render texture [TerrainNormalMap] - [%d, %d] success !",
						  pChunkedSetting->nResolution, pChunkedSetting->nResolution);
			}

			
			uint32_t mipMapCount = 1;
			//3> Terrain Diffuse
			{
				String nameTexture = "Texture-Terrain-Diffuse-" + FUtilString::SaveInt(pChunkedSetting->nX) + "-" + FUtilString::SaveInt(pChunkedSetting->nZ);
				this->pTexture_Diffuse = new VKTexture(0,
													   nameTexture,
													   pChunkedSetting->aPathTextureDiffuse,
													   F_Texture_2DArray,
													   F_TexturePixelFormat_R8G8B8A8_SRGB,
													   F_TextureFilter_Bilinear,
													   F_TextureAddressing_Clamp,
													   F_TextureBorderColor_OpaqueBlack,
													   false,
													   false);
				this->pTexture_Diffuse->LoadTexture(1024, 
													1024,
													1);

				F_LogInfo("TerrainChunked::createTextures: Create terrain diffuse texture array: [%s] success !",
						  pChunkedSetting->strTextureDiffuse.c_str());
			}
			//4> Terrain Normal
			{
				String nameTexture = "Texture-Terrain-Normal-" + FUtilString::SaveInt(pChunkedSetting->nX) + "-" + FUtilString::SaveInt(pChunkedSetting->nZ);
				this->pTexture_Normal = new VKTexture(0,
													  nameTexture,
												  	  pChunkedSetting->aPathTextureNormal,
													  F_Texture_2DArray,
													  F_TexturePixelFormat_R8G8B8A8_UNORM,
													  F_TextureFilter_Bilinear,
													  F_TextureAddressing_Clamp,
													  F_TextureBorderColor_OpaqueBlack,
													  false,
													  false);
				this->pTexture_Normal->LoadTexture(1024, 
												   1024,
												   1);

				F_LogInfo("TerrainChunked::createTextures: Create terrain normal texture array: [%s] success !",
						  pChunkedSetting->strTextureNormal.c_str());
			}
			//5> Terrain Control
			{
				String nameTexture = "Texture-Terrain-Control-" + FUtilString::SaveInt(pChunkedSetting->nX) + "-" + FUtilString::SaveInt(pChunkedSetting->nZ);
				this->pTexture_Control = new VKTexture(0,
													   nameTexture,
													   pChunkedSetting->aPathTextureControl,
													   F_Texture_2DArray,
													   F_TexturePixelFormat_R8G8B8A8_UNORM,
													   F_TextureFilter_Bilinear,
													   F_TextureAddressing_Clamp,
													   F_TextureBorderColor_OpaqueBlack,
													   false,
													   false);
				this->pTexture_Control->LoadTexture(1024, 
													1024,
													1);

				F_LogInfo("TerrainChunked::createTextures: Create terrain control texture array: [%s] success !",
						  pChunkedSetting->strTextureControl.c_str());
			}

			return true;
		}

		bool TerrainChunked::createRender()
		{
			String nameRender = "Render-Terrain-" + FUtilString::SaveInt(this->nChunkedX) + "-" + FUtilString::SaveInt(this->nChunkedZ);
			this->pRender = new TerrainRender(nameRender);
			if (!this->pRender->Init())
			{
				F_LogError("*********************** TerrainChunked::createRender: Create terrain render [%d, %d] failed !", this->nChunkedX, this->nChunkedZ);
				return false;
			}
			F_LogInfo("TerrainChunked::createRender: Create terrain render [%d, %d] success !", this->nChunkedX, this->nChunkedZ);
			
			return true;
		}

	void TerrainChunked::SelectDynamicLod(const FVector3& vPos, float fRadiusLod0, float fRadiusLod1, TerrainChunkedNodePtrVector& aNodeSelect)
	{
        FVector3 vLodCenter = vPos;
        vLodCenter.x = FMath::Clamp<float>(vLodCenter.x, this->pRootNode->bounds.m_vMin.x, this->pRootNode->bounds.m_vMax.x);
        vLodCenter.z = FMath::Clamp<float>(vLodCenter.z, this->pRootNode->bounds.m_vMin.z, this->pRootNode->bounds.m_vMax.z);
        selectDynamicRecursive(this->pRootNode, vLodCenter, fRadiusLod0, fRadiusLod1, aNodeSelect);
	}

	void TerrainChunked::BuildRenderData(const TerrainChunkedNodePtrVector& aNodeSelect, TerrainRenderDataPtrVector& aRenderData)
	{
		TerrainManager* pTerrainManager = TerrainManager::GetSingletonPtr();
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

			TerrainRenderData* pRenderData = pTerrainManager->GetRenderDataFromPool();
			pRenderData->Init(origin.x,
							  origin.z,
							  sizeWorld,
							  static_cast<uint32>(lod),
							  tint,
							  stitchStepWorld[0],
							  stitchStepWorld[1],
							  stitchStepWorld[2],
							  stitchStepWorld[3]);
			aRenderData.push_back(pRenderData);
		}
	}

	void TerrainChunked::BuildBatches(const TerrainRenderDataPtrVector& aRenderData)
	{
		if (!this->pRender)
			return;

		this->pRender->BeginAddRenderDatas();
		{
			this->pRender->AddRenderDatas(aRenderData);
		}
		this->pRender->EndAddRenderDatas();
	}

	int TerrainChunked::GetEffectiveSegmentStepCells(const TerrainChunkedNode* pNode, int lod) const
	{
		return std::max(1, (pNode->nSize * (1 << lod)) / this->nPatchQuads);
	}

	TerrainChunkedNode* TerrainChunked::buildNode(int x, int z, int size, int level)
	{
		const int id = static_cast<int>(this->aNodes.size());
		TerrainChunkedNode* pNode = TerrainManager::GetSingletonPtr()->GetNodeFromPool();
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