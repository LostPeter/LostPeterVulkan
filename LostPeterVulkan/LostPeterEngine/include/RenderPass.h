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

#ifndef _RENDER_PASS_H_
#define _RENDER_PASS_H_

#include "Base.h"

namespace LostPeterEngine
{
    class engineExport RenderPass : public Base
                                  , public FNonCopyable
    {
    public:
        RenderPass(const String& nameRenderPass,
                   FRenderPassType eRenderPass,
                   const String& nameRenderPassDescriptor);
        virtual ~RenderPass();

    public:
    protected:
        FRenderPassType m_eRenderPass;
        String m_strNameRenderPassDescriptor;
        RenderPassDescriptor* m_pRenderPassDescriptor;

    public:
        F_FORCEINLINE FRenderPassType GetRenderPassType() const { return m_eRenderPass; }
        F_FORCEINLINE const String& GetRenderPassDescriptorName() const { return m_strNameRenderPassDescriptor; }
        F_FORCEINLINE RenderPassDescriptor* GetRenderPassDescriptor() const { return m_pRenderPassDescriptor; }


    public:
        virtual void Destroy();
        virtual bool Init();

    
    public:
    protected:
        void destroyRenderPassDescriptor();

        bool createRenderPassDescriptor();
    };

}; //LostPeterEngine

#endif