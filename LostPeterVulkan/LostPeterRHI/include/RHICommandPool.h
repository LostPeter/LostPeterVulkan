/****************************************************************************
* LostPeterRHI - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-18
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _RHI_COMMAND_POOL_H_
#define _RHI_COMMAND_POOL_H_

#include "RHIObject.h"

namespace LostPeterRHI
{
    class rhiExport RHICommandPool : public RHIObject
    {
    public:
        RHICommandPool(RHIDevice* pDevice);
        virtual ~RHICommandPool();

    public:
    protected:
        

    public:

        
    };
    
}; //LostPeterRHI

#endif