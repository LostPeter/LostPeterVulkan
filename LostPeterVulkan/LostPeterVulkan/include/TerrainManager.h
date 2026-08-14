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

#ifndef _TERRAIN_MANAGER_H_
#define _TERRAIN_MANAGER_H_

#include "Base.h"
#include "ObjectPool.h"

namespace LostPeterVulkan
{
	class vulkanExport TerrainManager : public FSingleton<TerrainManager>
                                   	  , public Base
    {
    public:
        TerrainManager();
        virtual ~TerrainManager();

	public:
		static int s_nNodeCount_Init;
        static int s_nNodeCount_Max;
        static int s_nNodeCount_Step;
		
		ObjectPointerPool<TerrainChunkedNode>* pNodePool;

	public:
        static TerrainManager& GetSingleton();
		static TerrainManager* GetSingletonPtr();
		
	public:

	public:
        void Destroy();
        bool Init();

	protected:
		void destroyPools();

        void createPools();

	};

}; //LostPeterVulkan

#endif