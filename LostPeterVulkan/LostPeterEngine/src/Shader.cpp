/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-04
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/Shader.h"

namespace LostPeterEngine
{


    const String Shader::ms_nameShader = "Shader";
    Shader::Shader(ResourceManager* pResourceManager,
				   uint32 nGroup, 
				   const String& strName,
			   	   const String& strGroupName,
			       ResourceHandle nHandle,
				   bool bIsManualLoad /*= false*/,
			   	   ResourceManualLoader* pResourceManualLoader /*= nullptr*/)
        : Resource(pResourceManager,
				   nGroup, 
				   strName,
				   strGroupName,
				   nHandle,
				   bIsManualLoad,
				   pResourceManualLoader)
        , m_strPath("")
        , m_eShader(F_Shader_Vertex)
    {
        if (createParameterDictionary(ms_nameShader))
		{
			addParameterBase();
		}
    }
        void Shader::addParameterBase()
        {

        }
        
    Shader::~Shader()
    {

    }   

    void Shader::Destroy()
    {
        
        Resource::Destroy();
    }

    void Shader::loadImpl()
    {

    }

    void Shader::unloadImpl()
    {

    }

    uint32 Shader::calculateSize() const
    {

        return 0;
    }

    void Shader::destroyInternalResources()
    {

    }

    bool Shader::createInternalResources()
    {

        return true;
    }

}; //LostPeterEngine