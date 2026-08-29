/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2026-08-28
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _TERRAIN_COMPUTE_H_
#define _TERRAIN_COMPUTE_H_

#include "Base.h"

namespace LostPeterVulkan
{
	class vulkanExport TerrainCompute : public Base
    {
	public:
		TerrainCompute(const String& nameCompute);
        virtual ~TerrainCompute();

	public:
		

    public:
		TerrainChunked* pChunked;

	public:
		void Destroy();
		bool Init(TerrainChunked* pChunked);

	public:
		

	};	

}; //LostPeterVulkan

#endif