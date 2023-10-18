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
        static uint32 ms_nInstanceID;
		static String ms_strMaterialName_Default;
        static String ms_strMaterialName_DefaultOpaque;
        static String ms_strMaterialName_DefaultTransparent;

    protected:
        MaterialSerializer* m_pMaterialSerializer;
        MaterialPtrVector m_aMaterial;
        MaterialGroupPtrMap m_mapMaterialGroup;


        Material* m_pMaterial_Default;
		MaterialInstance* m_pMaterialInstance_Default;
        Material* m_pMaterial_DefaultOpaque;
		MaterialInstance* m_pMaterialInstance_DefaultOpaque;
        Material* m_pMaterial_DefaultTransparent;
		MaterialInstance* m_pMaterialInstance_DefaultTransparent;

    public:
        LP_FORCEINLINE MaterialSerializer* GetMaterialSerializer() const { return m_pMaterialSerializer; }
        LP_FORCEINLINE const MaterialPtrVector& GetMaterialPtrVector() const { return m_aMaterial; }
        LP_FORCEINLINE MaterialPtrVector& GetMaterialPtrVector() { return m_aMaterial; }
        LP_FORCEINLINE const MaterialGroupPtrMap& GetMaterialGroupPtrMap() const { return m_mapMaterialGroup; }
        LP_FORCEINLINE MaterialGroupPtrMap& GetMaterialGroupPtrMap() { return m_mapMaterialGroup; }

        LP_FORCEINLINE Material* GetMaterial_Default() const { return m_pMaterial_Default; }
		LP_FORCEINLINE MaterialInstance* GetMaterialInstance_Default() const { return m_pMaterialInstance_Default; }
        LP_FORCEINLINE Material* GetMaterial_DefaultOpaque() const { return m_pMaterial_DefaultOpaque; }
		LP_FORCEINLINE MaterialInstance* GetMaterialInstance_DefaultOpaque() const { return m_pMaterialInstance_DefaultOpaque; }
        LP_FORCEINLINE Material* GetMaterial_DefaultTransparent() const { return m_pMaterial_DefaultTransparent; }
		LP_FORCEINLINE MaterialInstance* GetMaterialInstance_DefaultTransparent() const { return m_pMaterialInstance_DefaultTransparent; }

    public:
        static MaterialManager&	GetSingleton();
		static MaterialManager*	GetSingletonPtr();

    public:
        void Destroy();
        bool Init(uint nGroupCfgMesh, const String& strNameCfg);

    protected:
        bool initMaterialDefault();

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