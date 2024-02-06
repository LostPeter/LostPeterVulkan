/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-02-06
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/ShaderProgramManager.h"
#include "../include/ShaderProgramSerializer.h"
#include "../include/ShaderProgram.h"

template<> LostPeterEngine::ShaderProgramManager* LostPeterFoundation::FSingleton<LostPeterEngine::ShaderProgramManager>::ms_Singleton = nullptr;

namespace LostPeterEngine
{
    ShaderProgramManager* ShaderProgramManager::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	ShaderProgramManager& ShaderProgramManager::GetSingleton()
	{  
		F_Assert(ms_Singleton && "ShaderProgramManager::GetSingleton")
		return (*ms_Singleton);     
	}

    const String ShaderProgramManager::ms_strShaderProgramConfigName = "Cfg_ShaderProgram.xml";
    ShaderProgramManager::ShaderProgramManager()
        : ResourceManager(E_GetResourceTypeName(E_Resource_Shader), E_Resource_Shader)
        , m_pShaderProgramSerializer(nullptr)
    {

    }
    ShaderProgramManager::~ShaderProgramManager()
    {

    }

    void ShaderProgramManager::Destroy()
    {
        F_DELETE(m_pShaderProgramSerializer)

        ResourceManager::Destroy();
    }
    bool ShaderProgramManager::Init(uint nGroup, const String& strNameCfg)
    {
        //1> ShaderProgram Cfg Path 
        String strPathCfgShaderProgram = FPathManager::GetSingleton().GetFilePath(nGroup, strNameCfg);
        if (strPathCfgShaderProgram.empty())
        {
            F_LogError("*********************** ShaderProgramManager::Init: Can not get file path from group: [%u], name: [%s] !", nGroup, strNameCfg.c_str());
            return false;
        }

        //2> ShaderProgram Serializer
        m_pShaderProgramSerializer = new ShaderProgramSerializer();
        if (!m_pShaderProgramSerializer->LoadFile(strPathCfgShaderProgram))
        {
            F_LogError("*********************** ShaderProgramManager::Init: Load file shader program cfg failed, group: [%u], name: [%s] !", nGroup, strNameCfg.c_str());
            return false;
        }

        return true;
    }

    ShaderProgram* ShaderProgramManager::LoadShaderProgram(uint nGroup, const String& strName, const String& strGroupName /*= ResourceGroupManager::ms_strNameResourceGroup_AutoDetect*/)
    {
        if (m_pShaderProgramSerializer == nullptr)
            return nullptr;

        ShaderProgram* pShaderProgram = GetShaderProgram(strName, strGroupName);
        if (pShaderProgram == nullptr)
        {
            ShaderProgramInfo* pShaderProgramInfo = m_pShaderProgramSerializer->GetShaderProgramInfo(nGroup, strName);
            if (pShaderProgramInfo == nullptr)
            {
                F_LogError("*********************** ShaderProgramManager::LoadShaderProgram: Can not find shader program info, group: [%u], name: [%s] !", nGroup, strName.c_str());
                return nullptr;
            }
            if (!loadShaderProgram(pShaderProgramInfo))
            {
                return nullptr;
            }
        }
        pShaderProgram->AddRef();
        return pShaderProgram;
    }
    ShaderProgram* ShaderProgramManager::loadShaderProgram(ShaderProgramInfo* pSPI)
    {
        
        return nullptr;
    }

    bool ShaderProgramManager::HasShaderProgram(const String& strName)
    {
        return GetResourceByName(strName) != nullptr;
    }
    bool ShaderProgramManager::HasShaderProgram(const String& strName, const String& strGroupName)
    {
        return GetResourceByName(strName, strGroupName) != nullptr;
    }
    ShaderProgram* ShaderProgramManager::GetShaderProgram(const String& strName)
    {
        Resource* pResource = GetResourceByName(strName);
        if (pResource == nullptr)
            return nullptr;
        return (ShaderProgram*)pResource;
    }
    ShaderProgram* ShaderProgramManager::GetShaderProgram(const String& strName, const String& strGroupName)
    {
        Resource* pResource = GetResourceByName(strName, strGroupName);
        if (pResource == nullptr)
            return nullptr;
        return (ShaderProgram*)pResource;
    }

    
}; //LostPeterEngine