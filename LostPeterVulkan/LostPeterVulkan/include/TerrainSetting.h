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

#ifndef _TERRAIN_SETTING_H_
#define _TERRAIN_SETTING_H_

#include "Base.h"

namespace LostPeterVulkan
{
	class vulkanExport TerrainSetting : public FSingleton<TerrainSetting>
                                   	  , public Base
    {
    public:
        TerrainSetting();
        virtual ~TerrainSetting();

	public:
		int nLeafQuads;
		int nPatchQuads;

		float fLodPixelError;
		bool bHeadless;
		uint32 nAutoCloseMs;

	public:
        static TerrainSetting& GetSingleton();
		static TerrainSetting* GetSingletonPtr();

	public:
		F_FORCEINLINE int GetLeafQuads() const { return this->nLeafQuads; }
		F_FORCEINLINE int GetPatchQuads() const { return this->nPatchQuads; }

		
	};

}; //LostPeterVulkan

#endif