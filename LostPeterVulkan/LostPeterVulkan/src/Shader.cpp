/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-04
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/Shader.h"
#include "../include/VulkanWindow.h"

namespace LostPeter
{
    Shader::Shader(uint32 _group,
                   const String& _nameShader,
                   const String& _pathShader,
                   FShaderType _typeShader)
        : Base(_group, _nameShader)
        , pathShader(_pathShader)
        , typeShader(_typeShader)
        , vkShaderModule(VK_NULL_HANDLE)
    {

    }
    Shader::~Shader()
    {

    }   

    void Shader::Destroy()
    {
        if (this->vkShaderModule != VK_NULL_HANDLE)
        {
            Base::GetWindowPtr()->destroyVkShaderModule(this->vkShaderModule);
        }
        this->vkShaderModule = VK_NULL_HANDLE;
    }

    bool Shader::LoadShader()
    {
        if (this->pathShader.empty())
        {
            F_LogError("*********************** Shader::LoadShader: Shader path is empty !");
                
            return false;
        }

        Destroy();
        this->vkShaderModule = Base::GetWindowPtr()->createVkShaderModule(this->typeShader, this->pathShader);
        
        return true;
    }

}; //LostPeter