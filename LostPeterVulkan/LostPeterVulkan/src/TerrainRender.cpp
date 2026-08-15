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

#include "../include/TerrainRender.h"
#include "../include/VulkanWindow.h"

namespace LostPeterVulkan
{
	/////////////////////////// TerrainRenderData ////////////////////////
	TerrainRenderData::TerrainRenderData()
		: fOriginX(0.0f)
		, fOriginZ(0.0f)
		, fSize(1.0f)
		, nLod(0)
		, vTint(1.0f, 1.0f, 1.0f)
	{
		this->aStitchStepWorld[0] = 0.0f;
		this->aStitchStepWorld[1] = 0.0f;
		this->aStitchStepWorld[2] = 0.0f;
		this->aStitchStepWorld[3] = 0.0f;
	}
	TerrainRenderData::TerrainRenderData(float oriX, 
										 float oriZ, 
										 float size,
										 uint32 lod,
										 const FVector3& tint,
										 float stitchStepWorld0,
										 float stitchStepWorld1,
										 float stitchStepWorld2,
										 float stitchStepWorld3)
	{
		Init(oriX, 
			 oriZ, 
			 size,
			 lod,
			 tint,
			 stitchStepWorld0,
			 stitchStepWorld1,
			 stitchStepWorld2,
			 stitchStepWorld3);
	}
	TerrainRenderData::~TerrainRenderData()
	{

	}

	void TerrainRenderData::Init(float oriX, 
								 float oriZ, 
								 float size,
								 uint32 lod,
								 const FVector3& tint,
								 float stitchStepWorld[4])
	{
		Init(oriX, 
			 oriZ, 
			 size,
			 lod,
			 tint,
			 stitchStepWorld[0],
			 stitchStepWorld[1],
			 stitchStepWorld[2],
			 stitchStepWorld[3]);
	}
	void TerrainRenderData::Init(float oriX, 
								 float oriZ, 
								 float size,
								 uint32 lod,
								 const FVector3& tint,
								 float stitchStepWorld0,
								 float stitchStepWorld1,
								 float stitchStepWorld2,
								 float stitchStepWorld3)
	{
		this->fOriginX = oriX;
		this->fOriginZ = oriZ;
		this->fSize = size;
		this->nLod = lod;
		this->vTint = tint;
		this->aStitchStepWorld[0] = stitchStepWorld0;
		this->aStitchStepWorld[1] = stitchStepWorld1;
		this->aStitchStepWorld[2] = stitchStepWorld2;
		this->aStitchStepWorld[3] = stitchStepWorld3;
	}

	/////////////////////////// TerrainRenderInstanceData ////////////////
	TerrainRenderInstanceData TerrainRenderInstanceData::MakeInstanceData(const TerrainRenderData& renderData)
	{
		TerrainRenderInstanceData instance;
		instance.Init(renderData);
		return instance;
	}
	void TerrainRenderInstanceData::MakeInstanceData(TerrainRenderInstanceData* pInstance, const TerrainRenderData& renderData)
	{
		pInstance->aPatch[0] = renderData.fOriginX;
		pInstance->aPatch[1] = renderData.fOriginZ;
		pInstance->aPatch[2] = renderData.fSize;
		pInstance->aPatch[3] = static_cast<float>(renderData.nLod);
		pInstance->aLodTint[0] = renderData.vTint.x;
		pInstance->aLodTint[1] = renderData.vTint.y;
		pInstance->aLodTint[2] = renderData.vTint.z;
		pInstance->aLodTint[3] = 1.0f;
		for (size_t i = 0; i < 4; ++i)
		{
			pInstance->aStitchStep[i] = renderData.aStitchStepWorld[i];
		}
	}

	TerrainRenderInstanceData::TerrainRenderInstanceData()
	{
		Clear();
	}
	TerrainRenderInstanceData::~TerrainRenderInstanceData()
	{

	}

