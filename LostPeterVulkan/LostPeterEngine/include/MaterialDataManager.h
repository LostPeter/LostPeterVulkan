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

#ifndef _MATERIAL_DATA_MANAGER_H_
#define _MATERIAL_DATA_MANAGER_H_

#include "ResourceManager.h"

namespace LostPeterEngine
{
    class engineExport MaterialDataManager : public FSingleton<MaterialDataManager>
                                           , public ResourceManager
    {
        friend class MaterialDataSerializer;

    public:
        MaterialDataManager();
        virtual ~MaterialDataManager();

    public:
    protected:
        MaterialDataSerializer* m_pMaterialDataSerializer;

    public:
        static MaterialDataManager&	GetSingleton();
		static MaterialDataManager*	GetSingletonPtr();

    public:
        virtual void Destroy();

	public:
		MaterialData* NewMaterialData(uint32 nGroup, const String& strName, const String& strGroupName = ResourceGroupManager::ms_strNameResourceGroup_AutoDetect);
		bool AddMaterialData(MaterialData* pMaterialData);
		
		MaterialData* LoadMaterialData(uint32 nGroup, const String& strName, bool bIsFromFile, const String& strGroupName = ResourceGroupManager::ms_strNameResourceGroup_AutoDetect);
        void UnloadMaterialData(MaterialData* pMaterialData);

        bool HasMaterialData(const String& strName);
        bool HasMaterialData(const String& strName, const String& strGroupName);
        MaterialData* GetMaterialData(const String& strName);
        MaterialData* GetMaterialData(const String& strName, const String& strGroupName);

	public:
		virtual ResourceCreateOrRetrieveResult CreateOrRetrieveMaterialData(const String& strPath,
																			uint32 nGroup, 
																			const String& strName, 
																			const String& strGroupName, 
																			bool bIsManualLoad = false,
																			ResourceManualLoader* pManualLoader = nullptr, 
																			const NameValuePairMap* pLoadParams = nullptr);

    public:
        virtual MaterialData* Prepare(const String& strPath,
									  uint32 nGroup, 
									  const String& strName, 
									  const String& strGroupName);

    public:
        virtual MaterialData* CreateMaterialData(const String& strPath,
												 uint32 nGroup, 
												 const String& strName, 
												 const String& strGroupName);

	protected:
        virtual Resource* createImpl(uint32 nGroup,
									 const String& strName,
			                         const String& strGroupName,
									 ResourceHandle nHandle, 
                                     bool bIsManualLoad,
                                     ResourceManualLoader* pManualLoader, 
			                         const NameValuePairMap* pLoadParams);

	public:
        bool Parser(uint32 nGroup, const String& strName, MaterialData* pMaterialData);
        bool Parser(uint32 nGroup, const String& strName, MaterialDataPtrVector* pRet = nullptr);

		bool ParserXML(uint32 nGroup, const String& strName, MaterialDataPtrVector* pRet = nullptr);
		bool ParserXML(uint32 nGroup, const String& strName, const String& strPath, MaterialDataPtrVector* pRet = nullptr);

		bool ParserBinary(uint32 nGroup, const String& strName, MaterialDataPtrVector* pRet = nullptr);
		bool ParserBinary(uint32 nGroup, const String& strName, const String& strPath, MaterialDataPtrVector* pRet = nullptr);

    public:
        bool SaveXML(Material* pMaterial);
		bool SaveXML(uint32 nGroup, Material* pMaterial);
		bool SaveXML(Mesh* pMesh);
		bool SaveXML(Object* pObject, const String& strPath);
		bool SaveXML(const String& strPath, MaterialPtrVector& aMA);

		bool SaveBinary(Material* pMaterialData);
		bool SaveBinary(uint32 nGroup, Material* pMaterial);
		bool SaveBinary(Mesh* pMesh);
		bool SaveBinary(Object* pObject, const String& strPath);
		bool SaveBinary(const String& strPath, MaterialPtrVector& aMA);
    };

}; //LostPeterEngine

#endif