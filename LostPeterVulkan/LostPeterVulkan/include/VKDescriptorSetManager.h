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

#ifndef _VK_DESCRIPTOR_SET_MANAGER_H_
#define _VK_DESCRIPTOR_SET_MANAGER_H_

#include "Base.h"

namespace LostPeter
{
    class utilExport VKDescriptorSetManager : public FSingleton<VKDescriptorSetManager>
                                            , public Base
    {
    public:
        VKDescriptorSetManager();
        virtual ~VKDescriptorSetManager();

    public:
    protected:
        VKDescriptorSetSerializer* m_pVKDescriptorSetSerializer;
        VKDescriptorSetPtrVector m_aVKDescriptorSet;
        VKDescriptorSetPtrMap m_mapVKDescriptorSet;

    public:
        LP_FORCEINLINE VKDescriptorSetSerializer* GetVKDescriptorSetSerializer() const { return m_pVKDescriptorSetSerializer; }
        LP_FORCEINLINE const VKDescriptorSetPtrVector& GetVKDescriptorSetPtrVector() const { return m_aVKDescriptorSet; }
        LP_FORCEINLINE VKDescriptorSetPtrVector& GetVKDescriptorSetPtrVector() { return m_aVKDescriptorSet; }
        LP_FORCEINLINE const VKDescriptorSetPtrMap& GetVKDescriptorSetMap() const { return m_mapVKDescriptorSet; }
        LP_FORCEINLINE VKDescriptorSetPtrMap& GetVKDescriptorSetMap() { return m_mapVKDescriptorSet; }

    public:
        static VKDescriptorSetManager& GetSingleton();
		static VKDescriptorSetManager* GetSingletonPtr();

    public:
        void Destroy();
        bool Init(uint nGroup, const String& strNameCfg);

    public:
        bool LoadVKDescriptorSetAll();
        VKDescriptorSet* LoadVKDescriptorSet(const String& strName);

        bool HasVKDescriptorSet(const String& strName);
        VKDescriptorSet* GetVKDescriptorSet(const String& strName);
        bool AddVKDescriptorSet(VKDescriptorSet* pVKDescriptorSet);
        void DeleteVKDescriptorSet(const String& strName);
        void DeleteVKDescriptorSetAll();

    private:
        VKDescriptorSet* loadVKDescriptorSet(DescriptorSetInfo* pDSI);
    };

}; //LostPeter

#endif