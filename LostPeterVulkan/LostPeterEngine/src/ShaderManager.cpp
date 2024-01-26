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

#include "../include/ShaderManager.h"
#include "../include/ShaderSerializer.h"
#include "../include/Shader.h"

template<> LostPeterEngine::ShaderManager* LostPeterFoundation::FSingleton<LostPeterEngine::ShaderManager>::ms_Singleton = nullptr;

namespace LostPeterEngine
{
    ShaderManager* ShaderManager::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	ShaderManager& ShaderManager::GetSingleton()
	{  
		F_Assert(ms_Singleton && "ShaderManager::GetSingleton")
		return (*ms_Singleton);     
	}

    const String ShaderManager::ms_strShaderConfigName = "Cfg_Shader.xml";
    ShaderManager::ShaderManager()
        : ResourceManager(E_GetResourceTypeName(E_Resource_Shader), E_Resource_Shader)
        , m_pShaderSerializer(nullptr)
    {

    }
    ShaderManager::~ShaderManager()
    {

    }

    void ShaderManager::Destroy()
    {
        F_DELETE(m_pShaderSerializer)

        ResourceManager::Destroy();
    }
    bool ShaderManager::Init(uint nGroup, const String& strNameCfg)
    {
        //1> Shader Cfg Path 
        String strPathCfgShader = FPathManager::GetSingleton().GetFilePath(nGroup, strNameCfg);
        if (strPathCfgShader.empty())
        {
            F_LogError("*********************** ShaderManager::Init: Can not get file path from group: [%u], name: [%s] !", nGroup, strNameCfg.c_str());
            return false;
        }

        //2> Shader Serializer
        m_pShaderSerializer = new ShaderSerializer();
        if (!m_pShaderSerializer->LoadFile(strPathCfgShader))
        {
            F_LogError("*********************** ShaderManager::Init: Load file shader cfg failed, group: [%u], name: [%s] !", nGroup, strNameCfg.c_str());
            return false;
        }

        return true;
    }

    Shader* ShaderManager::LoadShader(uint nGroup, const String& strName, const String& strGroupName /*= ResourceGroupManager::ms_strNameResourceGroup_AutoDetect*/)
    {
        if (m_pShaderSerializer == nullptr)
            return nullptr;

        Shader* pShader = GetShader(strName, strGroupName);
        if (pShader == nullptr)
        {
            ShaderInfo* pShaderInfo = m_pShaderSerializer->GetShaderInfo(nGroup, strName);
            if (pShaderInfo == nullptr)
            {
                F_LogError("*********************** ShaderManager::LoadShader: Can not find shader info, group: [%u], name: [%s] !", nGroup, strName.c_str());
                return nullptr;
            }
            if (!loadShader(pShaderInfo))
            {
                return nullptr;
            }
        }
        pShader->AddRef();
        return pShader;
    }
    Shader* ShaderManager::loadShader(ShaderInfo* pSI)
    {
        
        return nullptr;
    }

    bool ShaderManager::HasShader(const String& strName)
    {
        return GetResourceByName(strName) != nullptr;
    }
    bool ShaderManager::HasShader(const String& strName, const String& strGroupName)
    {
        return GetResourceByName(strName, strGroupName) != nullptr;
    }
    Shader* ShaderManager::GetShader(const String& strName)
    {
        Resource* pResource = GetResourceByName(strName);
        if (pResource == nullptr)
            return nullptr;
        return (Shader*)pResource;
    }
    Shader* ShaderManager::GetShader(const String& strName, const String& strGroupName)
    {
        Resource* pResource = GetResourceByName(strName, strGroupName);
        if (pResource == nullptr)
            return nullptr;
        return (Shader*)pResource;
    }

    

    

}; //LostPeterEngine