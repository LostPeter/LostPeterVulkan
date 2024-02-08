/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-02-03
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/ShaderProgram.h"
#include "../include/ShaderProgramManager.h"

namespace LostPeterEngine
{
    const String ShaderProgram::ms_nameProgramShader = "ShaderProgram";
    ShaderProgram::ShaderProgram(ResourceManager* pResourceManager,
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
        , m_bInternalResourcesCreated(false)
    {
        if (createParameterDictionary(ms_nameProgramShader))
		{
			addParameterBase();
		}
    }
        void ShaderProgram::addParameterBase()
        {

        }
        
    ShaderProgram::~ShaderProgram()
    {

    }   

    void ShaderProgram::Destroy()
    {
        
        Resource::Destroy();
    }

    void ShaderProgram::loadImpl()
    {
        createInternalResources();
    }

    void ShaderProgram::unloadImpl()
    {
        destroyInternalResources();
    }

    uint32 ShaderProgram::calculateSize() const
    {

        return 0;
    }

    void ShaderProgram::destroyInternalResources()
    {
        if (m_bInternalResourcesCreated)
		{
			destroyInternalResourcesImpl();
			m_bInternalResourcesCreated = false;

			if (m_eResourceLoading.load() != E_ResourceLoading_Unloading)
            {
                m_eResourceLoading.store(E_ResourceLoading_Unloaded);
                _FireUnloadingComplete();
            }
		}
    }

    bool ShaderProgram::createInternalResources()
    {
        if (!m_bInternalResourcesCreated)
		{
			createInternalResourcesImpl();
			m_bInternalResourcesCreated = true;

			if (!IsLoading())
            {
                m_eResourceLoading.store(E_ResourceLoading_Loaded);
                _FireLoadingComplete(false);
            }
		}
		return true;
    }

}; //LostPeterEngine