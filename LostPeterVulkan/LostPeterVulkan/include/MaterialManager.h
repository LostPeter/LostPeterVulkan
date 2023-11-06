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
        static uint32 GetNextInstanceID() 
        {
            return ++ ms_nInstanceID;
        }

    protected:
        MaterialSerializer* m_pMaterialSerializer;
        MaterialPtrVector m_aMaterial;
        MaterialGroupPtrMap m_mapMaterialGroup;

        MaterialPtrMap m_mapMaterialDefaults;
        MaterialInstancePtrMap m_mapMaterialInstanceDefaults;

        MaterialInstancePtrMap m_mapMaterialInstance;

    public:
        LP_FORCEINLINE MaterialSerializer* GetMaterialSerializer() const { return m_pMaterialSerializer; }
        LP_FORCEINLINE const MaterialPtrVector& GetMaterialPtrVector() const { return m_aMaterial; }
        LP_FORCEINLINE MaterialPtrVector& GetMaterialPtrVector() { return m_aMaterial; }
        LP_FORCEINLINE const MaterialGroupPtrMap& GetMaterialGroupPtrMap() const { return m_mapMaterialGroup; }
        LP_FORCEINLINE MaterialGroupPtrMap& GetMaterialGroupPtrMap() { return m_mapMaterialGroup; }

        LP_FORCEINLINE MaterialPtrMap& GetMaterialDefaultMap() { return m_mapMaterialDefaults; }

        LP_FORCEINLINE MaterialInstancePtrMap& GetMaterialInstancePtrMap() { return m_mapMaterialInstance; }


        bool IsMaterialDefault(Material* pMaterial);
        Material* GetMaterialDefault(const String& strName);
        Material* GetMaterial_Default();
        Material* GetMaterial_DefaultOpaque();
        Material* GetMaterial_DefaultTransparent();

        bool IsMaterialInstanceDefault(MaterialInstance* pMaterialInstance);
        MaterialInstance* GetMaterialInstanceDefault(const String& strName);
        MaterialInstance* GetMaterialInstance_Default();
        MaterialInstance* GetMaterialInstance_DefaultOpaque();
        MaterialInstance* GetMaterialInstance_DefaultTransparent();

    public:
        static MaterialManager&	GetSingleton();
		static MaterialManager*	GetSingletonPtr();

    public:
        void Destroy();
        bool Init(uint nGroupCfgMesh, const String& strNameCfg);

    protected:
        void destroyMaterialDefaults();
        bool initMaterialDefaults();

        void destroyMaterialInstanceDefaults();
        bool initMaterialInstanceDefaults(); 

    public:
        bool LoadMaterialAll();
        Material* LoadMaterial(uint nGroup, const String& strName);

        bool HasMaterial(uint nGroup, const String& strName);
        Material* GetMaterial(uint nGroup, const String& strName);
        bool AddMaterial(uint nGroup, Material* pMaterial);
        void DeleteMaterial(uint nGroup, const String& strName);
        void DeleteMaterialAll();

    private:
        Material* loadMaterial(uint nGroup, const String& strName, bool bIsFromFile = true);
        Material* loadMaterial(MaterialInfo* pMI);

    public:
        MaterialInstance* CreateMaterialInstance_Default(bool bIsUnique);
        MaterialInstance* CreateMaterialInstance_DefaultOpaque(bool bIsUnique);
        MaterialInstance* CreateMaterialInstance_DefaultTransparent(bool bIsUnique);

        MaterialInstance* CreateMaterialInstance(Material* pMaterial, bool bIsUnique);
		MaterialInstance* CreateMaterialInstance(uint32 nGroup, const String& strMaterialName, bool bIsUnique);
        void DestroyMaterialInstance(MaterialInstance* pMaterialInstance);
		void DestroyMaterialInstanceAll();

    };

}; //LostPeter

#endif