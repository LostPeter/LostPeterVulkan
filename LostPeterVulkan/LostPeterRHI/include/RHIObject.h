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
        uint32 m_nID;
        String m_strName;

    public:
        F_FORCEINLINE RHIDevice* GetDevice() const { return m_pDevice; }
        F_FORCEINLINE RHIObjectType GetObjectType() const { return m_eObject; }
        F_FORCEINLINE uint32 GetID() const { return m_nID; }
        F_FORCEINLINE const String& GetName() const { return m_strName; }

    public:
        virtual void Destroy() = 0;

    protected:
        virtual void setObjectType(RHIObjectType eObject);
    };

}; //LostPeterRHI

#endif