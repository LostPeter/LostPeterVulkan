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

#ifndef _VK_PIPELINE_LAYOUT_MANAGER_H_
#define _VK_PIPELINE_LAYOUT_MANAGER_H_

#include "Base.h"

namespace LostPeter
{
    class utilExport VKPipelineLayoutManager : public FSingleton<VKPipelineLayoutManager>
                                             , public Base
    {
    public:
        VKPipelineLayoutManager();
        virtual ~VKPipelineLayoutManager();

    public:
    protected:
        VKPipelineLayoutPtrVector m_aVKPipelineLayout;
        VKPipelineLayoutPtrMap m_mapVKPipelineLayout;

    public:
        LP_FORCEINLINE const VKPipelineLayoutPtrVector& GetVKPipelineLayoutPtrVector() const { return m_aVKPipelineLayout; }
        LP_FORCEINLINE VKPipelineLayoutPtrVector& GetVKPipelineLayoutPtrVector() { return m_aVKPipelineLayout; }
        LP_FORCEINLINE const VKPipelineLayoutPtrMap& GetVKPipelineLayoutPtrMap() const { return m_mapVKPipelineLayout; }
        LP_FORCEINLINE VKPipelineLayoutPtrMap& GetVKPipelineLayoutPtrMap() { return m_mapVKPipelineLayout; }

    public:
        static VKPipelineLayoutManager& GetSingleton();
		static VKPipelineLayoutManager* GetSingletonPtr();

    public:
        void Destroy();
        bool Init();

    public:
        bool LoadVKPipelineLayoutAll();
        VKPipelineLayout* LoadVKPipelineLayout(const String& strName);

        bool HasVKPipelineLayout(const String& strName);
        VKPipelineLayout* GetVKPipelineLayout(const String& strName);
        bool AddVKPipelineLayout(VKPipelineLayout* pVKPipelineLayout);
        void DeleteVKPipelineLayout(const String& strName);
        void DeleteVKPipelineLayoutAll();

    private:
        VKPipelineLayout* loadVKPipelineLayout(const String& strName);
    };

}; //LostPeter

#endif