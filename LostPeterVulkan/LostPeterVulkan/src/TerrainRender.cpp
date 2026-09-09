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
#include "../include/TerrainManager.h"
#include "../include/TerrainSetting.h"
#include "../include/TerrainChunked.h"
#include "../include/VKTexture.h"
#include "../include/VKBufferUniform.h"
#include "../include/VKStatePipelineGraphics.h"
#include "../include/VKBufferVertexIndex.h"

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
	TerrainRenderInstanceData* TerrainRenderInstanceData::MakeInstanceData(const TerrainRenderData* pRenderData)
	{
		TerrainRenderInstanceData* pInstance = TerrainManager::GetSingletonPtr()->GetRenderInstanceDataFromPool();
		MakeInstanceData(pInstance, pRenderData);
		return pInstance;
	}
	void TerrainRenderInstanceData::MakeInstanceData(TerrainRenderInstanceData* pInstance, const TerrainRenderData* pRenderData)
	{
		pInstance->aPatch[0] = pRenderData->fOriginX;
		pInstance->aPatch[1] = pRenderData->fOriginZ;
		pInstance->aPatch[2] = pRenderData->fSize;
		pInstance->aPatch[3] = static_cast<float>(pRenderData->nLod);
		pInstance->aLodTint[0] = pRenderData->vTint.x;
		pInstance->aLodTint[1] = pRenderData->vTint.y;
		pInstance->aLodTint[2] = pRenderData->vTint.z;
		pInstance->aLodTint[3] = 1.0f;
		for (size_t i = 0; i < 4; ++i)
		{
			pInstance->aStitchStep[i] = pRenderData->aStitchStepWorld[i];
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

	void TerrainRenderInstanceData::Init(const TerrainRenderData* pRenderData)
	{
		TerrainRenderInstanceData::MakeInstanceData(this, pRenderData);
	}


	/////////////////////////// TerrainRenderBatchData ///////////////////
	TerrainRenderBatchData::TerrainRenderBatchData()
		: nLod(0)
		, nFirstInstance(0)
		, nInstanceCount(0)
	{

	}
	TerrainRenderBatchData::~TerrainRenderBatchData()
	{
		Destroy();
	}

	void TerrainRenderBatchData::Destroy()
	{
		ClearInstanceDatas();
	}

	void TerrainRenderBatchData::Init(uint32 lod)
	{
		this->nLod = lod;
		this->nFirstInstance = 0;
		this->nInstanceCount = 0;
	}
	void TerrainRenderBatchData::Refresh(uint32 firstInstance, uint32 instanceCount)
	{
		this->nFirstInstance = firstInstance;
		this->nInstanceCount = instanceCount;
	}

	void TerrainRenderBatchData::ClearInstanceDatas()
	{
		TerrainManager* pTerrainManager = TerrainManager::GetSingletonPtr();
		size_t count = this->aInstances.size();
		for (size_t i = 0; i < count; i++)
		{
			pTerrainManager->BackRenderInstanceDataToPool(this->aInstances[i]);
		}
		this->aInstances.clear();
	}

	void TerrainRenderBatchData::AddInstanceData(const TerrainRenderData* pRenderData)
	{
		TerrainRenderInstanceData* pInstanceData = TerrainRenderInstanceData::MakeInstanceData(pRenderData);
		this->aInstances.push_back(pInstanceData);
	}
	void TerrainRenderBatchData::AddInstanceDatas(const TerrainRenderDataPtrVector& aRenderData)
	{
		for (const TerrainRenderData* pRenderData : aRenderData)
		{
			if (pRenderData->nLod != this->nLod)
			{
				continue;
			}
			AddInstanceData(pRenderData);
		}
	}

	/////////////////////////// TerrainRenderBatches /////////////////////
	TerrainRenderBatches::TerrainRenderBatches()
		: bAddBatches(false)
	{

	}
	TerrainRenderBatches::~TerrainRenderBatches()
	{

	}

	void TerrainRenderBatches::Destroy()
	{
		ClearBatches();
	}
	void TerrainRenderBatches::Init()
	{
		this->aBatches.clear();
		int lod_count = TerrainSetting::GetSingleton().GetLodCount();
		for (int i = 0; i < lod_count; i++)
		{
			TerrainRenderBatchData bd;
			bd.Init(i);
			this->aBatches.push_back(bd);
		}
	}

	void TerrainRenderBatches::BeginBatches()
	{
		SetIsAddBatches(true);
		ClearBatches();
	}
		void TerrainRenderBatches::ClearBatches()
		{
			size_t count = this->aBatches.size();
			for (size_t i = 0; i < count; i++)
			{
				TerrainRenderBatchData& batch = this->aBatches[i];
				batch.ClearInstanceDatas();
			}
			this->aInstances.clear();
		}
		void TerrainRenderBatches::AddBatches(const TerrainRenderDataPtrVector& aRenderData)
		{
			size_t count = this->aBatches.size();
			for (size_t i = 0; i < count; i++)
			{
				TerrainRenderBatchData& batch = this->aBatches[i];
				batch.AddInstanceDatas(aRenderData);
			}
		}
		void TerrainRenderBatches::SetupBatches()
		{
			size_t count_ins = 0;
			size_t count = this->aBatches.size();
			for (size_t i = 0; i < count; i++)
			{
				TerrainRenderBatchData& batch = this->aBatches[i];
				count_ins += batch.aInstances.size();
			}

			this->aInstances.reserve(count_ins);
			for (size_t i = 0; i < count; i++)
			{
				TerrainRenderBatchData& batch = this->aBatches[i];
				batch.Refresh((uint32)this->aInstances.size(), (uint32)batch.aInstances.size());
				this->aInstances.insert(this->aInstances.end(), batch.aInstances.begin(), batch.aInstances.end());
			}
		}
	void TerrainRenderBatches::EndBatches()
	{
		SetIsAddBatches(false);
		SetupBatches();
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
		: pBufferVertexIndex(nullptr)
		, pBufferVertexIndex_WireFrame(nullptr)
	{
		
	}
	TerrainRenderPatchGeometry::~TerrainRenderPatchGeometry()
	{
		Destroy();
	}

	void TerrainRenderPatchGeometry::Destroy()
	{
		destroyBufferVertexIndex();
	}
		void TerrainRenderPatchGeometry::destroyBufferVertexIndex()
		{
			F_DELETE(this->pBufferVertexIndex)
			F_DELETE(this->pBufferVertexIndex_WireFrame)
		}

	void TerrainRenderPatchGeometry::Init(int nPatchQuads)
	{
		TerrainRenderPatchGeometry::MakePatchGeometry(this, nPatchQuads);

		if (!createBufferVertexIndex())
		{
			F_LogError("*********************** TerrainRenderPatchGeometry::Init: createBufferVertexIndex failed !");
		}
	}
		bool TerrainRenderPatchGeometry::createBufferVertexIndex()
		{
			String nameBuffer = "BufferVertexIndex-TerrainRenderPatchGeometry";
			this->pBufferVertexIndex = Base::GetWindowPtr()->createBufferVertexIndex(nameBuffer,	
																					 F_MeshVertex_Pos3Color4Normal3Tex2,
                                                                                     (size_t)(sizeof(FVertex_Pos3Color4Normal3Tex2) * this->aVertices.size()), 
                                                                                     (uint8*)this->aVertices.data(), 
                                                                                     false,
                                                                                     this->aIndicesTriangle.size() * sizeof(uint32),
                                                                                     (uint8*)this->aIndicesTriangle.data(), 
                                                                                     false,
																					 false);
			if (this->pBufferVertexIndex == nullptr)
            {
                F_LogError("*********************** TerrainRenderPatchGeometry::createBufferVertexIndex: create buffer vertex index failed: [%s] !", nameBuffer.c_str());
                return false;
            }
			F_LogInfo("TerrainRenderPatchGeometry::createBufferVertexIndex: create buffer vertex index success: [%s] !", nameBuffer.c_str());

			String nameBuffer_WireFrame = "BufferVertexIndex-TerrainRenderPatchGeometry-WireFrame";
			this->pBufferVertexIndex_WireFrame = Base::GetWindowPtr()->createBufferVertexIndex(nameBuffer_WireFrame,	
																							   F_MeshVertex_Pos3Color4Normal3Tex2,
																							   (size_t)(sizeof(FVertex_Pos3Color4Normal3Tex2) * this->aVertices.size()), 
																							   (uint8*)this->aVertices.data(), 
																							   false,
																							   this->aIndicesWireFrame.size() * sizeof(uint32),
																							   (uint8*)this->aIndicesWireFrame.data(), 
																							   false,
																							   false);
			if (this->pBufferVertexIndex_WireFrame == nullptr)
            {
                F_LogError("*********************** TerrainRenderPatchGeometry::createBufferVertexIndex: create buffer vertex index wire frame failed: [%s] !", nameBuffer_WireFrame.c_str());
                return false;
            }
			F_LogInfo("TerrainRenderPatchGeometry::createBufferVertexIndex: create buffer vertex index wire frame success: [%s] !", nameBuffer_WireFrame.c_str());

			return true;
		}

	void TerrainRenderPatchGeometry::BindVertexIndexBuffer(VkCommandBuffer& commandBuffer, bool isWireFrame)
	{
		if (isWireFrame)
			this->pBufferVertexIndex_WireFrame->BindVertexIndexBuffer(commandBuffer);
		else
			this->pBufferVertexIndex->BindVertexIndexBuffer(commandBuffer);
	}

	const VkBuffer& TerrainRenderPatchGeometry::GetVkBufferVertex() const 
	{ 
		return this->pBufferVertexIndex->GetVkBufferVertex(); 
	}
	const VkDeviceMemory& TerrainRenderPatchGeometry::GetVkBufferVertexMemory() const
	{ 
		return this->pBufferVertexIndex->GetVkBufferVertexMemory(); 
	}
	const VkBuffer& TerrainRenderPatchGeometry::GetVkBufferIndex() const 
	{ 
		return this->pBufferVertexIndex->GetVkBufferIndex(); 
	}
	const VkDeviceMemory& TerrainRenderPatchGeometry::GetVkBufferIndexMemory() const 
	{ 
		return this->pBufferVertexIndex->GetVkBufferIndexMemory(); 
	}

	const VkBuffer& TerrainRenderPatchGeometry::GetVkBufferVertex_WireFrame() const
	{
		return this->pBufferVertexIndex_WireFrame->GetVkBufferVertex(); 
	}
	const VkDeviceMemory& TerrainRenderPatchGeometry::GetVkBufferVertexMemory_WireFrame() const
	{
		return this->pBufferVertexIndex_WireFrame->GetVkBufferVertexMemory(); 
	}
	const VkBuffer& TerrainRenderPatchGeometry::GetVkBufferIndex_WireFrame() const
	{
		return this->pBufferVertexIndex_WireFrame->GetVkBufferIndex(); 
	}
	const VkDeviceMemory& TerrainRenderPatchGeometry::GetVkBufferIndexMemory_WireFrame() const
	{
		return this->pBufferVertexIndex_WireFrame->GetVkBufferIndexMemory(); 
	}


	/////////////////////////// TerrainRender ////////////////////////////
	TerrainRenderPatchGeometry* TerrainRender::s_pPatchGeometry = nullptr;
	TerrainRenderBatches* TerrainRender::s_pRenderBatches = nullptr;
	int TerrainRender::s_nRenderInstanceMaxCount = 2048;
	const String TerrainRender::s_nameDescriptorSetLayout = "Pass-ObjectTerrain-Material-Instance-Terrain-TextureVS-TextureVS-TextureFS-TextureFS-TextureFS";
	const String TerrainRender::s_nameShaderVertex = "vert_standard_terrain_chunked_lit";
	const String TerrainRender::s_nameShaderFragment = "frag_standard_terrain_chunked_lit";
	DescriptorSetLayout* TerrainRender::s_pDescriptorSetLayout = nullptr;
	VkPipelineShaderStageCreateInfoVector TerrainRender::s_shaderStageCreateInfo;

	bool TerrainRender::InitStatic(int nPatchQuads)
	{
		VulkanWindow* pWindow = Base::GetWindowPtr();

		//s_pPatchGeometry
		if (s_pPatchGeometry == nullptr)
		{
			s_pPatchGeometry = new TerrainRenderPatchGeometry();
			s_pPatchGeometry->Init(nPatchQuads);
		}
		
		//s_pRenderBatches
		if (TerrainSetting::GetSingleton().GetIsGPUCullingAll())
		{
			if (s_pRenderBatches == nullptr)
			{
				s_pRenderBatches = new TerrainRenderBatches();
				s_pRenderBatches->Init();
			}
		}

		//s_pDescriptorSetLayout
		s_pDescriptorSetLayout = pWindow->FindDescriptorSetLayout_Internal(s_nameDescriptorSetLayout);
		if (s_pDescriptorSetLayout == nullptr)
		{
			F_LogError("*********************** TerrainRender::InitStatic: Can not find DescriptorSetLayout: [%s] !", s_nameDescriptorSetLayout.c_str());
		}

		//s_shaderStageCreateInfo
		if (!pWindow->CreatePipelineShaderStageCreateInfos(s_nameShaderVertex,
														   "",
														   "",
														   "",
														   s_nameShaderFragment,
														   pWindow->m_mapShaders_Internal,
														   s_shaderStageCreateInfo))
		{
			F_LogError("*********************** TerrainRender::InitStatic: Can not find shader vertex: [%s], fragment: [%s] !", s_nameShaderVertex.c_str(), s_nameShaderFragment.c_str());
		}
			
		return true;
	}
	void TerrainRender::DestroyStatic()
	{
		F_DELETE(s_pRenderBatches)
		F_DELETE(s_pPatchGeometry)
	}

	void TerrainRender::BeginRenderBatches()
	{
		if (!s_pRenderBatches)
			return;

		s_pRenderBatches->BeginBatches();
	}
	void TerrainRender::EndRenderBatches()
	{
		if (!s_pRenderBatches)
			return;

		s_pRenderBatches->EndBatches();
	}

	void TerrainRender::RenderBatches(VkCommandBuffer& commandBuffer)
	{
		if (!s_pRenderBatches)
			return;


	}


	TerrainRender::TerrainRender(const String& nameRender)
		: Base(nameRender)

		, pChunked(nullptr)
		, pRenderBatches(nullptr)

		, bAddRenderDatas(false)

		, poStatePipelineGraphics(nullptr)

        , poBuffer_TerrainObjectCB(nullptr)
        , poBuffer_MaterialCB(nullptr)
        , poBuffer_TerrainCB(nullptr)

	{
		
	}
    TerrainRender::~TerrainRender()
	{
		Destroy();
	}

	void TerrainRender::Destroy()
	{
		ClearRenderDatas();
		CleanupSwapChain();

		destroyRenderBatches();
		destroyBufferTerrainObject();
		destroyBufferMaterial();
		destroyBufferTerrain();
	}
		void TerrainRender::destroyRenderBatches()
		{
			F_DELETE(this->pRenderBatches)
		}
		void TerrainRender::destroyBufferTerrainObject()
		{
			this->terrainObjectCBs.clear();
			F_DELETE(this->poBuffer_TerrainObjectCB)
		}
        void TerrainRender::destroyBufferMaterial()
		{
			this->materialCBs.clear();
			F_DELETE(this->poBuffer_MaterialCB)
		}
        void TerrainRender::destroyBufferTerrain()
		{
			F_DELETE(this->poBuffer_TerrainCB)
		}

	bool TerrainRender::Init(TerrainChunked* pChunked)
	{
		this->pChunked = pChunked;

		VulkanWindow* pWindow = Base::GetWindowPtr();

		if (!TerrainSetting::GetSingleton().GetIsGPUCullingAll())
		{
			//1> createRenderBatches
			if (!createRenderBatches())
			{
				F_LogError("*********************** TerrainRender::Init: createRenderBatches: [%d, %d] failed !", pChunked->GetChunkedX(), pChunked->GetChunkedZ());
				return false;
			}

			//2> Buffer
			if (this->poBuffer_TerrainObjectCB == nullptr)
			{
				if (!createBufferTerrainObject())
				{
					F_LogError("*********************** TerrainRender::Init: createBufferTerrainObject failed !");
					return false;
				}
			}
			if (this->poBuffer_MaterialCB == nullptr)
			{
				if (!createBufferMaterial())
				{
					F_LogError("*********************** TerrainRender::Init: createBufferMaterial failed !");
					return false;
				}
			}
			if (this->poBuffer_TerrainCB == nullptr)
			{
				if (!createBufferTerrain())
				{
					F_LogError("*********************** TerrainRender::Init: createBufferTerrain failed !");
					return false;
				}
			}

			//3> Pipeline
			{
				if (!createPipelineTerrain())
				{
					F_LogError("*********************** TerrainRender::Init: createPipelineTerrain failed !");
					return false;
				}
			}

			//4> DescriptorSet
        	UpdateDescriptorSets();
		}

		return true;
	}
		bool TerrainRender::createRenderBatches()
		{
			if (this->pRenderBatches == nullptr)
			{
				this->pRenderBatches = new TerrainRenderBatches();
				this->pRenderBatches->Init();
			}

			return true;
		}
		bool TerrainRender::createBufferTerrainObject()
		{
			VulkanWindow* pWindow = Base::GetWindowPtr();

			this->terrainObjectCBs.clear();
			this->terrainObjectCBs.resize(s_nRenderInstanceMaxCount);
            VkDeviceSize bufferSize = sizeof(TerrainChunkedObjecctInstanceConstants) * this->terrainObjectCBs.size();
			String nameBuffer = "TerrainChunkedObjecctInstanceConstants-" + this->name;
			this->poBuffer_TerrainObjectCB = pWindow->createBufferUniform(nameBuffer,
																		  sizeof(TerrainChunkedObjecctInstanceConstants) * this->terrainObjectCBs.size(), 
																		  (uint8*)this->terrainObjectCBs.data(),
																		  false);
			if (!this->poBuffer_TerrainObjectCB)
			{
				String msg = "*********************** TerrainRender::createBufferTerrainObject: create buffer uniform: [" + nameBuffer + "] failed !";
				F_LogError(msg.c_str());
				throw std::runtime_error(msg);
			}
            return true;
		}
        bool TerrainRender::createBufferMaterial()
		{
			VulkanWindow* pWindow = Base::GetWindowPtr();

			this->materialCBs.clear();
            for (int i = 0; i < MAX_MATERIAL_COUNT; i++)
            {
                MaterialConstants mc;
                this->materialCBs.push_back(mc);
            }
			String nameBuffer = "MaterialConstants-" + this->name;
			this->poBuffer_MaterialCB = pWindow->createBufferUniform(nameBuffer,
																	 sizeof(MaterialConstants) * this->materialCBs.size(), 
																	 (uint8*)this->materialCBs.data(),
																	 false);
			if (!this->poBuffer_MaterialCB)
			{
				String msg = "*********************** TerrainRender::createBufferMaterial: create buffer uniform: [" + nameBuffer + "] failed !";
				F_LogError(msg.c_str());
				throw std::runtime_error(msg);
			}
			return true;
		}
        bool TerrainRender::createBufferTerrain()
		{
			VulkanWindow* pWindow = Base::GetWindowPtr();
			TerrainSetting* pSetting = TerrainSetting::GetSingletonPtr();

            this->terrainCB.textureX = (float)pSetting->GetResolution();
            this->terrainCB.textureZ = (float)pSetting->GetResolution();
            this->terrainCB.textureX_Inverse = 1.0f / this->terrainCB.textureX;
            this->terrainCB.textureZ_Inverse = 1.0f / this->terrainCB.textureZ;
            this->terrainCB.heightStart = pWindow->cfg_terrainHeightStart;
            this->terrainCB.heightMax = pWindow->cfg_terrainHeightMax;
            this->terrainCB.terrainSizeX = pSetting->GetTerrainSize();
            this->terrainCB.terrainSizeZ = pSetting->GetTerrainSize();

			String nameBuffer = "TerrainConstants-" + this->name;
			this->poBuffer_TerrainCB = pWindow->createBufferUniform(nameBuffer,
																	sizeof(TerrainConstants), 
																	(uint8*)&this->terrainCB,
																	false);
			if (!this->poBuffer_TerrainCB)
			{
				String msg = "*********************** TerrainRender::createBufferTerrain: create buffer uniform: [" + nameBuffer + "] failed !";
				F_LogError(msg.c_str());
				throw std::runtime_error(msg);
			}
			return true;
		}
		bool TerrainRender::createPipelineTerrain()
		{
			VulkanWindow* pWindow = Base::GetWindowPtr();

			VkViewportVector aViewports;
			aViewports.push_back(pWindow->poViewport);
			VkRect2DVector aScissors;
			aScissors.push_back(pWindow->poScissor);
			VkDynamicStateVector aDynamicStates =
			{
				VK_DYNAMIC_STATE_VIEWPORT,
				VK_DYNAMIC_STATE_SCISSOR
			};

			VkPrimitiveTopology vkPrimitiveTopology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			VkFrontFace vkFrontFace = VK_FRONT_FACE_CLOCKWISE;
			VkPolygonMode vkPolygonMode = VK_POLYGON_MODE_FILL;
			VkCullModeFlagBits vkCullModeFlagBits = VK_CULL_MODE_BACK_BIT;
			VkBool32 depthBiasEnable = VK_FALSE;
			float depthBiasConstantFactor = 0.0f;
			float depthBiasClamp = 0.0f;
			float depthBiasSlopeFactor = 0.0f;
			float lineWidth = 1.0f;
			VkBool32 isDepthTest = VK_TRUE;
			VkBool32 isDepthWrite = VK_TRUE; 
			VkCompareOp vkDepthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL; 
			VkBool32 isStencilTest = VK_FALSE;
			VkStencilOpState vkStencilOpFront; 
			VkStencilOpState vkStencilOpBack; 
			VkBool32 isBlend = VK_FALSE;
			VkBlendFactor vkBlendColorFactorSrc = VK_BLEND_FACTOR_ONE; 
			VkBlendFactor vkBlendColorFactorDst = VK_BLEND_FACTOR_ZERO; 
			VkBlendOp vkBlendColorOp = VK_BLEND_OP_ADD;
			VkBlendFactor vkBlendAlphaFactorSrc = VK_BLEND_FACTOR_ONE;
			VkBlendFactor vkBlendAlphaFactorDst = VK_BLEND_FACTOR_ZERO; 
			VkBlendOp vkBlendAlphaOp = VK_BLEND_OP_ADD;
			VkColorComponentFlags vkColorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

			String namePipeline = "PipelineGraphics-" + this->name;
			this->poStatePipelineGraphics = pWindow->createStatePipelineGraphics(namePipeline,
																				 s_pDescriptorSetLayout,
																				 s_shaderStageCreateInfo,
																				 F_MeshVertex_Pos3Color4Normal3Tex2,
																				 false, 0, 0,
																				 pWindow->poRenderPass, aViewports, aScissors, aDynamicStates,
																				 vkPrimitiveTopology, vkFrontFace, vkPolygonMode, vkCullModeFlagBits, depthBiasEnable, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor, lineWidth,
																				 VK_TRUE, isDepthTest, isDepthWrite, vkDepthCompareOp,
																				 isStencilTest, vkStencilOpFront, vkStencilOpBack, 
																				 isBlend, vkBlendColorFactorSrc, vkBlendColorFactorDst, vkBlendColorOp,
																				 vkBlendAlphaFactorSrc, vkBlendAlphaFactorDst, vkBlendAlphaOp,
																				 vkColorWriteMask);
			if (this->poStatePipelineGraphics == nullptr)
			{
				F_LogError("*********************** TerrainRender::createPipelineTerrain: Create terrain pipeline graphics: [%s] failed !", namePipeline.c_str());
				return false;
			}
			F_LogInfo("TerrainRender::createPipelineTerrain: Create terrain pipeline graphics: [%s] success !", namePipeline.c_str());
			return true;
		}

	void TerrainRender::Render(VkCommandBuffer& commandBuffer)
	{
		if (!this->pRenderBatches)
			return;

		VulkanWindow* pWindow = Base::GetWindowPtr();

		s_pPatchGeometry->BindVertexIndexBuffer(commandBuffer, pWindow->cfg_isWireFrame);

		//State/Shader/BufferUniform/Texture
		this->poStatePipelineGraphics->BindState(commandBuffer, pWindow->cfg_isWireFrame);
		this->poStatePipelineGraphics->BindShader(commandBuffer);
		this->poStatePipelineGraphics->BindBufferUniforms(commandBuffer);
		this->poStatePipelineGraphics->BindTextures(commandBuffer);

		const TerrainRenderBatchDataVector& aBatches = this->pRenderBatches->GetBatches();
		int count_batches = (int)aBatches.size();
		for (int i = 0; i < count_batches; i++)
		{
			const TerrainRenderBatchData& batch = aBatches[i];
			uint32 countInstance = (uint32)batch.aInstances.size();
			if (countInstance <= 0)
				continue;

			IndexRange* pRange = s_pPatchGeometry->GetRangesTrianglePtr(i);
			if (pWindow->cfg_isWireFrame)
				pRange = s_pPatchGeometry->GetRangesWireFramePtr(i);
			pWindow->drawIndexed(commandBuffer, pRange->nIndexCount, countInstance, pRange->nFirstIndex, 0, 0);
		}
		this->poStatePipelineGraphics->UnBindState(commandBuffer);
	}

	void TerrainRender::CleanupSwapChain()
	{
		F_DELETE(this->poStatePipelineGraphics)
	}
    void TerrainRender::UpdateDescriptorSets()
	{
		VulkanWindow* pWindow = Base::GetWindowPtr();

		size_t count = this->poStatePipelineGraphics->poDescriptorSets.size();
        for (size_t i = 0; i < count; i++)
        {
            VkWriteDescriptorSetVector descriptorWrites;
            //<0> PassConstants
            {
                VkDescriptorBufferInfo bufferInfo_Pass = {};
                bufferInfo_Pass.buffer = pWindow->poBuffers_PassCB[i]->GetVkBuffer();
                bufferInfo_Pass.offset = 0;
                bufferInfo_Pass.range = sizeof(PassConstants);
                pWindow->pushVkDescriptorSet_Uniform(descriptorWrites,
													 this->poStatePipelineGraphics->poDescriptorSets[i],
													 0,
													 0,
													 1,
													 bufferInfo_Pass);
            }
            //<1> TerrainChunkedObjecctInstanceConstants
            {
                VkDescriptorBufferInfo bufferInfo_TerrainObject = {};
                bufferInfo_TerrainObject.buffer = this->poBuffer_TerrainObjectCB->GetVkBuffer();
                bufferInfo_TerrainObject.offset = 0;
                bufferInfo_TerrainObject.range = sizeof(TerrainChunkedObjecctInstanceConstants) * this->terrainObjectCBs.size();
                pWindow->pushVkDescriptorSet_Uniform(descriptorWrites,
													 this->poStatePipelineGraphics->poDescriptorSets[i],
													 1,
													 0,
													 1,
													 bufferInfo_TerrainObject);
            }
            //<2> MaterialConstants
            {
                VkDescriptorBufferInfo bufferInfo_Material = {};
                bufferInfo_Material.buffer = this->poBuffer_MaterialCB->GetVkBuffer();
                bufferInfo_Material.offset = 0;
                bufferInfo_Material.range = sizeof(MaterialConstants) * this->materialCBs.size();
                pWindow->pushVkDescriptorSet_Uniform(descriptorWrites,
													 this->poStatePipelineGraphics->poDescriptorSets[i],
													 2,
													 0,
													 1,
													 bufferInfo_Material);
            }
            //<3> InstanceConstants
            {
                // VkDescriptorBufferInfo bufferInfo_Instance = {};
                // bufferInfo_Instance.buffer = this->poBuffers_InstanceCB[i];
                // bufferInfo_Instance.offset = 0;
                // bufferInfo_Instance.range = sizeof(InstanceConstants) * this->instanceCBs.size();
                // pWindow->pushVkDescriptorSet_Uniform(descriptorWrites,
                //                                      this->poStatePipelineGraphics->poDescriptorSets[i],
                //                                      3,
                //                                      0,
                //                                      1,
                //                                      bufferInfo_Instance);
            }
            //<4> Terrain
            {
                VkDescriptorBufferInfo bufferInfo_Terrain = {};
                bufferInfo_Terrain.buffer = this->poBuffer_TerrainCB->GetVkBuffer();
                bufferInfo_Terrain.offset = 0;
                bufferInfo_Terrain.range = sizeof(TerrainConstants);
                pWindow->pushVkDescriptorSet_Uniform(descriptorWrites,
													 this->poStatePipelineGraphics->poDescriptorSets[i],
													 4,
													 0,
													 1,
													 bufferInfo_Terrain);
            }
            //<5> pTexture_HeightMap
            {
                pWindow->pushVkDescriptorSet_Image(descriptorWrites,
												   this->poStatePipelineGraphics->poDescriptorSets[i],
												   5,
												   0,
												   1,
												   VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
												   this->pChunked->pTexture_HeightMap->GetVkDescriptorImageInfo_NoSampler());
            }
            //<6> pTexture_NormalMap
            {
                pWindow->pushVkDescriptorSet_Image(descriptorWrites,
												   this->poStatePipelineGraphics->poDescriptorSets[i],
												   6,
												   0,
												   1,
												   VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
												   this->pChunked->pTexture_NormalMap->GetVkDescriptorImageInfo_NoSampler());
            }
            //<7> pTexture_Diffuse
            {
                pWindow->pushVkDescriptorSet_Image(descriptorWrites,
												   this->poStatePipelineGraphics->poDescriptorSets[i],
											 	   7,
												   0,
												   1,
												   VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
												   this->pChunked->pTexture_Diffuse->GetVkDescriptorImageInfo());
            }
            //<8> pTexture_Normal
            {
                pWindow->pushVkDescriptorSet_Image(descriptorWrites,
												   this->poStatePipelineGraphics->poDescriptorSets[i],
												   8,
												   0,
												   1,
												   VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
												   this->pChunked->pTexture_Normal->GetVkDescriptorImageInfo());
            }
            //<9> pTexture_Control
            {
                pWindow->pushVkDescriptorSet_Image(descriptorWrites,
												   this->poStatePipelineGraphics->poDescriptorSets[i],
											  	   9,
												   0,
												   1,
												   VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
												   this->pChunked->pTexture_Control->GetVkDescriptorImageInfo());
            }
            pWindow->updateVkDescriptorSets(descriptorWrites);
        }
	}
	void TerrainRender::UpdateBufferTerrainObject()
	{
		if (!this->pRenderBatches)
			return;

		int count_ins = (int)this->pRenderBatches->aInstances.size();
		if (count_ins > s_nRenderInstanceMaxCount)
		{
			F_LogError("*********************** TerrainRender::UpdateBufferTerrainObject: Render terrain chunked: [%s], instance count: [%d] is bigger than max: [%d] !", this->name.c_str(), count_ins, s_nRenderInstanceMaxCount);
			return;
		}

		for (int i = 0; i < count_ins; i++)
		{
			TerrainRenderInstanceData* pInstance = this->pRenderBatches->aInstances[i];
			TerrainChunkedObjecctInstanceConstants& insConst = this->terrainObjectCBs[i];
			memcpy(&insConst.vPatch, pInstance->aPatch, sizeof(float) * 4);
			memcpy(&insConst.vLodTint, pInstance->aLodTint, sizeof(float) * 4);
			memcpy(&insConst.vStitchStep, pInstance->aStitchStep, sizeof(float) * 4);
		}
		this->poBuffer_TerrainObjectCB->UpdateBuffer(0,
													 sizeof(TerrainChunkedObjecctInstanceConstants) * count_ins,
													 (uint8*)this->terrainObjectCBs.data());
	}
	void TerrainRender::UpdateBufferTerrain()
	{
		this->poBuffer_TerrainCB->UpdateBuffer(0,
											   sizeof(TerrainConstants), 
											   (uint8*)&this->terrainCB);
	}

	void TerrainRender::BeginAddRenderDatas()
	{
		SetIsAddRenderDatas(true);
		ClearRenderDatas();
	}
		void TerrainRender::ClearRenderDatas()
		{
			TerrainManager* pTerrainManager = TerrainManager::GetSingletonPtr();
			for (TerrainRenderDataPtrVector::iterator it = this->aRenderDatas.begin();
				 it != this->aRenderDatas.end(); ++it)
			{
				pTerrainManager->BackRenderDataToPool(*it);
			}
			this->aRenderDatas.clear();
		}
		void TerrainRender::AddRenderData(TerrainRenderData* pRenderData)
		{
			this->aRenderDatas.push_back(pRenderData);
		}
		void TerrainRender::AddRenderDatas(const TerrainRenderDataPtrVector& aRDs)
		{
			this->aRenderDatas.insert(this->aRenderDatas.end(), aRDs.begin(), aRDs.end());
		}
	void TerrainRender::EndAddRenderDatas()
	{	
		SetIsAddRenderDatas(false);
		if (this->aRenderDatas.size() > 0)
		{
			if (TerrainSetting::GetSingleton().GetIsGPUCullingAll())
			{
				s_pRenderBatches->AddBatches(this->aRenderDatas);
			}
			else
			{
				pRenderBatches->AddBatches(this->aRenderDatas);
				UpdateBufferTerrainObject();
			}
		}
	}

}; //LostPeterVulkan