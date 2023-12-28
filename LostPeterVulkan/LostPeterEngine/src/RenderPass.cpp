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

#include "../include/RenderPass.h"
#include "../include/RenderPassDescriptor.h"
#include "../include/RenderPassDescriptorManager.h"

namespace LostPeterEngine
{
    RenderPass::RenderPass(const String& nameRenderPass, 
                           FRenderPassType eRenderPass,
                           const String& nameRenderPassDescriptor)
        : Base(nameRenderPass)
        , m_eRenderPass(eRenderPass)
        , m_strNameRenderPassDescriptor(nameRenderPassDescriptor)
        , m_pRenderPassDescriptor(nullptr)
    {

    }

    RenderPass::~RenderPass()
    {

    }

    void RenderPass::Destroy()
    {
        destroyRenderPassDescriptor();
    }
        void RenderPass::destroyRenderPassDescriptor()
        {
            RenderPassDescriptorManager::GetSingleton().DestroyRenderPassDescriptor(m_pRenderPassDescriptor);
            m_pRenderPassDescriptor = nullptr;
        }
    
    bool RenderPass::Init()
    {
        //1> createRenderPassDescriptor
        if (!createRenderPassDescriptor())
        {
            F_LogError("*********************** RenderPass::Init: createRenderPassDescriptor failed, typeRenderPass: [%s] !", F_GetRenderPassTypeName(this->m_eRenderPass).c_str());
            return false;
        }

        return true;
    }
        bool RenderPass::createRenderPassDescriptor()
        {
            m_pRenderPassDescriptor = RenderPassDescriptorManager::GetSingleton().CreateRenderPassDescriptor(m_strNameRenderPassDescriptor, m_eRenderPass);
            if (m_pRenderPassDescriptor == nullptr)
            {
                return false;
            }

            return true;    
        }
    

}; //LostPeterEngine