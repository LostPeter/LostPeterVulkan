/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-22
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VK_PIPELINE_MANAGER_H_
#define _VK_PIPELINE_MANAGER_H_

#include "Base.h"

namespace LostPeter
{
    class utilExport VKPipelineManager : public FSingleton<VKPipelineManager>
                                       , public Base
    {
    public:
        VKPipelineManager();
        virtual ~VKPipelineManager();

    public:
    protected:
        VKPipelinePtrVector m_aVKPipeline;
        VKPipelinePtrMap m_mapVKPipeline;

    public:
        LP_FORCEINLINE const VKPipelinePtrVector& GetVKPipelinePtrVector() const { return m_aVKPipeline; }
        LP_FORCEINLINE VKPipelinePtrVector& GetVKPipelinePtrVector() { return m_aVKPipeline; }
        LP_FORCEINLINE const VKPipelinePtrMap& GetVKPipelinePtrMap() const { return m_mapVKPipeline; }
        LP_FORCEINLINE VKPipelinePtrMap& GetVKPipelinePtrMap() { return m_mapVKPipeline; }

    public:
        static VKPipelineManager& GetSingleton();
		static VKPipelineManager* GetSingletonPtr();

    public:
        void Destroy();
        bool Init();

    public:
        VKPipeline* LoadVKPipeline(const String& strName);

        bool HasVKPipeline(const String& strName);
        VKPipeline* GetVKPipeline(const String& strName);
        bool AddVKPipeline(VKPipeline* pVKPipeline);
        void DeleteVKPipeline(const String& strName);
        void DeleteVKPipelineAll();

    private:
        VKPipeline* loadVKPipeline(const String& strName);
    };

}; //LostPeter

#endif