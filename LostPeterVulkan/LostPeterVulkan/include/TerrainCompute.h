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
		static const String s_nameShaderCompute;
		static DescriptorSetLayout* s_pDescriptorSetLayout;
		static VKShader* s_pShaderCompute;

		static void InitStatic();
		static void DestroyStatic();

		static void ComputeBatches(VkCommandBuffer& commandBuffer);

	public:
		TerrainChunked* pChunked;

		VKStatePipelineCompute* poStatePipelineCompute;
        
        TextureCopyConstants* pTextureCopy;
        VKBufferUniform* poBuffer_TextureCopy;  

        bool isNormalUpdated;
        bool isNormalUpdated_Sustained;
		
	public:
		void Destroy();
		bool Init(TerrainChunked* pChunked);

		void Compute(VkCommandBuffer& commandBuffer);

	public:
		void CleanupSwapChain();
        void UpdateDescriptorSet();

	protected:
		void destroyBufferTextureCopy();
        
        bool createBufferTextureCopy(int nHeightMapSize);

	};	

}; //LostPeterVulkan

#endif