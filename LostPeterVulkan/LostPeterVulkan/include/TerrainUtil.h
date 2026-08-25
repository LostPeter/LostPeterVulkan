/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2026-08-22
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _TERRAIN_UTIL_H_
#define _TERRAIN_UTIL_H_

#include "PreDefine.h"

namespace LostPeterVulkan
{
	class vulkanExport TerrainUtil
    {
	public:
		static const int s_nIDMax; 
		static const String s_strNameAssets;
		static const String s_strNameTerrain;

    public:
		static String GetTerrainSettingPath(const String& nameSetting);
		static String GetTerrainHeightMapPath(const String& nameHeightMap);
		static String GetTerrainHeightMapRelativePath(const String& nameHeightMap);


		static int ToChunkedID(int x, int z);
		static void FromChunkedID(int id, int& x, int& z);


		static void ParseChunkedXZ(float posX, float posZ, int& x, int& z);
		static void ParseChunkedXZ(const FVector2& pos, int& x, int& z);
		static void ParseChunkedXZ(const FVector3& pos, int& x, int& z);
		static void ParseChunkedXZ(const FCamera* pCamera, int& x, int& z);


	};	

}; //LostPeterVulkan

#endif