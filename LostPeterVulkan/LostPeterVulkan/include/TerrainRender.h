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

#ifndef _TERRAIN_RENDER_H_
#define _TERRAIN_RENDER_H_

#include "Base.h"

namespace LostPeterVulkan
{
	/////////////////////////// TerrainRenderData ////////////////////////
	class vulkanExport TerrainRenderData
	{
	public:
		TerrainRenderData();
		TerrainRenderData(float oriX, 
						  float oriZ, 
						  float size,
						  uint32 lod,
						  const FVector3& tint,
						  float stitchStepWorld0,
						  float stitchStepWorld1,
						  float stitchStepWorld2,
						  float stitchStepWorld3);
		~TerrainRenderData();

	public:
		float fOriginX;
		float fOriginZ;
		float fSize;
		uint32 nLod;
		FVector3 vTint;
		float aStitchStepWorld[4];

	public:
		void Init(float oriX, 
				  float oriZ, 
				  float size,
				  uint32 lod,
				  const FVector3& tint,
				  float stitchStepWorld[4]);
		void Init(float oriX, 
				  float oriZ, 
				  float size,
				  uint32 lod,
				  const FVector3& tint,
				  float stitchStepWorld0,
				  float stitchStepWorld1,
				  float stitchStepWorld2,
				  float stitchStepWorld3);
	};
	typedef std::vector<TerrainRenderData*> TerrainRenderDataPtrVector;

	/////////////////////////// TerrainRenderInstanceData ////////////////
	class vulkanExport TerrainRenderInstanceData
	{
	public:
		TerrainRenderInstanceData();
		~TerrainRenderInstanceData();

	public:
		float aPatch[4];
		float aLodTint[4];
		float aStitchStep[4];

	public:
		static TerrainRenderInstanceData* MakeInstanceData(const TerrainRenderData* pRenderData);
		static void MakeInstanceData(TerrainRenderInstanceData* pInstance, const TerrainRenderData* pRenderData);

	public:
		void Clear();

		void Init(const TerrainRenderData* pRenderData);

	};
	typedef std::vector<TerrainRenderInstanceData*> TerrainRenderInstanceDataPtrVector;

	/////////////////////////// TerrainRenderBatchData ///////////////////
	class vulkanExport TerrainRenderBatchData
	{
	public:
		TerrainRenderBatchData();
		~TerrainRenderBatchData();

	public:
		uint32 nLod;
		uint32 nFirstInstance;
		uint32 nInstanceCount;

		TerrainRenderInstanceDataPtrVector aInstances;
		
	public:
		void Destroy();

		void Init(uint32 lod);
		void Refresh(uint32 firstInstance, uint32 instanceCount);

		void ClearInstanceDatas();
		void AddInstanceData(const TerrainRenderData* pRenderData);
		void AddInstanceDatas(const TerrainRenderDataPtrVector& aRenderData);

	};
	typedef std::vector<TerrainRenderBatchData> TerrainRenderBatchDataVector;

	/////////////////////////// TerrainRenderBatches /////////////////////
	class vulkanExport TerrainRenderBatches
	{
	public:
		TerrainRenderBatches();
		~TerrainRenderBatches();

	public:
		bool bAddBatches;
		TerrainRenderBatchDataVector aBatches;
		TerrainRenderInstanceDataPtrVector aInstances;

	public:
		F_FORCEINLINE bool IsAddBatches() const { return this->bAddBatches; }
		F_FORCEINLINE void SetIsAddBatches(bool b) { this->bAddBatches = b; }

	public:
		void Destroy();
		void Init();

		void BeginBatches();
			void ClearBatches();
			void AddBatches(const TerrainRenderDataPtrVector& aRenderData);
			void SetupBatches();
		void EndBatches();

		uint32 GetActiveBatchCount() const;
	};

	/////////////////////////// TerrainRenderPatchGeometry ///////////////
	class vulkanExport TerrainRenderPatchGeometry
	{
	public:
		TerrainRenderPatchGeometry();
		~TerrainRenderPatchGeometry();

	public:
		std::vector<FVertex_Pos3Color4Normal3Tex2> aVertices;
		std::vector<uint32> aIndicesTriangle;
		std::vector<uint32> aIndicesWireFrame;
		std::array<IndexRange, 3> aRangesTriangle;
    	std::array<IndexRange, 3> aRangesWireFrame;

	public:
		static TerrainRenderPatchGeometry MakePatchGeometry(int nPatchQuads);
		static void MakePatchGeometry(TerrainRenderPatchGeometry* pPatchGeometry, int nPatchQuads);
		static IndexRange AppendWireframeRange(const std::vector<uint32>& aTriangleIndices, IndexRange triangleRange, std::vector<uint32>& aLineIndices);

	public:
		

	public:
		void Destroy();
		void Init(int nPatchQuads);

	};


	/////////////////////////// TerrainRender ////////////////////////////
	class vulkanExport TerrainRender : public Base
    {
    public:
        TerrainRender(const String& nameRender);
        virtual ~TerrainRender();

	public:
	////RenderPatchGeometry
		static TerrainRenderPatchGeometry* s_pPatchGeometry;

	////RenderBatches
		static TerrainRenderBatches* s_pRenderBatches;

	////Init/Destroy
		static bool InitStatic(int nPatchQuads);
		static void DestroyStatic();

		static void BeginRenderBatches();
		static void EndRenderBatches();

	public:
		TerrainChunked* pChunked;
		TerrainRenderBatches* pRenderBatches;

		bool bAddRenderDatas;
		TerrainRenderDataPtrVector aRenderDatas;


	public:
		F_FORCEINLINE bool IsAddRenderDatas() const { return this->bAddRenderDatas; }
		F_FORCEINLINE void SetIsAddRenderDatas(bool b) { this->bAddRenderDatas = b; }


	public:
		void Destroy();
		bool Init(TerrainChunked* pChunked);

		void BeginAddRenderDatas();
			void ClearRenderDatas();
			void AddRenderData(TerrainRenderData* pRenderData);
			void AddRenderDatas(const TerrainRenderDataPtrVector& aRDs);
		void EndAddRenderDatas();

	protected:
		void destroyRenderBatches();
		bool createRenderBatches();

	};

}; //LostPeterVulkan

#endif