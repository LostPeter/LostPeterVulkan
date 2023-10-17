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

#ifndef _MATERIAL_MANAGER_H_
#define _MATERIAL_MANAGER_H_

#include "Base.h"

namespace LostPeter
{
    class utilExport MaterialManager : public FSingleton<MaterialManager>
                                     , public Base
    {
    public:
        MaterialManager();
        virtual ~MaterialManager();

    public:
    protected:
        MaterialSerializer* m_pMaterialSerializer;
        MaterialPtrVector m_aMaterial;
        MaterialGroupPtrMap m_mapMaterialGroup;

    public:
        LP_FORCEINLINE MaterialSerializer* GetMaterialSerializer() const { return m_pMaterialSerializer; }
        LP_FORCEINLINE const MaterialPtrVector& GetMaterialPtrVector() const { return m_aMaterial; }
        LP_FORCEINLINE MaterialPtrVector& GetMaterialPtrVector() { return m_aMaterial; }
        LP_FORCEINLINE const MaterialGroupPtrMap& GetMaterialGroupPtrMap() const { return m_mapMaterialGroup; }
        LP_FORCEINLINE MaterialGroupPtrMap& GetMaterialGroupPtrMap() { return m_mapMaterialGroup; }

    public:
        static MaterialManager&	GetSingleton();
		static MaterialManager*	GetSingletonPtr();

    public:
        void Destroy();
        bool Init(uint nGroupCfgMesh, const String& strNameCfg);

    public:
        bool LoadMaterialAll();
        Material* LoadMaterial(uint nGroup, const String& strName);

        bool HasMaterial(uint nGroup, const String& strName);
        Material* GetMaterial(uint nGroup, const String& strName);
        bool AddMaterial(uint nGroup, Material* pMaterial);
        void DeleteMaterial(uint nGroup, const String& strName);
        void DeleteMaterialAll();

    private:
        Material* loadMaterial(MaterialInfo* pMI);
    };

}; //LostPeter

#endif