/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
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

namespace LostPeter
{
    class utilExport Material : public Base
    {
    public:
        Material(uint32 _group,
                 const String& _nameMaterial,
                 const String& _pathMaterial);
        virtual ~Material();

    public:
        String pathMaterial;

    protected:
        MaterialData* pMaterialData;
        RenderState* pRenderState;

    public:
        LP_FORCEINLINE MaterialData* GetMaterialData() const { return pMaterialData; }
		LP_FORCEINLINE void SetMaterialData(MaterialData* pMaterialData) { pMaterialData = pMaterialData; }
        LP_FORCEINLINE RenderState*	GetRenderState() const { return pRenderState; }

    public:
        void Destroy();

        bool LoadMaterial(bool bNew = false);
		bool UnloadMaterial();

	private:
		void init();
    };

}; //LostPeter

#endif