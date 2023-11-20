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

#ifndef _MATERIAL_DATA_H_
#define _MATERIAL_DATA_H_

#include "Base.h"
#include "RenderState.h"

namespace LostPeterEngine
{
    class utilExport MaterialData : public Base
                                  , public FNonCopyable
    {
    public:
        MaterialData(const String& nameMaterialData);
        virtual ~MaterialData();

    public:
    protected:
        String m_strPath;
        RenderStatePtrVector m_aRenderState;
        RenderStatePtrMap m_mapRenderState;

    public:
        LP_FORCEINLINE const String& GetPath() const { return m_strPath; }
        LP_FORCEINLINE void SetPath(const String& path) { m_strPath = path; }
		LP_FORCEINLINE const RenderStatePtrVector& GetRenderStatePtrVector() const { return m_aRenderState; }
        LP_FORCEINLINE RenderStatePtrVector& GetRenderStatePtrVector() { return m_aRenderState; }
        LP_FORCEINLINE const RenderStatePtrMap& GetRenderStatePtrMap() const { return m_mapRenderState; }
        LP_FORCEINLINE RenderStatePtrMap& GetRenderStatePtrMap() { return m_mapRenderState; }
        
    public:
		void Destroy();

	public:
		bool HasRenderState(const String& strName);
        RenderState* GetRenderState(const String& strName);
        bool AddRenderState(RenderState* pRenderState);
        void DeleteRenderState(const String& strName);
        void DeleteRenderStateAll();

    public:
        void SerializerFrom(const MaterialData* pMaterialData);
		
    };

}; //LostPeterEngine

#endif