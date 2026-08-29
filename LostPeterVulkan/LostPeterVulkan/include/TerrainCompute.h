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
		static const String s_nameDescriptorSetLayout;
		static const String s_nameShader;

		static DescriptorSetLayout* s_pDescriptorSetLayout;
		static VKShader* s_pShader;

		static void InitStatic();
		static void DestroyStatic();

	public:
		VKStatePipelineCompute* poStatePipelineCompute;
        
        TextureCopyConstants* pTextureCopy;
        VKBufferUniform* poBuffer_TextureCopy;  

        bool isNormalUpdated;
        bool isNormalUpdated_Sustained;

    public:
		TerrainChunked* pChunked;

	public:
		void Destroy();
		bool Init(TerrainChunked* pChunked);

	public:
		virtual void CleanupSwapChain();
        virtual void UpdateDescriptorSet();

	protected:
		void destroyBufferTextureCopy();
        
        bool createBufferTextureCopy(int nHeightMapSize);

	};	

}; //LostPeterVulkan

#endif