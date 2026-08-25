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

#ifndef _TERRAIN_CHUNKED_H_
#define _TERRAIN_CHUNKED_H_

#include "Base.h"
#include "TerrainRender.h"

namespace LostPeterVulkan
{
	/////////////////////////// TerrainChunkedNode ///////////////////////
	class vulkanExport TerrainChunkedNode
	{
	public:
		TerrainChunkedNode();

	public:
		int nID;
		int nLevel;
		int nX;
		int nZ;
		int nSize;
		float fHeightMin;
		float fHeightMax;
		float fGeometricError;
		FAABB bounds;

		TerrainChunkedNode* ppChildren[4];

	public:
		static float DistanceToAABB(const FVector3& point, const FAABB& box);
		static float DistanceToAABBXZ(const FVector3& point, const FAABB& box);

	public:	
		bool HasChildren() const;
		void ClearChildren();

		float DistanceToAABB(const FVector3& point);
		float DistanceToAABBXZ(const FVector3& point);

	public:
		void Init(int id,
				  int level,
				  int x,
				  int z,
				  int size);

	};


	/////////////////////////// TerrainChunked ///////////////////////////
	class vulkanExport TerrainChunked : public Base
    {
    public:
        TerrainChunked(const String& nameChunked);
        virtual ~TerrainChunked();

	public:	
		int nChunkedX;
		int nChunkedZ;
		int nChunkedID;

		//Node
		int nLeafQuads;
		int nPatchQuads;
		TerrainChunkedNode* pRootNode;
		int nMaxDepth;
		TerrainChunkedNodePtrVector aNodes;

		//HeightMap
		TerrainHeightMap* pHeightMap;
		VKTexture* pTexture_HeightMap;
		VKTexture* pTexture_NormalMap;

		//TextureDiffuse/Normal/Control


		bool bIsInit;

	public:
		static bool IntervalsOverlap(int a0, int a1, int b0, int b1);

	public:
		F_FORCEINLINE int GetChunkedX() const { return this->nChunkedX; }
		F_FORCEINLINE int GetChunkedZ() const { return this->nChunkedZ; }
		F_FORCEINLINE void GetChunkedXZ(int& x, int& z) const { x = this->nChunkedX; z = this->nChunkedZ; }
		F_FORCEINLINE int GetChunkedID() const { return this->nChunkedID; }

		F_FORCEINLINE int GetLeafQuads() const { return this->nLeafQuads; }
		F_FORCEINLINE int GetPatchQuads() const { return this->nPatchQuads; }
		F_FORCEINLINE TerrainChunkedNode* GetRootNode() const { return this->pRootNode; }
		F_FORCEINLINE int GetMaxDepth() const { return this->nMaxDepth; }
		F_FORCEINLINE const TerrainChunkedNodePtrVector& GetNodes() const { return this->aNodes; }

		F_FORCEINLINE TerrainHeightMap* GetHeightMap() const { return this->pHeightMap; }

		F_FORCEINLINE bool IsInit() const { return this->bIsInit; }
		F_FORCEINLINE void SetIsInit(bool b) { this->bIsInit = b; }

	public:
		void Destroy();
		bool Init(TerrainChunkedSetting* pChunkedSetting, 
				  TerrainHeightMap* pHeightMap,
				  int leafQuads, int patchQuads);

		void SelectDynamicLod(const FVector3& vPos, float fRadiusLod0, float fRadiusLod1, TerrainChunkedNodePtrVector& aNodeSelect);
		void BuildRenderData(const TerrainChunkedNodePtrVector& aNodeSelect, TerrainRenderDataVector& aRenderData);

		int GetEffectiveSegmentStepCells(const TerrainChunkedNode* pNode, int lod) const;

	protected:
		TerrainChunkedNode* buildNode(int x, int z, int size, int level);

		void computeBoundsAndError(TerrainChunkedNode* pNode);
		float computeGeometricError(TerrainChunkedNode* pNode);

		void selectDynamicRecursive(TerrainChunkedNode* pNode, const FVector3& vCenter, float fRadiusLod0, float fRadiusLod1, TerrainChunkedNodePtrVector& aNodeSelect);

		std::array<float, 4> stitchStepsForNode(const TerrainChunkedNode* pNode, int lod, const TerrainChunkedNodePtrVector& aNodeSelect) const;
	
	protected:
		void destroyTextures();
			
		bool createTextures(TerrainChunkedSetting* pChunkedSetting);

	};

}; //LostPeterVulkan

#endif