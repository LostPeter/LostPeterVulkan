/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-15
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VK_DESCRIPTOR_SET_LAYOUT_MANAGER_H_
#define _VK_DESCRIPTOR_SET_LAYOUT_MANAGER_H_

#include "Base.h"

namespace LostPeter
{
    class utilExport VKDescriptorSetLayoutManager : public FSingleton<VKDescriptorSetLayoutManager>
                                                  , public Base
    {
    public:
        VKDescriptorSetLayoutManager();
        virtual ~VKDescriptorSetLayoutManager();

    public:
    protected:
        VKDescriptorSetLayoutSerializer* m_pVKDescriptorSetLayoutSerializer;
        VKDescriptorSetLayoutPtrVector m_aVKDescriptorSetLayout;
        VKDescriptorSetLayoutPtrMap m_mapVKDescriptorSetLayout;

    public:
        LP_FORCEINLINE VKDescriptorSetLayoutSerializer* GetVKDescriptorSetLayoutSerializer() const { return m_pVKDescriptorSetLayoutSerializer; }
        LP_FORCEINLINE const VKDescriptorSetLayoutPtrVector& GetVKDescriptorSetLayoutPtrVector() const { return m_aVKDescriptorSetLayout; }
        LP_FORCEINLINE VKDescriptorSetLayoutPtrVector& GetVKDescriptorSetLayoutPtrVector() { return m_aVKDescriptorSetLayout; }
        LP_FORCEINLINE const VKDescriptorSetLayoutPtrMap& GetVKDescriptorSetLayoutMap() const { return m_mapVKDescriptorSetLayout; }
        LP_FORCEINLINE VKDescriptorSetLayoutPtrMap& GetVKDescriptorSetLayoutMap() { return m_mapVKDescriptorSetLayout; }

    public:
        static VKDescriptorSetLayoutManager& GetSingleton();
		static VKDescriptorSetLayoutManager* GetSingletonPtr();

    public:
        void Destroy();
        bool Init(uint nGroup, const String& strNameCfg);

    public:
        bool LoadVKDescriptorSetLayoutAll();
        VKDescriptorSetLayout* LoadVKDescriptorSetLayout(const String& strName);
        void UnloadVKDescriptorSetLayout(VKDescriptorSetLayout* pVKDescriptorSetLayout);

        bool HasVKDescriptorSetLayout(const String& strName);
        VKDescriptorSetLayout* GetVKDescriptorSetLayout(const String& strName);
        bool AddVKDescriptorSetLayout(VKDescriptorSetLayout* pVKDescriptorSetLayout);
        void DeleteVKDescriptorSetLayout(const String& strName);
        void DeleteVKDescriptorSetLayoutAll();

    private:
        VKDescriptorSetLayout* loadVKDescriptorSetLayout(DescriptorSetLayoutInfo* pDSLI);
    };

}; //LostPeter

#endif