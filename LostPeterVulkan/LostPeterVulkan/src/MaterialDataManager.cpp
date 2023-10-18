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

#include "../include/MaterialDataManager.h"
#include "../include/VulkanWindow.h"
#include "../include/MaterialDataSerializer.h"
#include "../include/MaterialData.h"
#include "../include/MaterialManager.h"
#include "../include/Material.h"

template<> LostPeter::MaterialDataManager* LostPeterFoundation::FSingleton<LostPeter::MaterialDataManager>::ms_Singleton = nullptr;

namespace LostPeter
{
	MaterialDataManager* MaterialDataManager::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	MaterialDataManager& MaterialDataManager::GetSingleton()
	{  
		F_Assert(ms_Singleton && "MaterialDataManager::GetSingleton")
		return (*ms_Singleton);     
	}

    MaterialDataManager::MaterialDataManager()
        : Base("MaterialDataManager")
    {
        m_pMaterialDataSerializer = new MaterialDataSerializer();
    }
    MaterialDataManager::~MaterialDataManager()
    {
		F_DELETE(m_pMaterialDataSerializer)
		Destroy();
    }

	void MaterialDataManager::Destroy()
	{
		DestroyAllMaterialData();
	}

	MaterialData* MaterialDataManager::GetMaterialData(const String& strName)
	{
		return findMaterialData(strName);
	}

	MaterialData* MaterialDataManager::CreateMaterialData(uint32 nGroup, const String& strName, bool bNew /*= false*/)
	{
		MaterialData* pMaterialData = findMaterialData(strName);
		if (pMaterialData)
		{
			pMaterialData->AddRef();
			return pMaterialData;
		}
		
		pMaterialData = new MaterialData(strName);
		if (bNew)
		{
			Material* pMaterialDefault = MaterialManager::GetSingleton().GetMaterial_Default();
			pMaterialData->SerializerFrom(pMaterialDefault->GetRenderState());
		}
		else
		{

		}
		pMaterialData->AddRef();

		m_mapMaterialData[strName] = pMaterialData;
		return pMaterialData;
	}

	void MaterialDataManager::DestroyMaterialData(const String& strName)
	{
		MaterialDataPtrMap::iterator itFind = m_mapMaterialData.find(strName);
		if (itFind == m_mapMaterialData.end())
			return;

		MaterialData* pMaterialData = itFind->second;
		pMaterialData->DelRef();
		if (!pMaterialData->HasRef())
		{
			delete pMaterialData;
			m_mapMaterialData.erase(itFind);
		}
	}

	void MaterialDataManager::DestroyMaterialData(MaterialData* pMaterialData)
	{
		DestroyMaterialData(pMaterialData->GetName());
	}

	void MaterialDataManager::DestroyAllMaterialData()
	{
		for (MaterialDataPtrMap::iterator it = m_mapMaterialData.begin();
			 it != m_mapMaterialData.end(); ++it)
		{
			delete it->second;
		}
		m_mapMaterialData.clear();
	}

	MaterialData* MaterialDataManager::findMaterialData(const String& strName)
	{
		MaterialDataPtrMap::iterator itFind = m_mapMaterialData.find(strName);
		if (itFind == m_mapMaterialData.end())
			return 0;
		return itFind->second;
	}

	bool MaterialDataManager::addMaterialData(MaterialData* pMaterialData)
	{
		const String& strName = pMaterialData->GetName();
		if (findMaterialData(strName))
			return false;

		m_mapMaterialData[strName] = pMaterialData;
		return true;
	}
    
}; //LostPeter