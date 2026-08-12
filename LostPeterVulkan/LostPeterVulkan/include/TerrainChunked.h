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
		void Destroy();
		bool Init(TerrainHeightMap* pHeightMap);	


	public:
		

	};

}; //LostPeterVulkan

#endif