	void TerrainRenderInstanceData::Clear()
	{
		for (int i = 0; i < 4; i++)
		{
			this->aPatch[i] = 0.0f;
			this->aLodTint[i] = 0.0f;
			this->aStitchStep[i] = 0.0f;
		}
	}

	void TerrainRenderInstanceData::Init(const TerrainRenderData& renderData)
	{
		TerrainRenderInstanceData::MakeInstanceData(this, renderData);
	}


	/////////////////////////// TerrainRenderBatchData ///////////////////
	TerrainRenderBatchData::TerrainRenderBatchData()
	{
		Clear();
	}
	TerrainRenderBatchData::~TerrainRenderBatchData()
	{

	}

	void TerrainRenderBatchData::Clear()
	{
		this->nLod = 0;
		this->nFirstInstance = 0;
		this->nInstanceCount = 0;
	}

	void TerrainRenderBatchData::Init(uint32 lod, uint32 firstInstance, uint32 instanceCount)
	{
		this->nLod = lod;
		this->nFirstInstance = firstInstance;
		this->nInstanceCount = instanceCount;
	}


	/////////////////////////// TerrainRenderBatches /////////////////////
	TerrainRenderBatches TerrainRenderBatches::MakeBatches(const TerrainRenderDataVector& aRenderData)
	{
		TerrainRenderBatches batches;
		batches.Init(aRenderData);
		return batches;
	}
	void TerrainRenderBatches::MakeBatches(TerrainRenderBatches* pBatches, const TerrainRenderDataVector& aRenderData)
	{
		pBatches->aInstances.reserve(aRenderData.size());
		for (uint32 lod = 0; lod < pBatches->aBatches.size(); ++lod)
		{
			for (const TerrainRenderData& renderData : aRenderData)
			{
				if (std::min<uint32>(renderData.nLod, 2u) != lod)
				{
					continue;
				}
				pBatches->aInstances.push_back(TerrainRenderInstanceData::MakeInstanceData(renderData));
			}

			TerrainRenderBatchData batch;
			batch.Init(lod, static_cast<uint32>(pBatches->aInstances.size()), static_cast<uint32>(pBatches->aInstances.size()) - batch.nFirstInstance);
			pBatches->aBatches[lod] = batch;
		}
	}

	TerrainRenderBatches::TerrainRenderBatches()
	{
		Clear();
	}
	TerrainRenderBatches::~TerrainRenderBatches()
	{

	}

	void TerrainRenderBatches::Clear()
	{
		this->aInstances.clear();
	}

	void TerrainRenderBatches::Init(const TerrainRenderDataVector& aRenderData)
	{
		TerrainRenderBatches::MakeBatches(this, aRenderData);

	}

	uint32 TerrainRenderBatches::GetActiveBatchCount() const
	{
		uint32 count = 0;
        for (const TerrainRenderBatchData& batch : this->aBatches)
        {
            if (batch.nInstanceCount > 0)
            {
                ++count;
            }
        }
        return count;
	}


