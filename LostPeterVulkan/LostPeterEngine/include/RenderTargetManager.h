/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-22
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _RENDER_TARGET_MANAGER_H_
#define _RENDER_TARGET_MANAGER_H_

#include "Base.h"

namespace LostPeterEngine
{
    class engineExport RenderTargetManager : public FSingleton<RenderTargetManager>
                                           , public Base
    {
    public:
        RenderTargetManager(Renderer* pRenderer);
        virtual ~RenderTargetManager();

    public:
    protected:
        Renderer* m_pRenderer;
        RenderTargetPtrMap m_mapRenderTarget;
		RenderTargetPtrPriorityMap m_mapPriorityRenderTarget;

    public:
        E_FORCEINLINE const RenderTargetPtrMap& GetRenderTargetPtrMap() const { return m_mapRenderTarget; }
        E_FORCEINLINE RenderTargetPtrMap& GetRenderTargetPtrMap() { return m_mapRenderTarget; }
        E_FORCEINLINE const RenderTargetPtrPriorityMap& GetRenderTargetPtrPriorityMap() const { return m_mapPriorityRenderTarget; }
        E_FORCEINLINE RenderTargetPtrPriorityMap& GetRenderTargetPtrPriorityMap() { return m_mapPriorityRenderTarget; }

    public:
        static RenderTargetManager& GetSingleton();
		static RenderTargetManager* GetSingletonPtr();

    public:
        void Destroy();
        bool Init();

    public:
        virtual RenderTarget* GetRenderTarget(const String& strName);
		virtual void AttachRenderTarget(RenderTarget* pRenderTarget);
		virtual RenderTarget* DetachRenderTarget(const String& strName);
		virtual void DestroyRenderTarget(const String& strName);
        virtual void DestroyRenderTargetAll();

		virtual void DestroyRenderWindow(const String& strName);
		virtual void DestroyRenderTexture(const String& strName);

    };

}; //LostPeterEngine

#endif