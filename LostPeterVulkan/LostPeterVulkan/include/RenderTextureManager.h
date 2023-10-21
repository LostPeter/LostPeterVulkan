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

#ifndef _RENDER_TEXTURE_MANAGER_H_
#define _RENDER_TEXTURE_MANAGER_H_

#include "Base.h"

namespace LostPeter
{
    class utilExport RenderTextureManager : public FSingleton<RenderTextureManager>
                                          , public Base
    {
    public:
        RenderTextureManager();
        virtual ~RenderTextureManager();

    public:
    protected:
        RenderTexturePtrVector m_aRenderTexture;
        RenderTexturePtrMap m_mapRenderTexture;

    public:
        LP_FORCEINLINE const RenderTexturePtrVector& GetRenderTexturePtrVector() const { return m_aRenderTexture; }
        LP_FORCEINLINE RenderTexturePtrVector& GetRenderTexturePtrVector() { return m_aRenderTexture; }
        LP_FORCEINLINE const RenderTexturePtrMap& GetRenderTexturePtrMap() const { return m_mapRenderTexture; }
        LP_FORCEINLINE RenderTexturePtrMap& GetRenderTexturePtrMap() { return m_mapRenderTexture; }

    public:
        static RenderTextureManager& GetSingleton();
		static RenderTextureManager* GetSingletonPtr();


    };

}; //LostPeter

#endif