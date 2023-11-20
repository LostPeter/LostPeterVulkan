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

#ifndef _MATERIAL_DATA_SERIALIZER_H_
#define _MATERIAL_DATA_SERIALIZER_H_

#include "PreDefine.h"

namespace LostPeterEngine
{
    class utilExport MaterialDataSerializer
    {
    public:
        MaterialDataSerializer(MaterialDataManager* pMaterialDataManager);
        virtual ~MaterialDataSerializer();

    public:
        MaterialDataManager* m_pMaterialDataManager;

    public:
        bool Parser(uint32 nGroup, const String& strName, MaterialData* pMaterialData, MaterialDataPtrVector* pRet = nullptr);

		bool ParserXML(uint32 nGroup, const String& strName, MaterialDataPtrVector* pRet = nullptr);
		bool ParserXML(const char* szFilePath, MaterialData* pMaterialData = nullptr, MaterialDataPtrVector* pRet = nullptr);

		bool ParserBinary(uint32 nGroup, const String& strName, MaterialDataPtrVector* pRet = nullptr);
		bool ParserBinary(const char* szFilePath, MaterialData* pMaterialData = nullptr, MaterialDataPtrVector* pRet = nullptr);

    public:
        bool SaveXML(Material* pMaterial);
		bool SaveXML(uint32 nGroup, Material* pMaterial);
		bool SaveXML(Mesh* pMesh);
		bool SaveXML(Object* pObject, const String& strPath);
		bool SaveXML(const char* szFilePath, MaterialPtrVector& aMA);
		
		bool SaveBinary(Material* pMaterial);
		bool SaveBinary(uint32 nGroup, Material* pMaterial);
		bool SaveBinary(Mesh* pMesh);
		bool SaveBinary(Object* pObject, const String& strPath);
		bool SaveBinary(const char* szFilePath, MaterialPtrVector& aMA);
    };

}; //LostPeterEngine

#endif