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

#ifndef _SHADER_PROGRAM_SERIALIZER_H_
#define _SHADER_PROGRAM_SERIALIZER_H_

#include "Base.h"

namespace LostPeterEngine
{
    class engineExport ShaderProgramSerializer : public FSerializer
                                               , public Base
    {
    public:
        ShaderProgramSerializer();
        virtual ~ShaderProgramSerializer();

    public:
    protected:
        ShaderProgramInfoPtrVector m_aShaderProgramInfo;
        ShaderProgramInfoGroupPtrMap m_mapShaderProgramInfoGroup;

    public:
        F_FORCEINLINE const ShaderProgramInfoPtrVector& GetShaderProgramInfoPtrVector() const { return m_aShaderProgramInfo; }
        F_FORCEINLINE ShaderProgramInfoPtrVector& GetShaderProgramInfoPtrVector() { return m_aShaderProgramInfo; }
        F_FORCEINLINE const ShaderProgramInfoGroupPtrMap& GetShaderProgramInfoGroupPtrMap() const { return m_mapShaderProgramInfoGroup; }
        F_FORCEINLINE ShaderProgramInfoGroupPtrMap& GetShaderProgramInfoGroupPtrMap() { return m_mapShaderProgramInfoGroup; }

    
    public:
        bool HasShaderProgramInfo(uint nGroup, const String& strName);
        ShaderProgramInfo* GetShaderProgramInfo(uint nGroup, const String& strName);
        bool AddShaderProgramInfo(ShaderProgramInfo* pShaderProgramInfo);
        void DeleteShaderProgramInfo(uint nGroup, const String& strName);
        void DeleteShaderProgramInfoAll();

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