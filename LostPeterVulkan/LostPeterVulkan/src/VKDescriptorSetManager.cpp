/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-15
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/VKDescriptorSetManager.h"
#include "../include/VulkanWindow.h"
#include "../include/VKDescriptorSetSerializer.h"
#include "../include/VKDescriptorSet.h"

template<> LostPeter::VKDescriptorSetManager* LostPeterFoundation::FSingleton<LostPeter::VKDescriptorSetManager>::ms_Singleton = nullptr;

namespace LostPeter
{
    VKDescriptorSetManager* VKDescriptorSetManager::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	VKDescriptorSetManager& VKDescriptorSetManager::GetSingleton()
	{  
		F_Assert(ms_Singleton && "VKDescriptorSetManager::GetSingleton")
		return (*ms_Singleton);  
	}

    VKDescriptorSetManager::VKDescriptorSetManager()
        : Base("VKDescriptorSetManager")
        , m_pVKDescriptorSetSerializer(nullptr)
    {

    }
    VKDescriptorSetManager::~VKDescriptorSetManager()
    {
        Destroy();
    }
    
    void VKDescriptorSetManager::Destroy()
    {
        F_DELETE(m_pVKDescriptorSetSerializer)
        DeleteVKDescriptorSetAll();
    }
    bool VKDescriptorSetManager::Init(uint nGroup, const String& strNameCfg)
    {
        //1> VKDescriptorSet Cfg Path 
        String strPathCfgShader = FPathManager::GetSingleton().GetFilePath(nGroup, strNameCfg);
        if (strPathCfgShader.empty())
        {
            F_LogError("*********************** VKDescriptorSetManager::Init: Can not get file path from group: [%u], name: [%s] !", nGroup, strNameCfg.c_str());
            return false;
        }

        //2> VKDescriptorSet Serializer
        m_pVKDescriptorSetSerializer = new VKDescriptorSetSerializer();
        if (!m_pVKDescriptorSetSerializer->LoadFile(strPathCfgShader))
        {
            F_LogError("*********************** VKDescriptorSetManager::Init: Load file descriptor set cfg failed, group: [%u], name: [%s] !", nGroup, strNameCfg.c_str());
            return false;
        }

        return true;
    }

    bool VKDescriptorSetManager::LoadVKDescriptorSetAll()
    {
        if (m_pVKDescriptorSetSerializer == nullptr)
            return false;

        DeleteVKDescriptorSetAll();
        DescriptorSetInfoPtrVector& aDescriptorSetInfos = m_pVKDescriptorSetSerializer->GetDescriptorSetInfoPtrVector();
        for (DescriptorSetInfoPtrVector::iterator it = aDescriptorSetInfos.begin();
             it != aDescriptorSetInfos.end(); ++it)
        {
            if (!loadVKDescriptorSet(*it))
                continue;
        }

        return true;
    }

    VKDescriptorSet* VKDescriptorSetManager::LoadVKDescriptorSet(const String& strName)
    {
        if (m_pVKDescriptorSetSerializer == nullptr)
            return nullptr;

        VKDescriptorSet* pVKDescriptorSet = GetVKDescriptorSet(strName);
        if (pVKDescriptorSet == nullptr)
        {
            DescriptorSetInfo* pDescriptorSetInfo = m_pVKDescriptorSetSerializer->GetDescriptorSetInfo(strName);
            if (pDescriptorSetInfo == nullptr)
            {
                F_LogError("*********************** VKDescriptorSetManager::LoadVKDescriptorSet: Can not find descriptor set info, name: [%s] !", strName.c_str());
                return nullptr;
            }
            if (!loadVKDescriptorSet(pDescriptorSetInfo))
            {
                return nullptr;
            }
        }
        return pVKDescriptorSet;
    }
    VKDescriptorSet* VKDescriptorSetManager::loadVKDescriptorSet(DescriptorSetInfo* pDSI)
    {
        Uint32Vector aSTs;
        StringVector aShaderTypes = FUtilString::Split(pDSI->nameShaderTypes, ";");
        size_t count = aShaderTypes.size();
        for (size_t i = 0; i < count; i++)
        {
            uint32 nShaderType = (uint32)F_ParseShaderType(aShaderTypes[i]);
            aSTs.push_back(nShaderType);
        }
        VKDescriptorSet* pVKDescriptorSet = new VKDescriptorSet(pDSI->nameDescriptorSetInfo,
                                                                pDSI->typeDescriptorSet,
                                                                pDSI->typeDescriptorSetUsage,
                                                                aSTs);
        pVKDescriptorSet->LoadDescriptorSet();
        if (AddVKDescriptorSet(pVKDescriptorSet))
        {
            F_LogInfo("VKDescriptorSetManager::loadVKDescriptorSet: Load descriptor set success, [%s]-[%s]-[%s]-[%s] !", 
                      pDSI->nameDescriptorSetInfo.c_str(), 
                      Util_GetDescriptorSetTypeName(pDSI->typeDescriptorSet).c_str(), 
                      Util_GetDescriptorSetUsageTypeName(pDSI->typeDescriptorSetUsage).c_str(),
                      pDSI->nameShaderTypes.c_str());
        }
        return pVKDescriptorSet;
    }

    bool VKDescriptorSetManager::HasVKDescriptorSet(const String& strName)
    {
        return GetVKDescriptorSet(strName) != nullptr;
    }

    VKDescriptorSet* VKDescriptorSetManager::GetVKDescriptorSet(const String& strName)
    {
        VKDescriptorSetPtrMap::iterator itFind = m_mapVKDescriptorSet.find(strName);
        if (itFind == m_mapVKDescriptorSet.end())
        {
            return nullptr;
        }
        return itFind->second;
    }

    bool VKDescriptorSetManager::AddVKDescriptorSet(VKDescriptorSet* pVKDescriptorSet)
    {
        const String& strName = pVKDescriptorSet->GetName();
        VKDescriptorSetPtrMap::iterator itFind = m_mapVKDescriptorSet.find(strName);
        if (itFind != m_mapVKDescriptorSet.end())
        {
            F_LogError("*********************** VKDescriptorSetManager::AddVKDescriptorSet: VKDescriptorSet name already exist: [%s] !", strName.c_str());
            F_DELETE(pVKDescriptorSet)
            return false;
        }

        m_aVKDescriptorSet.push_back(pVKDescriptorSet);
        m_mapVKDescriptorSet.insert(VKDescriptorSetPtrMap::value_type(strName, pVKDescriptorSet));
        return true;
    }

    void VKDescriptorSetManager::DeleteVKDescriptorSet(const String& strName)
    {
        VKDescriptorSetPtrMap::iterator itFind = m_mapVKDescriptorSet.find(strName);
        if (itFind == m_mapVKDescriptorSet.end())
        {
            return;
        }

        VKDescriptorSetPtrVector::iterator itFindA = std::find(m_aVKDescriptorSet.begin(), m_aVKDescriptorSet.end(), itFind->second);
        if (itFindA != m_aVKDescriptorSet.end())
            m_aVKDescriptorSet.erase(itFindA);
        F_DELETE(itFind->second)
        m_mapVKDescriptorSet.erase(itFind);
    }

    void VKDescriptorSetManager::DeleteVKDescriptorSetAll()
    {
        for (VKDescriptorSetPtrMap::iterator it = m_mapVKDescriptorSet.begin();
             it != m_mapVKDescriptorSet.end(); ++it)
        {
            F_DELETE(it->second)
        }
        m_aVKDescriptorSet.clear();
        m_mapVKDescriptorSet.clear();
    }

}; //LostPeter