/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-21
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _RENDER_PASS_MANAGER_H_
#define _RENDER_PASS_MANAGER_H_

#include "Base.h"

namespace LostPeterEngine
{
    class engineExport RenderPassManager : public FSingleton<RenderPassManager>
                                         , public Base
    {
    public:
        RenderPassManager();
        virtual ~RenderPassManager();

    public:
    protected:
        RenderPassPtrVector m_aRenderPass;
        RenderPassPtrMap m_mapRenderPass;

    public:
        E_FORCEINLINE const RenderPassPtrVector& GetRenderPassPtrVector() const { return m_aRenderPass; }
        E_FORCEINLINE RenderPassPtrVector& GetRenderPassPtrVector() { return m_aRenderPass; }
        E_FORCEINLINE const RenderPassPtrMap& GetRenderPassPtrMap() const { return m_mapRenderPass; }
        E_FORCEINLINE RenderPassPtrMap& GetRenderPassPtrMap() { return m_mapRenderPass; }

    public:
        static RenderPassManager& GetSingleton();
		static RenderPassManager* GetSingletonPtr();

    public:
        void Destroy();
        bool Init();


    };

}; //LostPeterEngine

#endif