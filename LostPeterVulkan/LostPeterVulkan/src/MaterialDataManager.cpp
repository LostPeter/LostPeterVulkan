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
#include "../include/Mesh.h"
#include "../include/Object.h"

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
        m_pMaterialDataSerializer = new MaterialDataSerializer(this);
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

	MaterialData* MaterialDataManager::CreateMaterialData(uint32 nGroup, const String& strName, bool bIsFromFile /*= false*/)
	{
		MaterialData* pMaterialData = findMaterialData(strName);
		if (pMaterialData)
		{
			pMaterialData->AddRef();
			return pMaterialData;
		}
		
		pMaterialData = new MaterialData(strName);
		if (bIsFromFile)
		{
			if (!Parser(nGroup, strName, pMaterialData))
			{
				delete pMaterialData;
				return nullptr;
			}
		}
		else
		{
			Material* pMaterialDefault = MaterialManager::GetSingleton().GetMaterial_Default();
			pMaterialData->SerializerFrom(pMaterialDefault->GetMaterialData());
			pMaterialData->SetPath(pMaterialDefault->GetMaterialData()->GetPath());
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
    
	bool MaterialDataManager::Parser(uint32 nGroup, const String& strName, MaterialData* pMaterialData)
	{
		return m_pMaterialDataSerializer->Parser(nGroup, strName, pMaterialData, nullptr);
	}
	bool MaterialDataManager::Parser(uint32 nGroup, const String& strName, MaterialDataPtrVector* pRet /*= nullptr*/)
	{
		return m_pMaterialDataSerializer->Parser(nGroup, strName, nullptr, pRet);
	}

	bool MaterialDataManager::ParserXML(uint32 nGroup, const String& strName, MaterialDataPtrVector* pRet /*= nullptr*/)
	{
		return m_pMaterialDataSerializer->ParserXML(nGroup, strName, pRet);
	}

	bool MaterialDataManager::ParserXML(const char* szFilePath, MaterialDataPtrVector* pRet /*= nullptr*/)
	{
		return m_pMaterialDataSerializer->ParserXML(szFilePath, nullptr, pRet);
	}

	bool MaterialDataManager::ParserBinary(uint32 nGroup, const String& strName, MaterialDataPtrVector* pRet /*= nullptr*/)
	{
		return m_pMaterialDataSerializer->ParserBinary(nGroup, strName, pRet);
	}

	bool MaterialDataManager::ParserBinary(const char* szFilePath, MaterialDataPtrVector* pRet /*= nullptr*/)
	{
		return m_pMaterialDataSerializer->ParserBinary(szFilePath, nullptr, pRet);
	}

	bool MaterialDataManager::SaveXML(Material* pMaterial)
	{
		return m_pMaterialDataSerializer->SaveXML(pMaterial);
	}

	bool MaterialDataManager::SaveXML(uint32 nGroup, Material* pMaterial)
	{
		return m_pMaterialDataSerializer->SaveXML(nGroup, pMaterial);
	}

	bool MaterialDataManager::SaveXML(Mesh* pMesh)
	{
		return m_pMaterialDataSerializer->SaveXML(pMesh);
	}
	
	bool MaterialDataManager::SaveXML(Object* pObject, const String& strPath)
	{
		return m_pMaterialDataSerializer->SaveXML(pObject, strPath);
	}	

	bool MaterialDataManager::SaveXML(const char* szFilePath, MaterialPtrVector& aMA)
	{
		return m_pMaterialDataSerializer->SaveXML(szFilePath, aMA);
	}

	bool MaterialDataManager::SaveBinary(Material* pMaterial)
	{
		return m_pMaterialDataSerializer->SaveBinary(pMaterial);
	}

	bool MaterialDataManager::SaveBinary(uint32 nGroup, Material* pMaterial)
	{
		return m_pMaterialDataSerializer->SaveBinary(nGroup, pMaterial);
	}

	bool MaterialDataManager::SaveBinary(Mesh* pMesh)
	{
		return m_pMaterialDataSerializer->SaveBinary(pMesh);
	}

	bool MaterialDataManager::SaveBinary(Object* pObject, const String& strPath)
	{
		return m_pMaterialDataSerializer->SaveBinary(pObject, strPath);
	}

	bool MaterialDataManager::SaveBinary(const char* szFilePath, MaterialPtrVector& aMA)
	{
		return m_pMaterialDataSerializer->SaveBinary(szFilePath, aMA);
	}

}; //LostPeter