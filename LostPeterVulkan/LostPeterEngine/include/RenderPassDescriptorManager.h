/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-12-28
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _RENDER_PASS_DESCRIPTOR_MANAGER_H_
#define _RENDER_PASS_DESCRIPTOR_MANAGER_H_

#include "Base.h"

namespace LostPeterEngine
{
    class engineExport RenderPassDescriptorManager : public FSingleton<RenderPassDescriptorManager>
                                                   , public Base
    {
    public:
        RenderPassDescriptorManager();
        virtual ~RenderPassDescriptorManager();

    public:
    protected:
        RenderPassDescriptorPtrVector m_aRenderPassDescriptor;
        RenderPassDescriptorPtrMap m_mapRenderPassDescriptor;

    public:
        F_FORCEINLINE const RenderPassDescriptorPtrVector& GetRenderPassDescriptorPtrVector() const { return m_aRenderPassDescriptor; }
        F_FORCEINLINE RenderPassDescriptorPtrVector& GetRenderPassDescriptorPtrVector() { return m_aRenderPassDescriptor; }
        F_FORCEINLINE const RenderPassDescriptorPtrMap& GetRenderPassDescriptorPtrMap() const { return m_mapRenderPassDescriptor; }
        F_FORCEINLINE RenderPassDescriptorPtrMap& GetRenderPassDescriptorPtrMap() { return m_mapRenderPassDescriptor; }

    public:
        static RenderPassDescriptorManager& GetSingleton();
		static RenderPassDescriptorManager* GetSingletonPtr();

    public:
        virtual void Destroy();
        virtual bool Init();

    public:
        virtual RenderPassDescriptor* GetRenderPassDescriptor(const String& nameRenderPassDescriptor) = 0;
        virtual RenderPassDescriptor* CreateRenderPassDescriptor(const String& nameRenderPassDescriptor, FRenderPassType eRenderPass) = 0;
        virtual void DestroyRenderPassDescriptor(RenderPassDescriptor* pRenderPassDescriptor) = 0;
        virtual void DestroyRenderPassDescriptorAll() = 0;
    };

}; //LostPeterEngine

#endif