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

#ifndef _RHI_OBJECT_H_
#define _RHI_OBJECT_H_

#include "RHIPreDefine.h"

namespace LostPeterRHI
{
    class rhiExport RHIObject : public FNonCopyable
    {
    public:
        RHIObject(RHIDevice* pDevice);
        virtual ~RHIObject();

    public:
    protected:
        RHIDevice* m_pDevice;
        RHIObjectType m_eObject;

    public:
        F_FORCEINLINE RHIDevice* GetDevice() { return m_pDevice; }
        F_FORCEINLINE RHIObjectType GetObjectType() const { return m_eObject; }
        

    public:
        virtual void Destroy() = 0;

    };

}; //LostPeterRHI

#endif