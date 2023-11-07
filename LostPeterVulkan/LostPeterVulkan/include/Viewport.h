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

#ifndef _VIEWPORT_H_
#define _VIEWPORT_H_

#include "Base.h"

namespace LostPeter
{
    class utilExport Viewport : public Base
							  , public FNonCopyable
    {
    public:
        Viewport(const String& nameViewport);
        virtual ~Viewport();

    public:
    protected:
        VkViewportVector m_aViewports;
        VkRect2DVector m_aScissors;

    public:
        LP_FORCEINLINE const VkViewportVector& GetVkViewportVector() const { return this->m_aViewports; }
        LP_FORCEINLINE VkViewportVector& GetVkViewportVector() { return this->m_aViewports; }
        LP_FORCEINLINE const VkRect2DVector& GetVkRect2DVector() const { return this->m_aScissors; }
        LP_FORCEINLINE VkRect2DVector& GetVkRect2DVector() { return this->m_aScissors; }

    public:
        void Destroy();
        bool Init(float vpStartX, 
                  float vpStartY, 
                  float vpWidth, 
                  float vpHeight,
                  float vpMinDepth,
                  float vpMaxDepth,
                  VkOffset2D scOffset,
                  VkExtent2D scExtent);

        void AddViewport(float vpStartX, 
                         float vpStartY, 
                         float vpWidth, 
                         float vpHeight,
                         float vpMinDepth,
                         float vpMaxDepth,
                         VkOffset2D scOffset,
                         VkExtent2D scExtent);

    public:
        void RefreshViewport(int index, const VkExtent2D& vkExtent);
    
    };

}; //LostPeter

#endif