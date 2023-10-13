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

#ifndef _SHADER_SERIALIZER_H_
#define _SHADER_SERIALIZER_H_

#include "Base.h"

namespace LostPeter
{
    class utilExport ShaderSerializer : public FSerializer
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
        LP_FORCEINLINE const ShaderInfoPtrVector& GetShaderInfos() const { return m_aShaderInfo; }
        LP_FORCEINLINE ShaderInfoPtrVector& GetShaderInfos() { return m_aShaderInfo; }
        LP_FORCEINLINE const ShaderInfoGroupPtrMap& GetShaderInfoGroups() const { return m_mapShaderInfoGroup; }
        LP_FORCEINLINE ShaderInfoGroupPtrMap& GetShaderInfoGroups() { return m_mapShaderInfoGroup; }

    
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

}; //LostPeter

#endif