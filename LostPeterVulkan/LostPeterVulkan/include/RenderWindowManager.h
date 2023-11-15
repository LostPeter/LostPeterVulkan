/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-11-15
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _RENDER_WINDOW_MANAGER_H_
#define _RENDER_WINDOW_MANAGER_H_

#include "Base.h"

namespace LostPeter
{
    class utilExport RenderWindowManager : public FSingleton<RenderWindowManager>
                                         , public Base
    {
    public:
        RenderWindowManager();
        virtual ~RenderWindowManager();

    public:
    protected:
        RenderWindowPtrVector m_aRenderWindow;
        RenderWindowPtrMap m_mapRenderWindow;

    public:
        LP_FORCEINLINE const RenderWindowPtrVector& GetRenderTexturePtrVector() const { return m_aRenderWindow; }
        LP_FORCEINLINE RenderWindowPtrVector& GetRenderTexturePtrVector() { return m_aRenderWindow; }
        LP_FORCEINLINE const RenderWindowPtrMap& GetRenderTexturePtrMap() const { return m_mapRenderWindow; }
        LP_FORCEINLINE RenderWindowPtrMap& GetRenderTexturePtrMap() { return m_mapRenderWindow; }

    public:
        static RenderWindowManager& GetSingleton();
		static RenderWindowManager* GetSingletonPtr();


    };

}; //LostPeter

#endif