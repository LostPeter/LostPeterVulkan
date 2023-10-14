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

#include "../include/ShaderManager.h"
#include "../include/VulkanWindow.h"
#include "../include/ShaderSerializer.h"
#include "../include/Shader.h"

template<> LostPeter::ShaderManager* LostPeterFoundation::FSingleton<LostPeter::ShaderManager>::ms_Singleton = nullptr;

namespace LostPeter
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

    ShaderManager::ShaderManager()
        : Base("ShaderManager")
        , m_pShaderSerializer(nullptr)
    {

    }
    ShaderManager::~ShaderManager()
    {
        Destroy();
    }

    void ShaderManager::Destroy()
    {
        F_DELETE(m_pShaderSerializer)
        DeleteShaderAll();
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

    bool ShaderManager::LoadShaderAll()
    {
        if (m_pShaderSerializer == nullptr)
            return false;

        DeleteShaderAll();
        ShaderInfoPtrVector& aShaderInfos = m_pShaderSerializer->GetShaderInfos();
        for (ShaderInfoPtrVector::iterator it = aShaderInfos.begin();
             it != aShaderInfos.end(); ++it)
        {
            if (!loadShader(*it))
                continue;
        }

        return true;
    }

    Shader* ShaderManager::LoadShader(uint nGroup, const String& strName)
    {
        if (m_pShaderSerializer == nullptr)
            return nullptr;

        Shader* pShader = GetShader(nGroup, strName);
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
        return pShader;
    }
    Shader* ShaderManager::loadShader(ShaderInfo* pSI)
    {
        Shader* pShader = new Shader(pSI->group,
                                     pSI->nameShader,
                                     pSI->pathShader,
                                     pSI->typeShader);
        pShader->LoadShader();
        if (AddShader(pSI->group, pShader))
        {
            F_LogInfo("ShaderManager::loadShader: Load shader success, [%u]-[%s]-[%s] !", 
                      pSI->group, 
                      pSI->nameShader.c_str(), 
                      pSI->pathShader.c_str());
        }
        return pShader;
    }

    bool ShaderManager::HasShader(uint nGroup, const String& strName)
    {
        return GetShader(nGroup, strName) != nullptr;
    }

    Shader* ShaderManager::GetShader(uint nGroup, const String& strName)
    {
        ShaderGroupPtrMap::iterator itFindGroup = m_mapShaderGroup.find(nGroup);
        if (itFindGroup == m_mapShaderGroup.end())
        {
            F_LogError("*********************** ShaderManager::GetShader: Can not find group: [%u], name: [%s]", nGroup, strName.c_str());
            return nullptr;
        }

        ShaderPtrMap::iterator itFindShader = itFindGroup->second.find(strName);
        if (itFindShader == itFindGroup->second.end())
        {
            F_LogError("*********************** ShaderManager::GetShader: Can not find shader from group: [%u], name: [%s]", nGroup, strName.c_str());
            return nullptr;
        }
        return itFindShader->second;
    }

    bool ShaderManager::AddShader(uint nGroup, Shader* pShader)
    {
        ShaderGroupPtrMap::iterator itFind = m_mapShaderGroup.find(nGroup);
        if (itFind == m_mapShaderGroup.end())
        {
            ShaderPtrMap mapShader;
            m_mapShaderGroup[nGroup] = mapShader;
            itFind = m_mapShaderGroup.find(nGroup);
        }
        const String& strName = pShader->GetName();
        ShaderPtrMap::iterator itFindShader = itFind->second.find(strName);
        if (itFindShader != itFind->second.end())
        {
            F_LogError("*********************** ShaderManager::AddShader: Shader name already exist: [%s] !", strName.c_str());
            F_DELETE(pShader)
            return false;
        }

        itFind->second.insert(ShaderPtrMap::value_type(strName, pShader));
        m_aShader.push_back(pShader);
        m_mapVkShaderModules.insert(VkShaderModuleMap::value_type(strName, pShader->vkShaderModule));
        return true;
    }

    void ShaderManager::DeleteShader(uint nGroup, const String& strName)
    {
        ShaderGroupPtrMap::iterator itFind = m_mapShaderGroup.find(nGroup);
        if (itFind == m_mapShaderGroup.end())
        {
            return;
        }

        ShaderPtrMap::iterator itFindShader = itFind->second.find(strName);
        if (itFindShader != itFind->second.end())
        {
            ShaderPtrVector::iterator itFindA = std::find(m_aShader.begin(), m_aShader.end(), itFindShader->second);
            if (itFindA != m_aShader.end())
                m_aShader.erase(itFindA);
            F_DELETE(itFindShader->second)
            itFind->second.erase(itFindShader);
        }
        VkShaderModuleMap::iterator itFindSM = m_mapVkShaderModules.find(strName);
        if (itFindSM != m_mapVkShaderModules.end())
        {
            m_mapVkShaderModules.erase(itFindSM);
        }
    }

    void ShaderManager::DeleteShaderAll()
    {
        for (ShaderGroupPtrMap::iterator it = m_mapShaderGroup.begin();
             it != m_mapShaderGroup.end(); ++it)
        {
            ShaderPtrMap& mapShader = it->second;
            for (ShaderPtrMap::iterator itShader = mapShader.begin(); 
                 itShader != mapShader.end(); ++itShader)
            {
                F_DELETE(itShader->second)
            }
        }
        m_aShader.clear();
        m_mapShaderGroup.clear();
        m_mapVkShaderModules.clear();
    }

}; //LostPeter