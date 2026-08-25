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

#ifndef _TERRAIN_CHUNKED_LOD_H_
#define _TERRAIN_CHUNKED_LOD_H_

#include "Base.h"

namespace LostPeterVulkan
{
	class vulkanExport TerrainChunkedLod : public Base
    {
    public:
        TerrainChunkedLod(const String& nameChunkedLod);
        virtual ~TerrainChunkedLod();

	public:
		TerrainChunked* pTerrainChunked;
		
	public:
		F_FORCEINLINE TerrainChunked* GetTerrainChunked() const { return this->pTerrainChunked; }

	public:
		void Destroy();
		bool Init(TerrainChunkedSetting* pChunkedSetting, 
				  TerrainHeightMap* pHeightMap,
				  int leafQuads, int patchQuads);

	public:
		

	};

}; //LostPeterVulkan

#endif