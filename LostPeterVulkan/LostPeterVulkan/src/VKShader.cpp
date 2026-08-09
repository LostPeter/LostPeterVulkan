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

#include "../include/VKShader.h"
#include "../include/VulkanWindow.h"

namespace LostPeterVulkan
{
	VKShader::VKShader(const String& nameShader)
		: Base(nameShader)

		, typeShader(F_Shader_Vertex)
		, pathFile("")
		, nameMain("main")

		, vkShaderModule(VK_NULL_HANDLE)

	{
		
	}
    VKShader::~VKShader()
	{
		Destroy();
	}

	void VKShader::Destroy()
	{
		if (this->vkShaderModule != VK_NULL_HANDLE)
		{
			Base::GetWindowPtr()->destroyVkShaderModule(this->vkShaderModule);
		}
		this->vkShaderModule = VK_NULL_HANDLE;
	}

	bool VKShader::Init(FShaderType type,
						const String& path,
						const String& nameMain)
	{
		this->typeShader = type;
		this->pathFile = path;
		this->nameMain = nameMain;

		const String& nameShader = this->name;
		this->vkShaderModule = Base::GetWindowPtr()->createVkShaderModule(nameShader,
																		  type,
																		  path);
		if (this->vkShaderModule == VK_NULL_HANDLE)
		{
			F_LogError("*********************** VKShader::Init failed, name: [%s], path: [%s] !", nameShader.c_str(), path.c_str());
			return false;
		}
		
		return true;
	}

}; //LostPeterVulkan