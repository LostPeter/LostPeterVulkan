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
        RenderPass*	m_ppRenderPass[F_RenderPass_Count];
        RenderPassPtrMap m_mapRenderPass;

    public:
        F_FORCEINLINE const RenderPassPtrMap& GetRenderPassPtrMap() const { return m_mapRenderPass; }
        F_FORCEINLINE RenderPassPtrMap& GetRenderPassPtrMap() { return m_mapRenderPass; }
        RenderPass* GetRenderPassByType(FRenderPassType eRenderPass);
        RenderPass* GetRenderPassByName(const String& strName);

    public:
        static RenderPassManager& GetSingleton();
		static RenderPassManager* GetSingletonPtr();

    public:
        void Destroy();
        bool Init();

        void SetRenderTarget(RenderTarget* pRenderTarget);
		void SetSceneManager(SceneManager* pSceneManager);
		void SetRenderTargetAndSceneManager(RenderTarget* pRenderTarget, SceneManager* pSceneManager);
        
    public:
        bool RegisterRenderPassCustom(RenderPass* pRenerPass);
		bool UnRegisterRenderPassCustom(RenderPass* pRenerPass);
    };

}; //LostPeterEngine

#endif