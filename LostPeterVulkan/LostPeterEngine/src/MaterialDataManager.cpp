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

#include "../include/MaterialDataManager.h"
#include "../include/MaterialDataSerializer.h"
#include "../include/MaterialData.h"
#include "../include/MaterialManager.h"
#include "../include/Material.h"
#include "../include/Mesh.h"
#include "../include/Object.h"

template<> LostPeterEngine::MaterialDataManager* LostPeterFoundation::FSingleton<LostPeterEngine::MaterialDataManager>::ms_Singleton = nullptr;

namespace LostPeterEngine
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
        : ResourceManager(E_GetResourceTypeName(E_Resource_Material), E_Resource_Material)
    {
        m_pMaterialDataSerializer = new MaterialDataSerializer(this);
    }

    MaterialDataManager::~MaterialDataManager()
    {
		Destroy();
    }

	void MaterialDataManager::Destroy()
	{
		F_DELETE(m_pMaterialDataSerializer)
		ResourceManager::Destroy();
	}

	MaterialData* MaterialDataManager::NewMaterialData(uint32 nGroup, const String& strName, const String& strGroupName /*= ResourceGroupManager::ms_strNameResourceGroup_AutoDetect*/)
	{
		MaterialData* pMaterialData = (MaterialData*)createImpl(nGroup,
																strName,
																strGroupName,
																getNextHandle(),
																false,
																nullptr,
																nullptr);
		return pMaterialData;
	}
	bool MaterialDataManager::AddMaterialData(MaterialData* pMaterialData)
	{
		if (GetMaterialData(pMaterialData->GetName(), pMaterialData->GetGroupName()))
			return false;

		addImpl(pMaterialData);
		ResourceGroupManager::GetSingleton()._NotifyResourceCreated(pMaterialData);
		return true;
	}
	MaterialData* MaterialDataManager::LoadMaterialData(uint32 nGroup, const String& strName, bool bIsFromFile, const String& strGroupName /*= ResourceGroupManager::ms_strNameResourceGroup_AutoDetect*/)
	{
		if (m_pMaterialDataSerializer == nullptr)
            return nullptr;

        MaterialData* pMaterialData = GetMaterialData(strName, strGroupName);
        if (pMaterialData != nullptr)
        {
            pMaterialData->AddRef();
        }
        else
        {
			pMaterialData = NewMaterialData(nGroup, strName, strGroupName);
			if (bIsFromFile)
			{
				if (!Parser(nGroup, strName, pMaterialData))
				{
					delete pMaterialData;
					return nullptr;
				}
				AddMaterialData(pMaterialData);
			}
			else
			{
				Material* pMaterialDefault = MaterialManager::GetSingleton().GetMaterial_Default();
				pMaterialData->SerializerFrom(pMaterialDefault->GetMaterialData());
				pMaterialData->SetPath(pMaterialDefault->GetMaterialData()->GetPath());
			}
        }
        return pMaterialData;
	}
	void MaterialDataManager::UnloadMaterialData(MaterialData* pMaterialData)
	{
		if (!pMaterialData)
            return;
        Delete(pMaterialData);
	}

	bool MaterialDataManager::HasMaterialData(const String& strName)
	{
		return GetResourceByName(strName) != nullptr;
	}	
	bool MaterialDataManager::HasMaterialData(const String& strName, const String& strGroupName)
	{
		return GetResourceByName(strName, strGroupName) != nullptr;
	}
	MaterialData* MaterialDataManager::GetMaterialData(const String& strName)
	{
		Resource* pResource = GetResourceByName(strName);
        if (pResource == nullptr)
            return nullptr;
        return (MaterialData*)pResource;
	}
	MaterialData* MaterialDataManager::GetMaterialData(const String& strName, const String& strGroupName)
	{
		Resource* pResource = GetResourceByName(strName, strGroupName);
        if (pResource == nullptr)
            return nullptr;
        return (MaterialData*)pResource;
	}

	ResourceCreateOrRetrieveResult MaterialDataManager::CreateOrRetrieveMaterialData(const String& strPath,
																					 uint32 nGroup, 
																					 const String& strName, 
																					 const String& strGroupName, 
																					 bool bIsManualLoad /*= false*/,
																					 ResourceManualLoader* pManualLoader /*= nullptr*/, 
																					 const NameValuePairMap* pLoadParams /*= nullptr*/)
    {
        NameValuePairMap mapMeshParam;
        if (bIsManualLoad && pLoadParams)
        {
            FUtil::CopyNameValuePairMapTo(pLoadParams, &mapMeshParam);
        }

        ResourceCreateOrRetrieveResult result = ResourceManager::CreateOrRetrieve(nGroup,
                                                                                  strName,
                                                                                  strGroupName,
                                                                                  bIsManualLoad,
                                                                                  pManualLoader,
                                                                                  &mapMeshParam);
		if (!result.first || !result.second)
		{
            F_LogError("*********************** MaterialDataManager::CreateOrRetrieveMaterialData: CreateOrRetrieve resource failed, group: [%d], name: [%s]", nGroup, strName.c_str());
			return result;
		}

        MaterialData* pMaterialData = (MaterialData*)result.first;
        pMaterialData->SetPath(strPath);

		return result;
    }

    MaterialData* MaterialDataManager::Prepare(const String& strPath,
											   uint32 nGroup, 
											   const String& strName, 
											   const String& strGroupName)
    {
        ResourceCreateOrRetrieveResult result = CreateOrRetrieveMaterialData(strPath,
																			 nGroup,
																			 strName,
																			 strGroupName,
																			 false,
																			 nullptr,
																			 nullptr);
		MaterialData* pMaterialData = (MaterialData*)result.first;
        if (!pMaterialData)
            return nullptr;
		pMaterialData->Prepare();

		return pMaterialData;
    }

    MaterialData* MaterialDataManager::CreateMaterialData(const String& strPath,
														  uint32 nGroup, 
														  const String& strName, 
														  const String& strGroupName)         
    {
        ResourceCreateOrRetrieveResult result = CreateOrRetrieveMaterialData(strPath,
																			 nGroup,
																			 strName,
																			 strGroupName,
																			 false,
																			 nullptr,
																			 nullptr);
		MaterialData* pMaterialData = (MaterialData*)result.first;
        if (!pMaterialData)
            return nullptr;
		pMaterialData->Load();

        pMaterialData->AddRef();
		return pMaterialData;
    }       

	Resource* MaterialDataManager::createImpl(uint32 nGroup,
											  const String& strName,
											  const String& strGroupName,
											  ResourceHandle nHandle, 
											  bool bIsManualLoad,
											  ResourceManualLoader* pManualLoader, 
											  const NameValuePairMap* pLoadParams)
    {
        return new MaterialData(this,
								nGroup, 
								strName,
								strGroupName,
								nHandle,
								bIsManualLoad,
								pManualLoader);
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

	bool MaterialDataManager::ParserXML(uint32 nGroup, const String& strName, const String& strPath, MaterialDataPtrVector* pRet /*= nullptr*/)
	{
		return m_pMaterialDataSerializer->ParserXML(nGroup, strName, strPath, nullptr, pRet);
	}

	bool MaterialDataManager::ParserBinary(uint32 nGroup, const String& strName, MaterialDataPtrVector* pRet /*= nullptr*/)
	{
		return m_pMaterialDataSerializer->ParserBinary(nGroup, strName, pRet);
	}

	bool MaterialDataManager::ParserBinary(uint32 nGroup, const String& strName, const String& strPath, MaterialDataPtrVector* pRet /*= nullptr*/)
	{
		return m_pMaterialDataSerializer->ParserBinary(nGroup, strName, strPath, nullptr, pRet);
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

	bool MaterialDataManager::SaveXML(const String& strPath, MaterialPtrVector& aMA)
	{
		return m_pMaterialDataSerializer->SaveXML(strPath, aMA);
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

	bool MaterialDataManager::SaveBinary(const String& strPath, MaterialPtrVector& aMA)
	{
		return m_pMaterialDataSerializer->SaveBinary(strPath, aMA);
	}

}; //LostPeterEngine