/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-04
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _MATERIAL_INSTANCE_H_
#define _MATERIAL_INSTANCE_H_

#include "Base.h"

namespace LostPeterEngine
{
    class engineExport MaterialInstance : public Base
    {
    public:
        MaterialInstance(const String& nameMaterialInstance, Material* pMaterial);
        virtual ~MaterialInstance();

    public:
    protected:
        Material* m_pMaterial;

    public:
        F_FORCEINLINE Material* GetMaterial() const { return m_pMaterial; }

    public:
        void Destroy();

    public:
        bool LoadMaterialInstance();
		void UnloadMaterialInstance();

    public:
        

    };

}; //LostPeterEngine

#endif