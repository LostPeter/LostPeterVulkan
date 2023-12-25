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
        F_FORCEINLINE const RenderPassPtrVector& GetRenderPassPtrVector() const { return m_aRenderPass; }
        F_FORCEINLINE RenderPassPtrVector& GetRenderPassPtrVector() { return m_aRenderPass; }
        F_FORCEINLINE const RenderPassPtrMap& GetRenderPassPtrMap() const { return m_mapRenderPass; }
        F_FORCEINLINE RenderPassPtrMap& GetRenderPassPtrMap() { return m_mapRenderPass; }

    public:
        static RenderPassManager& GetSingleton();
		static RenderPassManager* GetSingletonPtr();

    public:
        virtual void Destroy();
        virtual bool Init();

    public:
        virtual RenderPassDescriptor* GetRenderPassDescriptor(const String& nameRenderPassDescriptor) = 0;
        virtual RenderPassDescriptor* CreateRenderPassDescriptor(const String& nameRenderPassDescriptor, FRenderPassType eRenderPass) = 0;
        virtual void DestroyRenderPassDescriptor(RenderPassDescriptor* pRenderPassDescriptor) = 0;
        virtual void DestroyRenderPassDescriptorAll() = 0;

        virtual void DestroyRenderPassAll();
    };

}; //LostPeterEngine

#endif