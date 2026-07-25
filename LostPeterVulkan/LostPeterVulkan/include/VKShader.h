/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2026-07-25
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VK_SHADER_H_
#define _VK_SHADER_H_

#include "Base.h"

namespace LostPeterVulkan
{
	class vulkanExport VKShader : public Base
    {
    public:
        VKShader(const String& nameShader);
        virtual ~VKShader();

	public:
		FShaderType typeShader;
        String pathFile;
		String nameMain;

		VkShaderModule vkShaderModule;
		
	public:
		void Destroy();
        bool Init(FShaderType type,
				  const String& path,
				  const String& nameMain);

	public:
		F_FORCEINLINE const VkShaderModule& GetVkShaderModule() const { return this->vkShaderModule; }	

	public:

		
	};

}; //LostPeterVulkan

#endif