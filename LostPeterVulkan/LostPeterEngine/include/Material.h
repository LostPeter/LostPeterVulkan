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

#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "Base.h"

namespace LostPeterEngine
{
    class utilExport Material : public Base
    {
    public:
        Material(uint32 _group,
                 const String& _nameMaterial);
        virtual ~Material();

    public:
    protected:
        MaterialData* m_pMaterialData;

    public:
        LP_FORCEINLINE MaterialData* GetMaterialData() const { return m_pMaterialData; }
		LP_FORCEINLINE void SetMaterialData(MaterialData* pMaterialData) { m_pMaterialData = pMaterialData; }

    public:
        void Destroy();

    public:
        bool LoadMaterial(bool bIsFromFile = true);
		void UnloadMaterial();

	private:
		void init();
    };

}; //LostPeterEngine

#endif