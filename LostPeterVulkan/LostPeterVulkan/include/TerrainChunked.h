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

namespace LostPeterVulkan
{
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
		FAABB aabb;

		TerrainChunkedNode* ppChildren[4];

	public:	
		bool HasChildren() const;
		void ClearChildren();

	public:
		void Init(int id,
				  int level,
				  int x,
				  int z,
				  int size);

	};


	class vulkanExport TerrainChunked : public Base
    {
    public:
        TerrainChunked(const String& nameChunked);
        virtual ~TerrainChunked();

	public:	
		TerrainHeightMap* pHeightMap;
		int nLeafQuads;
		int nPatchQuads;
		TerrainChunkedNode* pRootNode;
		int nMaxDepth;
		TerrainChunkedNodePtrVector aNodes;

	public:
		F_FORCEINLINE TerrainHeightMap* GetHeightMap() const { return this->pHeightMap; }
		F_FORCEINLINE int GetLeafQuads() const { return this->nLeafQuads; }
		F_FORCEINLINE int GetPatchQuads() const { return this->nPatchQuads; }
		F_FORCEINLINE TerrainChunkedNode* GetRootNode() const { return this->pRootNode; }
		F_FORCEINLINE int GetMaxDepth() const { return this->nMaxDepth; }
		const TerrainChunkedNodePtrVector& GetNodes() const { return this->aNodes; }

	public:
		void Destroy();
		bool Init(TerrainHeightMap* pHeightMap,
				  int leafQuads, 
				  int patchQuads);


	public:
		

	protected:
		TerrainChunkedNode* createNode(int x, int z, int size, int level);

		void computeBoundsAndError(TerrainChunkedNode* pNode);
		float computeGeometricError(TerrainChunkedNode* pNode);
	};

}; //LostPeterVulkan

#endif