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

#ifndef _SHADER_SERIALIZER_H_
#define _SHADER_SERIALIZER_H_

#include "Base.h"

namespace LostPeterEngine
{
    class engineExport ShaderSerializer : public FSerializer
                                        , public Base
    {
    public:
        ShaderSerializer();
        virtual ~ShaderSerializer();

    public:
    protected:
        ShaderInfoPtrVector m_aShaderInfo;
        ShaderInfoGroupPtrMap m_mapShaderInfoGroup;

    public:
        F_FORCEINLINE const ShaderInfoPtrVector& GetShaderInfoPtrVector() const { return m_aShaderInfo; }
        F_FORCEINLINE ShaderInfoPtrVector& GetShaderInfoPtrVector() { return m_aShaderInfo; }
        F_FORCEINLINE const ShaderInfoGroupPtrMap& GetShaderInfoGroupPtrMap() const { return m_mapShaderInfoGroup; }
        F_FORCEINLINE ShaderInfoGroupPtrMap& GetShaderInfoGroupPtrMap() { return m_mapShaderInfoGroup; }

    
    public:
        bool HasShaderInfo(uint nGroup, const String& strName);
        ShaderInfo* GetShaderInfo(uint nGroup, const String& strName);
        bool AddShaderInfo(ShaderInfo* pShaderInfo);
        void DeleteShaderInfo(uint nGroup, const String& strName);
        void DeleteShaderInfoAll();

    protected:
        //File Content XML
        virtual bool serializeXML();
        virtual bool deserializeXML();

        //File Content Binary
        virtual bool serializeBinary();
        virtual bool deserializeBinary();
    };

}; //LostPeterEngine

#endif