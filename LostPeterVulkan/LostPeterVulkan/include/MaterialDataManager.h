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

#ifndef _MATERIAL_DATA_MANAGER_H_
#define _MATERIAL_DATA_MANAGER_H_

#include "Base.h"

namespace LostPeter
{
    class utilExport MaterialDataManager : public FSingleton<MaterialDataManager>
                                         , public Base
    {
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
			
		MaterialData* CreateMaterialData(uint32 nGroup, const String& strName, bool bNew = false);
		void DestroyMaterialData(const String& strName);
		void DestroyMaterialData(MaterialData* pMaterialData);
		void DestroyAllMaterialData();

	private:
		MaterialData* findMaterialData(const String& strName);
		bool addMaterialData(MaterialData* pMaterialData);
        
    public:


    };

}; //LostPeter

#endif