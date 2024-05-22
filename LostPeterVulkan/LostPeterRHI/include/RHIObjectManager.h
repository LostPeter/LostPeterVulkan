/****************************************************************************
* LostPeterRHI - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-22
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _RHI_OBJECT_MANAGER_H_
#define _RHI_OBJECT_MANAGER_H_

#include "RHIPreDefine.h"

namespace LostPeterRHI
{
    class rhiExport RHIObjectManager : public FSingleton<RHIObjectManager>
    {
    public:
        RHIObjectManager(RHIDevice* pDevice);
        virtual ~RHIObjectManager();

    public:
    protected:
        RHIDevice* m_pDevice;


    public:
        static RHIObjectManager& GetSingleton();
		static RHIObjectManager* GetSingletonPtr();

    public:
        F_FORCEINLINE RHIDevice* GetDevice() { return m_pDevice; }


    public:
        virtual void Destroy() = 0;
        
    };

}; //LostPeterRHI

#endif