	/////////////////////////// TerrainRenderPatchGeometry ///////////////
	TerrainRenderPatchGeometry TerrainRenderPatchGeometry::MakePatchGeometry(int nPatchQuads)
	{
		TerrainRenderPatchGeometry patchGeometry;
		patchGeometry.Init(nPatchQuads);
		return patchGeometry;
	}
	void TerrainRenderPatchGeometry::MakePatchGeometry(TerrainRenderPatchGeometry* pPatchGeometry, int nPatchQuads)
	{	
		F_Assert(nPatchQuads > 0 && "TerrainRenderPatchGeometry::MakePatchGeometry")
		
		pPatchGeometry->aVertices.reserve(static_cast<size_t>(nPatchQuads + 1) * static_cast<size_t>(nPatchQuads + 1));

		for (int z = 0; z <= nPatchQuads; ++z)
		{
			for (int x = 0; x <= nPatchQuads; ++x)
			{
				const float u = static_cast<float>(x) / static_cast<float>(nPatchQuads);
				const float v = static_cast<float>(z) / static_cast<float>(nPatchQuads);

				FVertex_Pos3Color4Normal3Tex2 vertex;
				vertex.pos = FVector3(x, 0, z);
				vertex.color = FMath::ms_clWhite;
				vertex.normal = FMath::ms_v3UnitY;
				vertex.texCoord = FVector2(u, v);
				pPatchGeometry->aVertices.push_back(vertex);
			}
		}

		const int row = nPatchQuads + 1;
		std::vector<uint32> northSkirt(static_cast<size_t>(row));
		std::vector<uint32> southSkirt(static_cast<size_t>(row));
		std::vector<uint32> westSkirt(static_cast<size_t>(row));
		std::vector<uint32> eastSkirt(static_cast<size_t>(row));

		for (int x = 0; x <= nPatchQuads; ++x)
		{
			const float u = static_cast<float>(x) / static_cast<float>(nPatchQuads);

			northSkirt[static_cast<size_t>(x)] = static_cast<uint32>(pPatchGeometry->aVertices.size());
			FVertex_Pos3Color4Normal3Tex2 vertex0;
			vertex0.pos = FVector3(x, 0.0f, 1.0f);
			vertex0.color = FMath::ms_clWhite;
			vertex0.normal = FMath::ms_v3UnitY;
			vertex0.texCoord = FVector2(u, 0.0f);
			pPatchGeometry->aVertices.push_back(vertex0);

			southSkirt[static_cast<size_t>(x)] = static_cast<uint32>(pPatchGeometry->aVertices.size());
			FVertex_Pos3Color4Normal3Tex2 vertex1;
			vertex1.pos = FVector3(x, 1.0f, 1.0f);
			vertex1.color = FMath::ms_clWhite;
			vertex1.normal = FMath::ms_v3UnitY;
			vertex1.texCoord = FVector2(u, 1.0f);
			pPatchGeometry->aVertices.push_back(vertex1);
		}

		for (int z = 0; z <= nPatchQuads; ++z)
		{
			const float v = static_cast<float>(z) / static_cast<float>(nPatchQuads);

			westSkirt[static_cast<size_t>(z)] = static_cast<uint32>(pPatchGeometry->aVertices.size());
			FVertex_Pos3Color4Normal3Tex2 vertex0;
			vertex0.pos = FVector3(0.0f, 0.0f, z);
			vertex0.color = FMath::ms_clWhite;
			vertex0.normal = FMath::ms_v3UnitY;
			vertex0.texCoord = FVector2(0.0f, v);
			pPatchGeometry->aVertices.push_back(vertex0);

			eastSkirt[static_cast<size_t>(z)] = static_cast<uint32>(pPatchGeometry->aVertices.size());
			FVertex_Pos3Color4Normal3Tex2 vertex1;
			vertex1.pos = FVector3(1.0f, 0.0f, z);
			vertex1.color = FMath::ms_clWhite;
			vertex1.normal = FMath::ms_v3UnitY;
			vertex1.texCoord = FVector2(1.0f, v);
			pPatchGeometry->aVertices.push_back(vertex1);
		}

		auto topIndex = [row](int x, int z) {
			return static_cast<uint32>(z * row + x);
		};
		auto appendSkirtQuad = [pPatchGeometry](uint32 top0, uint32 top1, uint32 skirt0, uint32 skirt1) {
			pPatchGeometry->aIndicesTriangle.insert(pPatchGeometry->aIndicesTriangle.end(), {top0, skirt0, top1, top1, skirt0, skirt1});
		};

		for (uint32 lod = 0; lod < pPatchGeometry->aRangesTriangle.size(); ++lod)
		{
			const int stride = 1 << static_cast<int>(lod);
			const uint32 firstIndex = static_cast<uint32>(pPatchGeometry->aIndicesTriangle.size());

			for (int z = 0; z < nPatchQuads; z += stride)
			{
				for (int x = 0; x < nPatchQuads; x += stride)
				{
					const uint32 i0 = static_cast<uint32>(z * row + x);
					const uint32 i1 = static_cast<uint32>(z * row + x + stride);
					const uint32 i2 = static_cast<uint32>((z + stride) * row + x);
					const uint32 i3 = static_cast<uint32>((z + stride) * row + x + stride);
					pPatchGeometry->aIndicesTriangle.insert(pPatchGeometry->aIndicesTriangle.end(), {i0, i2, i1, i1, i2, i3});
				}
			}

			for (int x = 0; x < nPatchQuads; x += stride)
			{
				appendSkirtQuad(topIndex(x, 0),
								topIndex(x + stride, 0),
								northSkirt[static_cast<size_t>(x)],
								northSkirt[static_cast<size_t>(x + stride)]);
				appendSkirtQuad(topIndex(x + stride, nPatchQuads),
								topIndex(x, nPatchQuads),
								southSkirt[static_cast<size_t>(x + stride)],
								southSkirt[static_cast<size_t>(x)]);
			}

			for (int z = 0; z < nPatchQuads; z += stride)
			{
				appendSkirtQuad(topIndex(0, z + stride),
								topIndex(0, z),
								westSkirt[static_cast<size_t>(z + stride)],
								westSkirt[static_cast<size_t>(z)]);
				appendSkirtQuad(topIndex(nPatchQuads, z),
								topIndex(nPatchQuads, z + stride),
								eastSkirt[static_cast<size_t>(z)],
								eastSkirt[static_cast<size_t>(z + stride)]);
			}

			pPatchGeometry->aRangesTriangle[lod] = {
				firstIndex,
				static_cast<uint32>(pPatchGeometry->aIndicesTriangle.size()) - firstIndex,
			};
			pPatchGeometry->aRangesWireFrame[lod] = AppendWireframeRange(pPatchGeometry->aIndicesTriangle,
																		 pPatchGeometry->aRangesTriangle[lod],
																		 pPatchGeometry->aIndicesWireFrame);
		}
	}
	IndexRange TerrainRenderPatchGeometry::AppendWireframeRange(const std::vector<uint32>& aTriangleIndices, IndexRange triangleRange, std::vector<uint32>& aLineIndices)
	{
		const uint32 firstLineIndex = static_cast<uint32>(aLineIndices.size());
		const uint32 end = triangleRange.nFirstIndex + triangleRange.nIndexCount;
		aLineIndices.reserve(aLineIndices.size() + static_cast<size_t>(triangleRange.nIndexCount) * 2u);

		for (uint32 i = triangleRange.nFirstIndex; i + 2 < end; i += 3)
		{
			const uint32 i0 = aTriangleIndices[i + 0];
			const uint32 i1 = aTriangleIndices[i + 1];
			const uint32 i2 = aTriangleIndices[i + 2];
			aLineIndices.insert(aLineIndices.end(), {i0, i1, i1, i2, i2, i0});
		}

		return IndexRange(firstLineIndex, static_cast<uint32>(aLineIndices.size()) - firstLineIndex);
	}

	TerrainRenderPatchGeometry::TerrainRenderPatchGeometry()
	{
		Clear();
	}
	TerrainRenderPatchGeometry::~TerrainRenderPatchGeometry()
	{

	}

	void TerrainRenderPatchGeometry::Clear()
	{

	}

	void TerrainRenderPatchGeometry::Init(int nPatchQuads)
	{
		TerrainRenderPatchGeometry::MakePatchGeometry(this, nPatchQuads);

	}


	/////////////////////////// TerrainRender ////////////////////////////
	TerrainRender::TerrainRender(const String& nameRender)
		: Base(nameRender)

		
	{
		
	}
    TerrainRender::~TerrainRender()
	{
		
	}



}; //LostPeterVulkan