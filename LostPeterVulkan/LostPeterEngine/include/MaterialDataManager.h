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

#include "Base.h"

namespace LostPeterEngine
{
    class utilExport MaterialDataManager : public FSingleton<MaterialDataManager>
                                         , public Base
    {
        friend class MaterialDataSerializer;

    public:
        MaterialDataManager();
        virtual ~MaterialDataManager();

    public:
    protected:
        MaterialDataSerializer* m_pMaterialDataSerializer;
		MaterialDataPtrMap m_mapMaterialData;

    public:
        static MaterialDataManager&	GetSingleton();
		static MaterialDataManager*	GetSingletonPtr();

    public:
        void Destroy();

    public:
		MaterialData* GetMaterialData(const String& strName);
			
		MaterialData* CreateMaterialData(uint32 nGroup, const String& strName, bool bIsFromFile = true);
		void DestroyMaterialData(const String& strName);
		void DestroyMaterialData(MaterialData* pMaterialData);
		void DestroyAllMaterialData();

	private:
		MaterialData* findMaterialData(const String& strName);
		bool addMaterialData(MaterialData* pMaterialData);
        
    public:
        bool Parser(uint32 nGroup, const String& strName, MaterialData* pMaterialData);
        bool Parser(uint32 nGroup, const String& strName, MaterialDataPtrVector* pRet = nullptr);

		bool ParserXML(uint32 nGroup, const String& strName, MaterialDataPtrVector* pRet = nullptr);
		bool ParserXML(const char* szFilePath, MaterialDataPtrVector* pRet = nullptr);

		bool ParserBinary(uint32 nGroup, const String& strName, MaterialDataPtrVector* pRet = nullptr);
		bool ParserBinary(const char* szFilePath, MaterialDataPtrVector* pRet = nullptr);

    public:
        bool SaveXML(Material* pMaterial);
		bool SaveXML(uint32 nGroup, Material* pMaterial);
		bool SaveXML(Mesh* pMesh);
		bool SaveXML(Object* pObject, const String& strPath);
		bool SaveXML(const char* szFilePath, MaterialPtrVector& aMA);

		bool SaveBinary(Material* pMaterialData);
		bool SaveBinary(uint32 nGroup, Material* pMaterial);
		bool SaveBinary(Mesh* pMesh);
		bool SaveBinary(Object* pObject, const String& strPath);
		bool SaveBinary(const char* szFilePath, MaterialPtrVector& aMA);
    };

}; //LostPeterEngine

#endif